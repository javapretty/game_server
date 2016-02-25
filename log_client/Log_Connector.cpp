/*
 * Log_Connector.cpp
 *
 *  Created on: Dec30,2015
 *      Author: zhangyalei
 */

#include "Log_Connector.h"

Log_Connector::Log_Connector(void) { }

Log_Connector::~Log_Connector(void) { }

Log_Connector *Log_Connector::instance_ = 0;

Log_Connector *Log_Connector::instance(void) {
	if (! instance_)
		instance_ = new Log_Connector;
	return instance_;
}

int Log_Connector::push_data_block(Block_Buffer &buf, bool use_swap) {
	if (buf.readable_bytes() == 0)
		return -1;

	Block_Buffer *msg_buf = block_pool_.pop();
	if (! msg_buf)
		return -1;

	msg_buf->reset();

	if (use_swap) {
		msg_buf->swap(buf);
	} else {
		msg_buf->copy(&buf);
	}

	block_list_.push_back(msg_buf);

	return 0;
}

void Log_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (! block_list_.empty()) {
			buf = block_list_.front();
			block_list_.pop_front();
			send_block(get_cid(), *buf);
			block_pool_.push(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
