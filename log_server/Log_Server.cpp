/*
 * Log_Server.cpp
 *
 *  Created on: Dec30,2015
 *      Author: zhangyalei
 */

#include "Log_Server.h"
#include "Log_Manager.h"

Log_Server::Log_Server(void) { }

Log_Server::~Log_Server(void) { }

Log_Server *Log_Server::instance_ = 0;

Log_Server *Log_Server::instance(void) {
	if (! instance_)
		instance_ = new Log_Server;
	return instance_;
}

void Log_Server::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			LOG_MANAGER->push_data_block(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(SLEEP_TIME);
		}
	}
}
