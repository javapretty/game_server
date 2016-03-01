/*
 * DB_Manager.cpp
 *
 *  Created on: Dec30,2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "DB_Server.h"
#include "DB_Manager.h"
#include "DB_Operator.h"

DB_Manager::DB_Manager(void) :
	work_num_(0),
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
	CACHED_INSTANCE->init();
#ifdef LOCAL_DEBUG
	DB_MANAGER->set(3);
#else
	DB_MANAGER->set(get_hash_table_size(10));
#endif

	DB_Worker *worker = 0;
	for (int i = 0; i < work_num_; ++i) {
		if ((worker = db_worker_pool_.pop()) == 0) {
			LOG_INFO("db_worker_pool_.pop() return 0.");
			continue;
		}
		db_worker_vec_.push_back(worker);
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
	/*int32_t cid*/ buf->read_int32();
	/*int16_t len*/ buf->read_int16();
	int32_t msg_id = buf->read_int32();
	buf->set_read_idx(read_idx);

	switch (msg_id) {
	case SYNC_GAME_DB_LOAD_PLAYER_INFO: {
		db_worker_vec_[load_player_num_++ % db_worker_vec_.size()]->push_load_player(buf);
		break;
	}
	case SYNC_GAME_DB_CREATE_PLAYER: {
		db_worker_vec_[create_player_num_++ % db_worker_vec_.size()]->push_create_player(buf);
		break;
	}
	case SYNC_GAME_DB_SAVE_PLAYER_INFO: {
		db_worker_vec_[save_player_num_++ % db_worker_vec_.size()]->push_save_player(buf);
		break;
	}
	default : {
		db_worker_vec_[db_data_num_++ % db_worker_vec_.size()]->push_data_block(buf);
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
