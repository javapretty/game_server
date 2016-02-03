/*
 * DB_Operator_A.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "DB_Operator.h"
#include "DB_Fields.h"
#include "Configurator.h"
#include "DB_Manager.h"
#include "mongo/client/dbclient.h"

using namespace mongo;

DB_Operator::DB_Operator(void) : agent_num_(0), server_num_(0), agent_and_server_prefix_(0), min_server_diff_key_(0), max_server_diff_key_(0){ }

DB_Operator::~DB_Operator(void) { }

DB_Operator *DB_Operator::instance_;

DB_Operator *DB_Operator::instance(void) {
	if (instance_ == 0)
		instance_ = new DB_Operator;
	return instance_;
}

DBClientConnection &DB_Operator::connection(void) {
	GUARD(MUTEX, mon, connection_map_lock_);

	const int64_t thread_key = pthread_self();
	DBClientConnection *conn = 0;
	Connection_Map::iterator it = connection_map_.find(thread_key);
	if (it == connection_map_.end()) {
		conn = connection_pool_.pop();

		std::string err_msg;
		std::stringstream host_stream;
		const Json::Value &mongodb_server = CONFIG_INSTANCE->server_maintainer()["mongodb_server"];
		if (mongodb_server == Json::Value::null) {
			MSG_ABORT("cannot find mongodb_server");
		}
		host_stream << (mongodb_server["ip"].asString());
		if (mongodb_server.isMember("port")) {
			host_stream << ":" << (mongodb_server["port"].asInt());
		}
		conn->connect(host_stream.str().c_str(), err_msg);
		connection_map_[thread_key] = conn;
	} else {
		conn = it->second;
	}

  return *conn;
}

int DB_Operator::init(void) {
	/// 创建所有索引
	this->create_index();

	/// 初始化agent_num_,server_num_和server_map_
	server_map_.clear();
	const Json::Value &servers_config = CONFIG_INSTANCE->server_list();
	if (servers_config == Json::Value::null || servers_config["server_list"].size() == 0) {
		MSG_ABORT("configure file error");
	}

	int agent = 0;
	int server = 0;
	for (uint i = 0; i < servers_config["server_list"].size(); ++i) {
		agent = servers_config["server_list"][i]["agent_num"].asInt();
		server = servers_config["server_list"][i]["server_num"].asInt();
		if (agent < 100 || agent > 999 || server < 1000 || server > 9999) {
			continue;
		}

		if (agent_num_ == 0 || server_num_ == 0) {
			agent_num_ = agent;
			server_num_ = server;
		}

		Int_IntSet_Map::iterator it = server_map_.find(agent);
		if (it == server_map_.end()) {
			Int_Set server_set;
			server_set.clear();
			server_set.insert(server);
			server_map_.insert(std::make_pair(agent, server_set));
		} else {
			it->second.insert(server);
		}
	}

	uint server_amount = 0;
	for (Int_IntSet_Map::const_iterator it = server_map_.begin(); it != server_map_.end(); ++it) {
		server_amount += it->second.size();
	}
	if (server_amount != servers_config["server_list"].size()) {
		MSG_ABORT("server_list is error.");
	}

	if (agent_num_ < 100 || agent_num_ > 999 || server_num_ < 1000 || server_num_ > 9999) {
		MSG_ABORT("agent_num_ = %d, server_num_ = %d", agent_num_, server_num_);
	}
	int64_t agent_l = agent_num_ * 10000000000000L;
	int64_t server_l = server_num_ * 1000000000L;
	agent_and_server_prefix_ = agent_l + server_l;
	min_server_diff_key_ = agent_and_server_prefix_ + 1;
	max_server_diff_key_ = agent_and_server_prefix_ + 999999999L;

	return 0;
}

int DB_Operator::create_index(void) {
	{//global
		CACHED_CONNECTION.createIndex(Global_Fields::COLLECTION, BSON(Global_Fields::KEY << 1));

		if (CACHED_CONNECTION.count(Global_Fields::COLLECTION, BSON(Global_Fields::KEY << Global_Fields::ROLE)) == 0) {
			BSONObjBuilder builder;
			builder << Global_Fields::KEY << Global_Fields::ROLE
					<< Global_Fields::ID << 0;

			CACHED_CONNECTION.update(Global_Fields::COLLECTION, MONGO_QUERY(Global_Fields::KEY << Global_Fields::ROLE),
					BSON("$set" << builder.obj()), true);
		}
	}
	{
		//role
		CACHED_CONNECTION.createIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ROLE_ID << 1));
		CACHED_CONNECTION.createIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ROLE_NAME << 1));
		CACHED_CONNECTION.createIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ACCOUNT << 1));

		CACHED_CONNECTION.createIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ACCOUNT << 1 << Role_Fields::AGENT_NUM << 1 << Role_Fields::SERVER_NUM << 1));
		CACHED_CONNECTION.createIndex(Role_Fields::COLLECTION, BSON(Role_Fields::ACCOUNT << 1 << Role_Fields::LEVEL << -1));
	}
	{ // mail index
		CACHED_CONNECTION.createIndex(Mail_Fields::COLLECTION, BSON(Mail_Fields::ROLE_ID << 1));
	}
	return 0;
}

int DB_Operator::load_db_cache(int cid) {
	unsigned long long res_count = CACHED_CONNECTION.count(Role_Fields::COLLECTION);
	BSONObjBuilder field_builder;
	field_builder 	<< Role_Fields::ROLE_ID << 1
					<< Role_Fields::ACCOUNT << 1
					<< Role_Fields::ROLE_NAME << 1
					<< Role_Fields::AGENT_NUM << 1
					<< Role_Fields::SERVER_NUM << 1
					<< Role_Fields::ROLE_GENDER << 1
					<< Role_Fields::CAREER << 1
					<< Role_Fields::LEVEL << 1
					<< Role_Fields::LAST_SIGN_OUT_TIME << 1;

	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);

	CACHED_CONNECTION.findN(iter_record, Role_Fields::COLLECTION, mongo::Query(), res_count, 0, &field_bson);

	MSG_550000 msg;
	for (std::vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj obj = *iter;

		Player_DB_Cache db_cache;
		db_cache.role_id = obj[Role_Fields::ROLE_ID].numberLong();
		db_cache.account = obj[Role_Fields::ACCOUNT].valuestrsafe();
		db_cache.role_name = obj[Role_Fields::ROLE_NAME].valuestrsafe();
		db_cache.agent_num = obj[Role_Fields::AGENT_NUM].numberInt();
		db_cache.server_num = obj[Role_Fields::SERVER_NUM].numberInt();
		db_cache.gender = obj[Role_Fields::ROLE_GENDER].numberInt();
		db_cache.career = obj[Role_Fields::CAREER].numberInt();
		db_cache.level = obj[Role_Fields::LEVEL].numberInt();
		msg.db_cache_vec.push_back(db_cache);
	}

	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_LOAD_DB_CACHE);
	msg.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

role_id_t DB_Operator::create_init_player(Game_Player_Info &player_info) {
#ifdef LOCAL_DEBUG
	player_info.agent_num = player_info.agent_num ? player_info.agent_num : 1;
	player_info.server_num = player_info.server_num ? player_info.server_num : 1;

#else
	Int_IntSet_Map::const_iterator it = server_map_.find(player_info.agent_num);
	if (it == server_map_.end()) {
		MSG_USER("player_info.agent_num = [%d] is not find.", player_info.agent_num);
		return -1;
	}
	if (it->second.count(player_info.server_num) == 0) {
		MSG_USER("player_info.server_num = [%d] is not find.", player_info.server_num);
		return -1;
	}
#endif

	BSONObj fields = BSON(Role_Fields::ACCOUNT << 1 << Role_Fields::ROLE_ID << 1 << Role_Fields::ROLE_NAME << 1);
	BSONObj res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION,
			MONGO_QUERY(Role_Fields::ROLE_NAME << player_info.role_name), &fields);

	if (res.isEmpty() == false) {
		MSG_USER("role_name = [%s] has already existed.", player_info.role_name.c_str());
		return -1;
	}

	BSONObj cmd = fromjson("{findandmodify:'global', query:{key:'role'}, update:{$inc:{id:1}}}");
	if (CACHED_CONNECTION.runCommand("mmo", cmd, res) == false) {
		MSG_USER("increase role key failed.");
		return -1;
	}

	int order = res.getFieldDotted("value.id").numberLong() + 1;
	int64_t agent = player_info.agent_num * 10000000000000L;
	int64_t server = player_info.server_num * 1000000000L;
	role_id_t role_id = agent + server + order;

	player_info.role_id = role_id;
	int now_sec = Time_Value::gettimeofday().sec();
	CACHED_CONNECTION.update(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << ((long long int)role_id)), BSON("$set" <<
			BSON(Role_Fields::ACCOUNT << player_info.account
					<< Role_Fields::AGENT_NUM << player_info.agent_num
					<< Role_Fields::SERVER_NUM << player_info.server_num
					<< Role_Fields::ROLE_ID << (long long int)role_id
					<< Role_Fields::CAREER << 0
					<< Role_Fields::ROLE_NAME << player_info.role_name
					<< Role_Fields::ROLE_GENDER << player_info.gender
					<< Role_Fields::LEVEL << 1
					<< Role_Fields::LAST_SIGN_IN_TIME << now_sec
					<< Role_Fields::LAST_SIGN_OUT_TIME << now_sec)), true);
	return role_id;
}

int DB_Operator::has_role_by_account(std::string &account, const int agent_num, const int server_num) {
	BSONObj res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ACCOUNT << account << Role_Fields::AGENT_NUM << agent_num << Role_Fields::SERVER_NUM << server_num));
	if (res.isEmpty()) return 0;
	else return 1;
}

role_id_t DB_Operator::get_role_id(const std::string &account, const int agent_num, const int server_num) {
	BSONObj res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ACCOUNT << account << Role_Fields::AGENT_NUM << agent_num << Role_Fields::SERVER_NUM << server_num));
	if (res.isEmpty()) {
		res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ACCOUNT << account).sort(BSON(Role_Fields::LEVEL << -1)));
		if (!res.isEmpty()) {
			return res[Role_Fields::ROLE_ID].numberLong();
		}
		return 0;
	}
	else return res[Role_Fields::ROLE_ID].numberLong();
}

int DB_Operator::get_role_name_by_id(role_id_t role_id, std::string &name) {
	BSONObj res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
	if (res.isEmpty()) return 0;
	name = res[Role_Fields::ROLE_NAME].valuestrsafe();
	return 1;
}

int DB_Operator::get_account_by_id(role_id_t role_id, std::string &account) {
	BSONObj res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
	if (res.isEmpty()) return 0;
	account = res[Role_Fields::ACCOUNT].valuestrsafe();
	return 1;
}

int DB_Operator::has_role_by_id(role_id_t role_id) {
	BSONObj res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)role_id));
	if (res.isEmpty()) return 0;
	else return 1;
}


/////////////////// Base Info DB Operator ///////////////////
int DB_Operator::load_player_info(Game_Player_Info &player_info) {
	BSONObj res = CACHED_CONNECTION.findOne(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)player_info.role_id));
	if (! res.isEmpty()) {
		player_info.role_id = res[Role_Fields::ROLE_ID].numberLong();
		player_info.account = res[Role_Fields::ACCOUNT].valuestrsafe();
		player_info.role_name = res[Role_Fields::ROLE_NAME].valuestrsafe();
		player_info.gender = res[Role_Fields::ROLE_GENDER].numberInt();
		player_info.career = res[Role_Fields::CAREER].numberInt();
		player_info.create_time = res[Role_Fields::CREATE_TIME].numberInt();
		player_info.level = res[Role_Fields::LEVEL].numberInt();
		player_info.last_sign_in_time = res[Role_Fields::LAST_SIGN_IN_TIME].numberInt();
		player_info.last_sign_out_time = res[Role_Fields::LAST_SIGN_OUT_TIME].numberInt();

		player_info.agent_num = res[Role_Fields::AGENT_NUM].numberInt();
		if (player_info.agent_num == 0) {
			player_info.server_num = player_info.role_id / 10000000000000L;
		}
		player_info.server_num = res[Role_Fields::SERVER_NUM].numberInt();
		if (player_info.server_num == 0) {
			player_info.server_num = player_info.role_id / 1000000000L % 10000L;
		}

		if (res.hasField(Role_Fields::IP.c_str()))
			player_info.ip = res[Role_Fields::IP].valuestrsafe();

		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::save_player_info(Game_Player_Info &player_info) {
	BSONObjBuilder builder;
	builder << Role_Fields::ROLE_ID << (long long int)player_info.role_id
			<< Role_Fields::AGENT_NUM << player_info.agent_num
			<< Role_Fields::SERVER_NUM << player_info.server_num
			<< Role_Fields::ACCOUNT << player_info.account
			<< Role_Fields::ROLE_NAME << player_info.role_name
			<< Role_Fields::ROLE_GENDER << player_info.gender
			<< Role_Fields::CAREER << player_info.career
			<< Role_Fields::CREATE_TIME << player_info.create_time
			<< Role_Fields::LEVEL << player_info.level
			<< Role_Fields::LAST_SIGN_IN_TIME << player_info.last_sign_in_time
			<< Role_Fields::LAST_SIGN_OUT_TIME << player_info.last_sign_out_time
			<< Role_Fields::IP << (player_info.ip);


	CACHED_CONNECTION.update(Role_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)player_info.role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_bag_info(Bag_Info &bag_info) {
	BSONObj res = CACHED_CONNECTION.findOne(Bag_Fields::COLLECTION,
			MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)bag_info.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	bag_info.capacity.bag_cap = res[Bag_Fields::BAG_CAPACITY].numberInt();
	bag_info.capacity.storage_cap = res[Bag_Fields::STORAGE_CAPACITY].numberInt();

	BSONObj money = res.getObjectField(Bag_Fields::MONEY.c_str());
	bag_info.money_info.bind_copper = money[Bag_Fields::Money::BIND_COPPER].numberInt();
	bag_info.money_info.copper = money[Bag_Fields::Money::COPPER].numberInt();
	bag_info.money_info.bind_gold = money[Bag_Fields::Money::BIND_GOLD].numberInt();
	bag_info.money_info.gold = money[Bag_Fields::Money::GOLD].numberInt();

	BSONObjIterator iter(res.getObjectField(Bag_Fields::ITEM.c_str()));
	BSONObj obj;
	while (iter.more()){
		obj = iter.next().embeddedObject();
		Item_Info item;
		if (load_item_detail(obj, item) == 0) {
			bag_info.item_map[item.item_basic.index] = item;
		}
	}
	return 0;
}

int DB_Operator::save_bag_info(Bag_Info &bag_info) {
	std::vector<BSONObj> vc_item;
	BSONObj obj;
	for (Bag_Info::Item_Map::const_iterator iter = bag_info.item_map.begin(); iter != bag_info.item_map.end(); ++iter) {
		save_item_detail(iter->second, obj);
		vc_item.push_back(obj);
	}

	BSONObjBuilder tmp_builder;
	tmp_builder << Bag_Fields::BAG_CAPACITY << bag_info.capacity.bag_cap
		<< Bag_Fields::STORAGE_CAPACITY << bag_info.capacity.storage_cap
		<< Bag_Fields::MONEY << BSON(Bag_Fields::Money::BIND_COPPER << bag_info.money_info.bind_copper
		<< Bag_Fields::Money::COPPER << bag_info.money_info.copper
		<< Bag_Fields::Money::BIND_GOLD << bag_info.money_info.bind_gold
		<< Bag_Fields::Money::GOLD << bag_info.money_info.gold)
		<< Bag_Fields::ITEM << vc_item;

	CACHED_CONNECTION.update(Bag_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)bag_info.role_id),
			BSON("$set" << tmp_builder.obj()), true);

	return 0;
}

int DB_Operator::load_item_detail(const BSONObj &obj, Item_Info &item) {
	int result = -1;
	if (obj.hasField(Bag_Fields::Item::INDEX.c_str()) && obj.hasField(Bag_Fields::Item::ID.c_str())
			&& obj.hasField(Bag_Fields::Item::AMOUNT.c_str()) && obj.hasField(Bag_Fields::Item::BIND.c_str())) {
		result = 0;
		item.reset();
		// set_basic为指针以及固定的值如hole分配了空间
		item.set_basic(obj[Bag_Fields::Item::INDEX].numberInt(), obj[Bag_Fields::Item::ID].numberInt(),
				obj[Bag_Fields::Item::AMOUNT].numberInt(), obj[Bag_Fields::Item::BIND].numberInt());
	}

	return result;
}

int DB_Operator::save_item_detail(const Item_Info &item, mongo::BSONObj &obj) {
	BSONObjBuilder item_builder;
	item_builder << Bag_Fields::Item::INDEX << item.item_basic.index << Bag_Fields::Item::ID
			<< item.item_basic.id << Bag_Fields::Item::AMOUNT << item.item_basic.amount
			<< Bag_Fields::Item::BIND << item.item_basic.bind;
	obj = item_builder.obj();
	return 0;
}

int DB_Operator::load_mail_info(Mail_Info &mail_info) {
	BSONObj res = CACHED_CONNECTION.findOne(Mail_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)mail_info.role_id));
	if (res.isEmpty()) {
		return -1;
	}

	mail_info.total_count = res[Mail_Fields::TOTAL_COUNT.c_str()].numberInt();

	BSONObj mail = res.getObjectField(Mail_Fields::MAIL_DETAIL.c_str());
	BSONObjIterator iter(mail);
	while (iter.more()) {
		BSONObj obj = iter.next().embeddedObject();
		Mail_Detail mail_detail;
		mail_detail.mail_id = obj[Mail_Fields::Mail_Detail::MAIL_ID].numberInt();
		mail_detail.pickup = obj[Mail_Fields::Mail_Detail::PICKUP].numberInt();
		mail_detail.send_time = obj[Mail_Fields::Mail_Detail::SEND_TIME].numberInt();
		mail_detail.sender_type = obj[Mail_Fields::Mail_Detail::SENDER_TYPE].numberInt();
		mail_detail.sender_id = obj[Mail_Fields::Mail_Detail::SENDER_ID].numberLong();
		mail_detail.sender_name = obj[Mail_Fields::Mail_Detail::SENDER_NAME].valuestrsafe();
		mail_detail.mail_title = obj[Mail_Fields::Mail_Detail::MAIL_TITLE].valuestrsafe();
		mail_detail.mail_content = obj[Mail_Fields::Mail_Detail::MAIL_CONTENT].valuestrsafe();
		mail_detail.money_info.copper = obj[Mail_Fields::Mail_Detail::COPPER].numberInt();
		mail_detail.money_info.bind_copper = obj[Mail_Fields::Mail_Detail::BIND_COPPER].numberInt();
		mail_detail.money_info.gold = obj[Mail_Fields::Mail_Detail::GOLD].numberInt();
		mail_detail.money_info.bind_gold = obj[Mail_Fields::Mail_Detail::BIND_GOLD].numberInt();

		//加载附件信息
		BSONObjIterator item_iter(obj.getObjectField(Mail_Fields::Mail_Detail::ITEM.c_str()));
		while (item_iter.more()) {
			BSONObj item_obj = item_iter.next().embeddedObject();
			Item_Info item;
			int result = load_item_detail(item_obj, item);
			if (result != 0)
				continue;
			mail_detail.item_vector.push_back(item.item_basic);
		}
		mail_info.mail_map.insert(std::make_pair(mail_detail.mail_id, mail_detail));
	}

	return 0;
}

int DB_Operator::save_mail_info(Mail_Info &mail_info) {
	std::vector<BSONObj> mail_vector;
	for (Mail_Info::Mail_Map::const_iterator iter = mail_info.mail_map.begin();
			iter != mail_info.mail_map.end(); iter++) {

		std::vector<BSONObj> item_vector;
		BSONObj obj;
		for (std::vector<Item_Basic_Info>::const_iterator it = iter->second.item_vector.begin();
				it != iter->second.item_vector.end(); ++it) {
			save_item_detail(Item_Info(*it), obj);
			item_vector.push_back(obj);
		}

		mail_vector.push_back(BSON(Mail_Fields::Mail_Detail::MAIL_ID << iter->second.mail_id
				<< Mail_Fields::Mail_Detail::PICKUP << iter->second.pickup
				<< Mail_Fields::Mail_Detail::SEND_TIME << iter->second.send_time
				<< Mail_Fields::Mail_Detail::SENDER_TYPE << iter->second.sender_type
				<< Mail_Fields::Mail_Detail::SENDER_ID << (long long int)iter->second.sender_id
				<< Mail_Fields::Mail_Detail::SENDER_NAME << iter->second.sender_name
				<< Mail_Fields::Mail_Detail::MAIL_TITLE << iter->second.mail_title
				<< Mail_Fields::Mail_Detail::MAIL_CONTENT << iter->second.mail_content
				<< Mail_Fields::Mail_Detail::ITEM << item_vector
				<< Mail_Fields::Mail_Detail::COPPER << iter->second.money_info.copper
				<< Mail_Fields::Mail_Detail::BIND_COPPER<< iter->second.money_info.bind_copper
				<< Mail_Fields::Mail_Detail::GOLD << iter->second.money_info.gold
				<< Mail_Fields::Mail_Detail::BIND_GOLD << iter->second.money_info.bind_gold));
	}

	BSONObjBuilder set_builder;
	set_builder << Mail_Fields::TOTAL_COUNT << mail_info.total_count
			<< Mail_Fields::MAIL_DETAIL << mail_vector;

	CACHED_CONNECTION.update(Mail_Fields::COLLECTION, MONGO_QUERY(Role_Fields::ROLE_ID << (long long int)mail_info.role_id),
			BSON("$set" << set_builder.obj() ), true);

	return 0;
}
