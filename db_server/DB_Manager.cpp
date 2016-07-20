/*
 * DB_Manager.cpp
 *
 *  Created on: Dec30,2015
 *      Author: zhangyalei
 */

#include <Mongo_Operator.h>
#include "Server_Config.h"
#include "Msg_Define.h"
#include "DB_Server.h"
#include "DB_Manager.h"
#include "Mongo_Struct.h"
#include "Mysql_Struct.h"

DB_Manager::DB_Manager(void) :
	load_player_num_(0),
	create_player_num_(0),
	save_player_num_(0),
	db_data_num_(0)	{ }

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

int DB_Manager::init(void) {
	MONGO_INSTANCE->init();

	DB_Worker *worker = 0;
	for (int i = 0; i < 10; ++i) {
		if ((worker = db_worker_pool_.pop()) == 0) {
			LOG_FATAL("db_worker_pool_ pop fatal");
			continue;
		}
		db_worker_vec_.push_back(worker);
	}

	load_struct("config/struct/game_struct.xml");
	return 0;
}

int DB_Manager::load_struct(const char *path){
	int db_type = SERVER_CONFIG->server_misc()["db_type"].asInt();
	Xml xml;
	xml.load_xml(path);
	TiXmlNode *node = xml.get_root_node();
	XML_LOOP_BEGIN(node)
		DB_Struct *def = nullptr;
		if (db_type == MONGODB)	{
			def = new Mongo_Struct(xml, node);
		} else if (db_type == MYSQL) {
			def = new Mysql_Struct(xml, node);
		} else {
			LOG_FATAL("db init type = %d error abort", db_type);
		}

		if(def->msg_id() != 0) {
			db_struct_id_map_.insert(std::pair<int32_t, DB_Struct*>(def->msg_id(), def));
		}
		db_struct_name_map_.insert(std::pair<std::string, DB_Struct*>(def->struct_name(), def));
	XML_LOOP_END(node)
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
	/*int32_t cid*/ buf->read_int32();
	/*int16_t len*/ buf->read_int16();
	int32_t msg_id = buf->read_int32();
	buf->set_read_idx(read_idx);

	switch (msg_id) {
	case SYNC_GAME_DB_LOAD_PLAYER_INFO: {
		db_worker_vec_[load_player_num_++ % 10]->push_load_player(buf);
		break;
	}
	case SYNC_GAME_DB_CREATE_PLAYER: {
		db_worker_vec_[create_player_num_++ % 10]->push_create_player(buf);
		break;
	}
	case SYNC_GAME_DB_SAVE_PLAYER_INFO: {
		db_worker_vec_[save_player_num_++ % 10]->push_save_player(buf);
		break;
	}
	default : {
		db_worker_vec_[db_data_num_++ % 10]->push_data_block(buf);
	}
	}

	LOG_DEBUG("push db message load_player_num_=%d, create_player_num_=%d, save_player_num_=%d",
			load_player_num_, create_player_num_, save_player_num_);
	return 0;
}

int DB_Manager::send_data_block(int cid, Block_Buffer &buf) {
	if (cid < 2) {
		LOG_INFO("cid = %d", cid);
		return -1;
	}
	return DB_SERVER->send_block(cid, buf);
}
