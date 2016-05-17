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
			LOG_FATAL("server_misc config cannot find mongodb_server");
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
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	const Json::Value &server_list = server_misc["server_list"];
	if (server_misc == Json::Value::null || server_list == Json::Value::null || server_list.size() == 0) {
		LOG_FATAL("server_list config error");
	}

	for (uint i = 0; i < server_list.size(); ++i) {
		int agent_num = server_list[i]["agent_num"].asInt();
		int server_num = server_list[i]["server_num"].asInt();
		if (agent_num < 1 || agent_num > 999 || server_num < 1 || server_num > 9999) {
			LOG_FATAL("agent_num = %d, server_num = %d, config error", agent_num, server_num);
			return -1;
		}

		Int_IntSet_Map::iterator it = server_map_.find(agent_num);
		if (it == server_map_.end()) {
			Int_Set server_set;
			server_set.clear();
			server_set.insert(server_num);
			server_map_.insert(std::make_pair(agent_num, server_set));
		} else {
			it->second.insert(server_num);
		}
	}

	agent_num_ = server_misc["agent_num"].asInt();
	server_num_ = server_misc["server_num"].asInt();
	if (agent_num_ < 1 || agent_num_ > 999 || server_num_ < 1 || server_num_ > 9999) {
		LOG_FATAL("agent_num = %d, server_num = %d, config error", agent_num_, server_num_);
		return -1;
	}
	agent_and_server_prefix_ = agent_num_ * 10000000000000L + server_num_ * 1000000000L;
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
		//guild
		CACHED_CONNECTION.createIndex("mmo.guild", BSON("guild_id" << 1));
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
	{ // shop index
		CACHED_CONNECTION.createIndex("mmo.shop", BSON("role_id" << 1));
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
	player_info.agent_num = player_info.agent_num ? player_info.agent_num : 1;
	player_info.server_num = player_info.server_num ? player_info.server_num : 1;

	Int_IntSet_Map::const_iterator it = server_map_.find(player_info.agent_num);
	if (it == server_map_.end()) {
		LOG_INFO("player_info.agent_num = %d not exist, account = %s", player_info.agent_num, player_info.account.c_str());
		return -1;
	}
	if (it->second.count(player_info.server_num) == 0) {
		LOG_INFO("player_info.server_num = %d not exist, account = %s", player_info.server_num, player_info.account.c_str());
		return -1;
	}
	if (player_info.agent_num != agent_num_ || player_info.server_num != server_num_) {
		LOG_INFO("player_info.agent = %d server_num = %d account = %s error, login wrong server", player_info.agent_num, player_info.server_num, player_info.account.c_str());
		return -1;
	}

	BSONObj fields = BSON("account" << 1 << "role_id" << 1 << "role_name" << 1);
	BSONObj res = CACHED_CONNECTION.findOne("mmo.role",
			MONGO_QUERY("role_name" << player_info.role_name), &fields);

	if (!res.isEmpty()) {
		LOG_INFO("role_name = %s has existed.", player_info.role_name.c_str());
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
		player_info.buy_vitality_times = res["buy_vitality_times"].numberInt();
		player_info.vip_level = res["vip_level"].numberInt();
		player_info.vip_exp = res["vip_exp"].numberInt();
		player_info.charge_gold = res["charge_gold"].numberInt();
		player_info.skill_point = res["skill_point"].numberInt();
		player_info.recover_skill_time = res["recover_skill_time"].numberLong();
		player_info.exchange_count = res["exchange_count"].numberInt();
		player_info.guild_id = res["guild_id"].numberLong();
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
			<< "buy_vitality_times" << player_info.buy_vitality_times
			<< "vip_level" << player_info.vip_level
			<< "vip_exp" << player_info.vip_exp
			<< "charge_gold" << player_info.charge_gold
			<< "skill_point" << player_info.skill_point
			<< "recover_skill_time" << (long long int)player_info.recover_skill_time
			<< "exchange_count" << player_info.exchange_count
			<< "guild_id" << player_info.guild_id;

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
		hero_detail.energy = obj["energy"].numberInt();

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
	for (boost::unordered_map<int32_t,Hero_Detail>::const_iterator iter = hero_info.hero_map.begin(); iter != hero_info.hero_map.end(); ++iter) {
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
				<< "energy" << iter->second.energy
				<< "equip" << equip_vec
				<< "property" << property_vec));
	}

	BSONObjBuilder tmp_builder;
	tmp_builder << "role_id" << (long long int)role_id << "hero_detail" << hero_vec;

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
	for (boost::unordered_map<int32_t,Item_Info>::const_iterator iter = bag_info.item_map.begin(); iter != bag_info.item_map.end(); ++iter) {
		save_item_info(iter->second, obj);
		item_vec.push_back(obj);
	}

	BSONObjBuilder tmp_builder;
	tmp_builder << "role_id" << (long long int)role_id
		<< "copper" << bag_info.copper
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
	item.exp = obj["exp"].numberInt();

	return 0;
}

int DB_Operator::save_item_info(const Item_Info &item, mongo::BSONObj &obj) {
	BSONObjBuilder item_builder;
	item_builder << "item_id" << item.item_id
			<< "amount" << item.amount
			<< "level" << item.level
			<< "exp" << item.exp;
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
	for (std::map<int,Mail_Detail>::const_iterator iter = mail_info.mail_map.begin();
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
				<< "item" << item_vec));
	}

	BSONObjBuilder set_builder;
	set_builder << "role_id" << (long long int)role_id
			<< "total_count" << mail_info.total_count
			<< "mail_detail" << mail_vec;

	CACHED_CONNECTION.update("mmo.mail", MONGO_QUERY("role_id" << (long long int)role_id),
			BSON("$set" << set_builder.obj() ), true);

	return 0;
}

int DB_Operator::load_shop_info(int64_t role_id, Shop_Info &shop_info) {
	BSONObj res = CACHED_CONNECTION.findOne("mmo.shop", MONGO_QUERY("role_id" << (long long int)role_id));
	if (res.isEmpty()) {
		return -1;
	}

	BSONObj shop_obj = res.getObjectField("shop_detail");
	BSONObjIterator iter(shop_obj);
	while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			Shop_Detail shop_detail;
			shop_detail.shop_type = obj["shop_type"].numberInt();
			shop_detail.refresh_count = obj["refresh_count"].numberInt();

			BSONObj shop_obj = res.getObjectField("products");
			BSONObjIterator iter(shop_obj);
			while (iter.more()) {
				BSONObj obj = iter.next().embeddedObject();
				Product_Info product_info;
				product_info.product_id = obj["product_id"].numberInt();
				product_info.item_id = obj["item_id"].numberInt();
				product_info.price = obj["price"].numberInt();
				product_info.count = obj["count"].numberInt();

				shop_detail.products.push_back(product_info);
			}
		shop_info.shop_detail.insert(std::make_pair(shop_detail.shop_type, shop_detail));
	}

	return 0;
}

int DB_Operator::save_shop_info(int64_t role_id, Shop_Info &shop_info) {
	std::vector<BSONObj> shop_vec;
	for (boost::unordered_map<int,Shop_Detail>::const_iterator iter = shop_info.shop_detail.begin();
			iter != shop_info.shop_detail.end(); iter++) {
		std::vector<BSONObj> product_vec;
		for (std::vector<Product_Info>::const_iterator it = iter->second.products.begin(); it != iter->second.products.end(); ++it) {
			BSONObjBuilder item_builder;
			item_builder << "product_id" << (*it).product_id
					<< "item_id" << (*it).item_id
					<< "price" << (*it).price
					<< "count" << (*it).count;
				product_vec.push_back(item_builder.obj());
		}

		shop_vec.push_back(BSON("shop_type" << iter->second.shop_type
				<< "refresh_count" << iter->second.refresh_count
				<< "products" << product_vec));
	}

	BSONObjBuilder set_builder;
	set_builder << "role_id" << (long long int)role_id << "shop_detail" << shop_vec;

	CACHED_CONNECTION.update("mmo.shop", MONGO_QUERY("role_id" << (long long int)role_id),
			BSON("$set" << set_builder.obj() ), true);

	return 0;
}

int DB_Operator::load_guild_info(Guild_Info &guild_info) {
	std::auto_ptr<DBClientCursor> cursor = CACHED_CONNECTION.query("mmo.guild", Query());
	while(cursor->more()){
		BSONObj obj = cursor->next();
		Guild_Data data;
		data.guild_id = obj["guild_id"].numberLong();
		data.guild_name = obj["guild_name"].valuestrsafe();
		data.chief_id = obj["chief_id"].numberLong();
		
		BSONObj member_obj = obj.getObjectField("member_list");
		BSONObjIterator iter(member_obj);
		while (iter.more()) {
			BSONObj obj = iter.next().embeddedObject();
			int64_t member_id = obj["member_id"].numberLong();
			data.member_list.push_back(member_id);
		}
		
		guild_info.guild_map.insert(std::make_pair(data.guild_id, data));
	}

	return 0;
}

int DB_Operator::save_guild_info(Guild_Info &guild_info) {
	for(std::map<int64_t,Guild_Data>::iterator iter = guild_info.guild_map.begin();
			iter != guild_info.guild_map.end(); iter++){
		save_guild_data(iter->second);
	}
	return 0;
}

int DB_Operator::save_guild_data(Guild_Data &guild_data) {
	std::vector<BSONObj> member_list;
	for (std::vector<int64_t>::const_iterator iter = guild_data.member_list.begin();
			iter != guild_data.member_list.end(); ++iter) {
		BSONObjBuilder playerid_builder;
		playerid_builder << "member_id" << (long long int)(*iter);
		member_list.push_back(playerid_builder.obj());
	}

	BSONObjBuilder set_builder;
	set_builder << "guild_id" << (long long int)guild_data.guild_id << "guild_name" << guild_data.guild_name
			<< "chief_id" << (long long int)guild_data.chief_id << "member_list" << member_list;

	CACHED_CONNECTION.update("mmo.guild", MONGO_QUERY("guild_id" << (long long int)guild_data.guild_id),
			BSON("$set" << set_builder.obj() ), true);

	return 0;
}

int DB_Operator::drop_guild_info(std::vector<int64_t> &guild_list){
	for(std::vector<int64_t>::iterator iter = guild_list.begin();
			iter != guild_list.end(); iter++){
		drop_guild_data(*iter);
	}
	return 0;
}

int DB_Operator::drop_guild_data(int64_t guild_id){
	CACHED_CONNECTION.remove("mmo.guild", MONGO_QUERY("guild_id" << (long long int)guild_id));
	return 0;
}
