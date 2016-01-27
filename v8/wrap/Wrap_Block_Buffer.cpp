/*
 * Wrap_Block_Buffer.cpp
 *
 *  Created on: 2016年1月27日
 *      Author: zhangyalei
 */

#include "V8_Manager.h"
#include "V8_Class.h"
#include "V8_Property.h"

int V8_Manager::wrap_block(void) {
	Isolate* isolate = context_->isolate();
	HandleScope scope(isolate);

	class_<Block_Buffer> block_buffer(isolate);
	block_buffer
		.ctor()
		.set("finish_message", &Block_Buffer::finish_message)
	;

	class_<MSG_120200> msg_120200(isolate);
	msg_120200
		.ctor()
		.set("serialize", &MSG_120200::serialize)
	;

	context_->set("Block_Buffer", block_buffer);

	return 0;
}

