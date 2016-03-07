/*
 * DB_Operator_A.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "DB_Operator.h"
#include "Server_Config.h"
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
		const Json::Value &mongodb_server = SERVER_CONFIG->server_misc()["mongodb_server"];
		if (mongodb_server == Json::Value::null) {
			LOG_FATAL("cannot find mongodb_server");
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
	const Json::Value &servers_config = SERVER_CONFIG->server_list();
	if (servers_config == Json::Value::null || servers_config["server_list"].size() == 0) {
		LOG_FATAL("configure file error");
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
		LOG_FATAL("server_list is error.");
	}

	if (agent_num_ < 100 || agent_num_ > 999 || server_num_ < 1000 || server_num_ > 9999) {
		LOG_FATAL("agent_num_ = %d, server_num_ = %d", agent_num_, server_num_);
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
		CACHED_CONNECTION.createIndex("mmo.global", BSON("key" << 1));

		if (CACHED_CONNECTION.count("mmo.global", BSON("key" << "role")) == 0) {
			BSONObjBuilder builder;
			builder << "key" << "role" << "id" << 0;

			CACHED_CONNECTION.update("mmo.global", MONGO_QUERY("key" << "role"),
					BSON("$set" << builder.obj()), true);
		}
	}
	{
		//role
		CACHED_CONNECTION.createIndex("mmo.role", BSON("role_id" << 1));
		CACHED_CONNECTION.createIndex("mmo.role", BSON("role_name" << 1));
		CACHED_CONNECTION.createIndex("mmo.role", BSON("account" << 1));

		CACHED_CONNECTION.createIndex("mmo.role", BSON("account" << 1 << "agent_num" << 1 << "server_num" << 1));
		CACHED_CONNECTION.createIndex("mmo.role", BSON("account" << 1 << "level" << -1));
	}
	{ // hero index
		CACHED_CONNECTION.createIndex("mmo.hero", BSON("role_id" << 1));
	}
	{ // bag index
		CACHED_CONNECTION.createIndex("mmo.bag", BSON("role_id" << 1));
	}
	{ // mail index
		CACHED_CONNECTION.createIndex("mmo.mail", BSON("role_id" << 1));
	}
	return 0;
}

int DB_Operator::load_db_cache(int cid) {
	unsigned long long res_count = CACHED_CONNECTION.count("mmo.role");
	BSONObjBuilder field_builder;
	field_builder << "role_id" << 1
					<< "account" << 1
					<< "role_name" << 1
					<< "agent_num" << 1
					<< "server_num" << 1
					<< "level" << 1
					<< "gender" << 1
					<< "career" << 1
					<< "last_sign_out_time" << 1;

	BSONObj field_bson = field_builder.obj();
	std::vector<BSONObj> iter_record;
	iter_record.reserve(res_count);

	CACHED_CONNECTION.findN(iter_record, "mmo.role", mongo::Query(), res_count, 0, &field_bson);

	MSG_550000 msg;
	for (std::vector<BSONObj>::iterator iter = iter_record.begin(); iter != iter_record.end(); ++iter) {
		BSONObj obj = *iter;

		Player_DB_Cache db_cache;
		db_cache.role_id = obj["role_id"].numberLong();
		db_cache.account = obj["account"].valuestrsafe();
		db_cache.role_name = obj["role_name"].valuestrsafe();
		db_cache.agent_num = obj["agent_num"].numberInt();
		db_cache.server_num = obj["server_num"].numberInt();
		db_cache.level = obj["level"].numberInt();
		db_cache.gender = obj["gender"].numberInt();
		db_cache.career = obj["career"].numberInt();
		msg.db_cache_vec.push_back(db_cache);
	}

	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_LOAD_DB_CACHE);
	msg.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int64_t DB_Operator::create_init_player(Game_Player_Info &player_info) {
#ifdef LOCAL_DEBUG
	player_info.agent_num = player_info.agent_num ? player_info.agent_num : 1;
	player_info.server_num = player_info.server_num ? player_info.server_num : 1;

#else
	Int_IntSet_Map::const_iterator it = server_map_.find(player_info.agent_num);
	if (it == server_map_.end()) {
		LOG_INFO("player_info.agent_num = [%d] is not find.", player_info.agent_num);
		return -1;
	}
	if (it->second.count(player_info.server_num) == 0) {
		LOG_INFO("player_info.server_num = [%d] is not find.", player_info.server_num);
		return -1;
	}
#endif

	BSONObj fields = BSON("account" << 1 << "role_id" << 1 << "role_name" << 1);
	BSONObj res = CACHED_CONNECTION.findOne("mmo.role",
			MONGO_QUERY("role_name" << player_info.role_name), &fields);

	if (res.isEmpty() == false) {
		LOG_INFO("role_name = [%s] has already existed.", player_info.role_name.c_str());
		return -1;
	}

	BSONObj cmd = fromjson("{findandmodify:'global', query:{key:'role'}, update:{$inc:{id:1}}}");
	if (CACHED_CONNECTION.runCommand("mmo", cmd, res) == false) {
		LOG_INFO("increase role key failed.");
		return -1;
	}

	int order = res.getFieldDotted("value.id").numberLong() + 1;
	int64_t agent = player_info.agent_num * 10000000000000L;
	int64_t server = player_info.server_num * 1000000000L;
	int64_t role_id = agent + server + order;

	player_info.role_id = role_id;
	int now_sec = Time_Value::gettimeofday().sec();
	CACHED_CONNECTION.update("mmo.role", MONGO_QUERY("role_id" << ((long long int)role_id)), BSON("$set" <<
			BSON("role_id" << (long long int)role_id
					<< "agent_num" << player_info.agent_num
					<< "server_num" << player_info.server_num
					<< "account" << player_info.account
					<< "role_name" << player_info.role_name
					<< "level" << 1
					<< "gender" << player_info.gender
					<< "career" << 0
					<< "last_sign_in_time" << now_sec
					<< "last_sign_out_time" << now_sec)), true);
	return role_id;
}

bool DB_Operator::has_role_by_account(std::string &account, const int agent_num, const int server_num) {
	BSONObj res = CACHED_CONNECTION.findOne("mmo.role", MONGO_QUERY("account" << account << "agent_num" << agent_num << "server_num" << server_num));
	if (res.isEmpty()) {
		return false;
	} else {
		return true;
	}
}

int64_t DB_Operator::get_role_id(const std::string &account, const int agent_num, const int server_num) {
	BSONObj res = CACHED_CONNECTION.findOne("mmo.role", MONGO_QUERY("account" << account << "agent_num" << agent_num << "server_num" << server_num));
	if (res.isEmpty()) {
		res = CACHED_CONNECTION.findOne("mmo.role", MONGO_QUERY("account" << account).sort(BSON("level" << -1)));
		if (!res.isEmpty()) {
			return res["role_id"].numberLong();
		}
		return 0;
	}
	else return res["role_id"].numberLong();
}

/////////////////// Base Info DB Operator ///////////////////
int DB_Operator::load_player_info(int64_t role_id, Game_Player_Info &player_info) {
	BSONObj res = CACHED_CONNECTION.findOne("mmo.role", MONGO_QUERY("role_id" << (long long int)role_id));
	if (! res.isEmpty()) {
		player_info.role_id = res["role_id"].numberLong();
		player_info.account = res["account"].valuestrsafe();
		player_info.role_name = res["role_name"].valuestrsafe();
		player_info.client_ip = res["client_ip"].valuestrsafe();
		player_info.agent_num = res["agent_num"].numberInt();
		if (player_info.agent_num == 0) {
			player_info.server_num = player_info.role_id / 10000000000000L;
		}
		player_info.server_num = res["server_num"].numberInt();
		if (player_info.server_num == 0) {
			player_info.server_num = player_info.role_id / 1000000000L % 10000L;
		}
		player_info.level = res["level"].numberInt();
		player_info.exp = res["exp"].numberInt();
		player_info.gender = res["gender"].numberInt();
		player_info.career = res["career"].numberInt();
		player_info.create_time = res["create_time"].numberInt();
		player_info.last_sign_in_time = res["last_sign_in_time"].numberInt();
		player_info.last_sign_out_time = res["last_sign_out_time"].numberInt();
		player_info.vitality = res["vitality"].numberInt();
		player_info.last_change_time = res["last_change_time"].numberLong();
		player_info.today_buy = res["today_buy"].numberInt();
		player_info.vip = res["vip"].numberInt();
		player_info.vip_exp = res["vip_exp"].numberInt();
		player_info.charge_gold = res["charge_gold"].numberInt();
		return 0;
	} else {
		return -1;
	}
}

int DB_Operator::save_player_info(int64_t role_id, Game_Player_Info &player_info) {
	BSONObjBuilder builder;
	builder << "role_id" << (long long int)player_info.role_id
			<< "account" << player_info.account
			<< "role_name" << player_info.role_name
			<< "client_ip" << (player_info.client_ip)
			<< "agent_num" << player_info.agent_num
			<< "server_num" << player_info.server_num
			<< "level" << player_info.level
			<< "exp" << player_info.exp
			<< "gender" << player_info.gender
			<< "career" << player_info.career
			<< "create_time" << player_info.create_time
			<< "last_sign_in_time" << player_info.last_sign_in_time
			<< "last_sign_out_time" << player_info.last_sign_out_time
			<< "vitality" << player_info.vitality
			<< "last_change_time" <<  (long long int)player_info.last_change_time
			<< "today_buy" << player_info.today_buy
			<< "vip" << player_info.vip
			<< "vip_exp" << player_info.vip_exp
			<< "charge_gold" << player_info.charge_gold;

	CACHED_CONNECTION.update("mmo.role", MONGO_QUERY("role_id" << (long long int)role_id),
			BSON("$set" << builder.obj()), true);

	return 0;
}

int DB_Operator::load_hero_info(int64_t role_id, Hero_Info &hero_info) {
	BSONObj res = CACHED_CONNECTION.findOne("mmo.hero",
			MONGO_QUERY("role_id" << (long long int)role_id));
	if (res.isEmpty()) {
		return -1;
	}

	BSONObjIterator iter(res.getObjectField("hero_detail"));
	BSONObj obj;
	while (iter.more()){
		obj = iter.next().embeddedObject();
		Hero_Detail hero_detail;
		hero_detail.hero_id = obj["hero_id"].numberInt();
		hero_detail.level = obj["level"].numberInt();
		hero_detail.exp = obj["exp"].numberInt();
		hero_detail.star = obj["star"].numberInt();
		hero_detail.quality = obj["quality"].numberInt();

		BSONObjIterator iter_equip(obj.getObjectField("equip"));
		BSONObj obj_equip;
		while (iter_equip.more()){
			obj_equip = iter_equip.next().embeddedObject();
			Item_Info item_info;
			item_info.item_id = obj_equip["item_id"].numberInt();
			item_info.amount = obj_equip["amount"].numberInt();
			item_info.level = obj_equip["level"].numberInt();
			hero_detail.equip_info.push_back(item_info);
		}

		BSONObjIterator iter_property(obj.getObjectField("property"));
		BSONObj obj_property;
		while (iter_property.more()){
			obj_property = iter_property.next().embeddedObject();
			Property_Detail property_detail;
			property_detail.type = obj_property["type"].numberInt();
			property_detail.value = obj_property["value"].numberInt();
			hero_detail.property_info.push_back(property_detail);
		}

		hero_info.hero_map.insert(std::make_pair(hero_detail.hero_id, hero_detail));
	}
	return 0;
}

int DB_Operator::save_hero_info(int64_t role_id, Hero_Info &hero_info) {
	std::vector<BSONObj> hero_vec;
	for (Hero_Info::Hero_Map::const_iterator iter = hero_info.hero_map.begin(); iter != hero_info.hero_map.end(); ++iter) {
		std::vector<BSONObj> equip_vec;
		for (std::vector<Item_Info>::const_iterator it = iter->second.equip_info.begin(); it != iter->second.equip_info.end(); ++it) {
			equip_vec.push_back(BSON("item_id" << it->item_id << "amount" << it->amount << "level" << it->level));
		}

		std::vector<BSONObj> property_vec;
		for (std::vector<Property_Detail>::const_iterator it = iter->second.property_info.begin(); it != iter->second.property_info.end(); ++it) {
			property_vec.push_back(BSON("type" << it->type << "value" << it->value));
		}

		hero_vec.push_back(BSON("hero_id" << iter->second.hero_id
				<< "level" << iter->second.level
				<< "exp" << iter->second.exp
				<< "star" << iter->second.star
				<< "quality" << iter->second.quality
				<< "equip" << equip_vec
				<< "property" << property_vec));
	}

	BSONObjBuilder tmp_builder;
	tmp_builder << "hero_detail" << hero_vec;

	CACHED_CONNECTION.update("mmo.hero", MONGO_QUERY("role_id" << (long long int)role_id),
			BSON("$set" << tmp_builder.obj()), true);

	return 0;
}

int DB_Operator::load_bag_info(int64_t role_id, Bag_Info &bag_info) {
	BSONObj res = CACHED_CONNECTION.findOne("mmo.bag",
			MONGO_QUERY("role_id" << (long long int)role_id));
	if (res.isEmpty()) {
		return -1;
	}

	bag_info.copper = res["copper"].numberInt();
	bag_info.gold = res["gold"].numberInt();

	BSONObjIterator iter(res.getObjectField("item"));
	BSONObj obj;
	while (iter.more()){
		obj = iter.next().embeddedObject();
		Item_Info item;
		if (load_item_info(obj, item) == 0) {
			bag_info.item_map[item.item_id] = item;
		}
	}
	return 0;
}

int DB_Operator::save_bag_info(int64_t role_id, Bag_Info &bag_info) {
	std::vector<BSONObj> item_vec;
	BSONObj obj;
	for (Bag_Info::Item_Map::const_iterator iter = bag_info.item_map.begin(); iter != bag_info.item_map.end(); ++iter) {
		save_item_info(iter->second, obj);
		item_vec.push_back(obj);
	}

	BSONObjBuilder tmp_builder;
	tmp_builder << "copper" << bag_info.copper
		<< "gold" << bag_info.gold
		<< "item" << item_vec;

	CACHED_CONNECTION.update("mmo.bag", MONGO_QUERY("role_id" << (long long int)role_id),
			BSON("$set" << tmp_builder.obj()), true);

	return 0;
}

int DB_Operator::load_item_info(const BSONObj &obj, Item_Info &item) {
	item.reset();
	item.item_id = obj["item_id"].numberInt();
	item.amount = obj["amount"].numberInt();
	item.level = obj["level"].numberInt();

	return 0;
}

int DB_Operator::save_item_info(const Item_Info &item, mongo::BSONObj &obj) {
	BSONObjBuilder item_builder;
	item_builder << "item_id" << item.item_id << "amount" << item.amount << "level" << item.level;
	obj = item_builder.obj();
	return 0;
}

int DB_Operator::load_mail_info(int64_t role_id, Mail_Info &mail_info) {
	BSONObj res = CACHED_CONNECTION.findOne("mmo.mail", MONGO_QUERY("role_id" << (long long int)role_id));
	if (res.isEmpty()) {
		return -1;
	}

	mail_info.total_count = res["total_count"].numberInt();

	BSONObj mail_obj = res.getObjectField("mail_detail");
	BSONObjIterator iter(mail_obj);
	while (iter.more()) {
		BSONObj obj = iter.next().embeddedObject();
		Mail_Detail mail_detail;
		mail_detail.mail_id = obj["mail_id"].numberInt();
		mail_detail.pickup = obj["pickup"].numberInt();
		mail_detail.send_time = obj["send_time"].numberInt();
		mail_detail.sender_type = obj["sender_type"].numberInt();
		mail_detail.sender_id = obj["sender_id"].numberLong();
		mail_detail.sender_name = obj["sender_name"].valuestrsafe();
		mail_detail.mail_title = obj["mail_title"].valuestrsafe();
		mail_detail.mail_content = obj["mail_content"].valuestrsafe();
		mail_detail.copper = obj["copper"].numberInt();
		mail_detail.gold = obj["gold"].numberInt();

		BSONObjIterator item_iter(res.getObjectField("item"));
		BSONObj item_obj;
		while (item_iter.more()){
			item_obj = iter.next().embeddedObject();
			Item_Info item;
			if (load_item_info(item_obj, item) == 0) {
				mail_detail.item_info.push_back(item);
			}
		}

		mail_info.mail_map.insert(std::make_pair(mail_detail.mail_id, mail_detail));
	}

	return 0;
}

int DB_Operator::save_mail_info(int64_t role_id, Mail_Info &mail_info) {
	std::vector<BSONObj> mail_vec;
	for (Mail_Info::Mail_Map::const_iterator iter = mail_info.mail_map.begin();
			iter != mail_info.mail_map.end(); iter++) {
		std::vector<BSONObj> item_vec;
		BSONObj item_obj;
		for (std::vector<Item_Info>::const_iterator it = iter->second.item_info.begin(); it != iter->second.item_info.end(); ++it) {
			save_item_info(*it, item_obj);
			item_vec.push_back(item_obj);
		}

		mail_vec.push_back(BSON("mail_id" << iter->second.mail_id
				<< "pickup" << iter->second.pickup
				<< "send_time" << iter->second.send_time
				<< "sender_type" << iter->second.sender_type
				<< "sender_id" << (long long int)iter->second.sender_id
				<< "sender_name" << iter->second.sender_name
				<< "mail_title" << iter->second.mail_title
				<< "mail_content" << iter->second.mail_content
				<< "copper" << iter->second.copper
				<< "gold" << iter->second.gold
				<< "item" << item_obj));
	}

	BSONObjBuilder set_builder;
	set_builder << "total_count" << mail_info.total_count
			<< "mail_detail" << mail_vec;

	CACHED_CONNECTION.update("mmo.mail", MONGO_QUERY("role_id" << (long long int)role_id),
			BSON("$set" << set_builder.obj() ), true);

	return 0;
}
