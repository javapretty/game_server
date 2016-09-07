/*
 * DB_Server.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "DB_Server.h"
#include "DB_Manager.h"

DB_Server::DB_Server(void) { }

DB_Server::~DB_Server(void) { }

DB_Server *DB_Server::instance_ = 0;

DB_Server *DB_Server::instance(void) {
	if (! instance_)
		instance_ = new DB_Server;
	return instance_;
}

void DB_Server::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			DB_MANAGER->push_data_block(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
