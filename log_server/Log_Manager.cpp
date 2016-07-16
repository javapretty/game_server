/*
 * Log_Manager.cpp
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Log_Timer.h"
#include "Log_Manager.h"
#include "Log_Server.h"
#include "Server_Config.h"
#include "Msg_Define.h"
#include <sys/stat.h>

Log_Manager::Log_Manager(void) { }

Log_Manager::~Log_Manager(void) { }

Log_Manager *Log_Manager::instance_;

Log_Manager *Log_Manager::instance(void) {
	if (! instance_)
		instance_ = new Log_Manager;
	return instance_;
}

int Log_Manager::init(void) {
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	if (server_misc == Json::Value::null) {
		LOG_FATAL("server_misc config error");
	}
	std::string mysql_ip(server_misc["mysql_server"]["ip"].asString());
	int mysql_port = server_misc["mysql_server"]["port"].asInt();
	std::string mysql_user(server_misc["mysql_server"]["user"].asString());
	std::string mysql_pw(server_misc["mysql_server"]["password"].asString());

	db_record_.set(mysql_ip.c_str(), mysql_port, mysql_user, mysql_pw);
	db_record_.init();

	LOG_TIMER->thr_create();

	return 0;
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

void Log_Manager::run_handler(void) {
	process_list();
	return ;
}

int Log_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;
		if (! block_list_.empty()) {
			all_empty = false;
			buf = block_list_.front();
			block_list_.pop_front();
			cid = buf->peek_int32();
			process_block(*buf);
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
	/*int32_t cid*/ buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	int32_t status = buf.read_int32();

	switch (msg_id) {
	case 999999: {
		file_record_.process_log_file_block(buf);
		break;
	}
	case SYNC_LOG_TEST: {
		db_record_.process_180000(msg_id, status, buf);
		break;
	case SYNC_LOG_LOGINOUT: {
		db_record_.process_180001(msg_id, status, buf);
		break;
	}
	}
	default: {
		break;
	}
	}
	return 0;
}

int Log_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	db_record_.tick(now);

	return 0;
}
