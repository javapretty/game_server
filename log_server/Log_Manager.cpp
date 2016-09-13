/*
 * Log_Manager.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#include "Log_Timer.h"
#include "Log_Manager.h"
#include "Log_Server.h"
#include "Log_DB.h"

Log_Manager::Log_Manager(void) { }

Log_Manager::~Log_Manager(void) { }

Log_Manager *Log_Manager::instance_;

Log_Manager *Log_Manager::instance(void) {
	if (! instance_)
		instance_ = new Log_Manager;
	return instance_;
}

int Log_Manager::init(int server_id) {
	LOG_DB->init();
	LOG_TIMER->thr_create();

	return 0;
}

void Log_Manager::run_handler(void) {
	process_list();
	return ;
}

int Log_Manager::push_data_block(Block_Buffer *buf) {
	if (! buf)
		return -1;

	block_list_.push_back(buf);
	return 0;
}

int Log_Manager::push_tick(int x) {
	tick_list_.push_back(x);
	return 0;
}

int Log_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;
		if ((buf = block_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				process_block(*buf);
			} else {
				buf->reset();
			}
			LOG_SERVER->push_block(cid, buf);
		}
		if (! tick_list_.empty()) {
			all_empty = false;
			tick_list_.pop_front();
			tick();
		}
		if (all_empty) {
			Time_Value::sleep(Time_Value(0,100));
		}
	}

	return 0;
}

int Log_Manager::process_block(Block_Buffer &buf) {
	int32_t cid = 0;
	int16_t len = 0;
	int32_t msg_id = 0;
	int32_t status = 0;
	buf.read_int32(cid);
	buf.read_int16(len);
	buf.read_int32(msg_id);
	buf.read_int32(status);

	switch (msg_id) {
	case 999999: {
		log_file_.process_log_file_block(buf);
		break;
	}
	default: {
		Struct_Id_Map::iterator iter = LOG_DB->db_struct_id_map().find(msg_id);
		if(iter != LOG_DB->db_struct_id_map().end()){
			iter->second->save_data(buf);
		}
		break;
	}
	}
	return 0;
}

int Log_Manager::tick(void) {
	return 0;
}
