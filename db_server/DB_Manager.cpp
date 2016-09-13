/*
 * DB_Manager.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Server_Config.h"
#include "DB_Server.h"
#include "DB_Manager.h"
#include "Mongo_Operator.h"
#include "Mysql_Operator.h"

DB_Manager::DB_Manager(void) :
	db_struct_id_map_(get_hash_table_size(512)),
	db_struct_name_map_(get_hash_table_size(512)),
	db_cache_id_map_(get_hash_table_size(50000)),
	db_cache_account_map_(get_hash_table_size(50000)),
	load_player_num_(0),
	create_player_num_(0),
	save_player_num_(0),
	db_data_num_(0)	,
	db_type_(0)
{ }

DB_Manager::~DB_Manager(void) { }

DB_Manager *DB_Manager::instance_ = 0;

DB_Manager *DB_Manager::instance(void) {
	if (! instance_)
		instance_ = new DB_Manager;
	return instance_;
}

void DB_Manager::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

int DB_Manager::init(int server_id) {
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	if (server_misc == Json::Value::null) {
		LOG_FATAL("db init, server_misc null");
		return -1;
	}
	db_type_ = server_misc["db_type"].asInt();
	load_struct(server_misc["game_db_struct_path"].asString().c_str(), (Struct_Type)db_type_, db_struct_id_map_, db_struct_name_map_);

	DB_Worker *worker = 0;
	for (int i = 0; i < 10; ++i) {
		if ((worker = db_worker_pool_.pop()) == 0) {
			LOG_FATAL("db_worker_pool_ pop fatal");
			continue;
		}
		db_worker_vec_.push_back(worker);
	}

	if (db_type_ == MONGO_STRUCT)	{
		MONGO_INSTANCE->init();
	} else if (db_type_ == MYSQL_STRUCT) {
		MYSQL_INSTANCE->init();
	} else {
		LOG_FATAL("db init type = %d error abort", db_type_);
	}

	return 0;
}

int DB_Manager::start(void) {
	for (DB_Worker_Vector::iterator it = db_worker_vec_.begin(); it != db_worker_vec_.end(); ++it) {
		(*it)->thr_create();
	}
	return 0;
}

int DB_Manager::push_data_block(Block_Buffer *buf) {
	int read_idx = buf->get_read_idx();
	int32_t cid = 0;
	int16_t len = 0;
	int32_t msg_id = 0;
	buf->read_int32(cid);
	buf->read_int16(len);
	buf->read_int32(msg_id);
	buf->set_read_idx(read_idx);

	switch (msg_id) {
	case SYNC_GAME_DB_LOAD_PLAYER: {
		db_worker_vec_[load_player_num_++ % 10]->push_load_player(buf);
		break;
	}
	case SYNC_GAME_DB_CREATE_PLAYER: {
		db_worker_vec_[create_player_num_++ % 10]->push_create_player(buf);
		break;
	}
	case SYNC_GAME_DB_SAVE_PLAYER: {
		db_worker_vec_[save_player_num_++ % 10]->push_save_player(buf);
		break;
	}
	default : {
		db_worker_vec_[db_data_num_++ % 10]->push_data_block(buf);
	}
	}

	//LOG_DEBUG("push db message load_player_num_=%d, create_player_num_=%d, save_player_num_=%d",
	//		load_player_num_, create_player_num_, save_player_num_);
	return 0;
}

int DB_Manager::send_data_block(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		LOG_ERROR("cid = %d", cid);
		return -1;
	}
	return DB_SERVER->send_block(cid, buf);
}

int64_t DB_Manager::create_player(Create_Role_Info &role_info) {
	if (db_type_ == MONGO_STRUCT)	{
		return MONGO_INSTANCE->create_player(role_info);
	} else if (db_type_ == MYSQL_STRUCT) {
		return MYSQL_INSTANCE->create_player(role_info);
	} else {
		return -1;
	}
}

int64_t DB_Manager::create_guild(Create_Guild_Info &guild_info) {
	if (db_type_ == MONGO_STRUCT)	{
		return MONGO_INSTANCE->create_guild(guild_info);
	} else if (db_type_ == MYSQL_STRUCT) {
		return MYSQL_INSTANCE->create_guild(guild_info);
	} else {
		return -1;
	}
}
