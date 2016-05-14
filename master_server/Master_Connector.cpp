/*
 * Master_Connector.cpp
 *
 *  Created on: 2016年5月9日
 *      Author: lijunliang
 */

#include "Master_Connector.h"
#include "Master_Manager.h"

Master_DB_Connector::Master_DB_Connector(void) { }

Master_DB_Connector::~Master_DB_Connector(void) { }

Master_DB_Connector *Master_DB_Connector::instance_ = 0;

Master_DB_Connector *Master_DB_Connector::instance(void) {
	if (! instance_)
		instance_ = new Master_DB_Connector;
	return instance_;
}

void Master_DB_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			MASTER_MANAGER->push_master_db_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

