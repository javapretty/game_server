/*
 * V8_Manager.cpp
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#include <iostream>
#include <sstream>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"
#include "V8_Manager.h"
#include "V8_Class.h"
#include "V8_Property.h"
#include "Common_Func.h"
#include "Game_Server.h"
#include "Game_Client_Messager.h"
#include "Game_Player.h"

using namespace v8;

V8_Manager::V8_Manager(void):context_(nullptr) { }

V8_Manager::~V8_Manager(void) { }

V8_Manager *V8_Manager::instance_;

V8_Manager *V8_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new V8_Manager;
	return instance_;
}

void V8_Manager::run_handler(void) {
	start_v8();
}

int V8_Manager::start_v8() {
  // Initialize V8.
  V8::InitializeICU();
  V8::InitializeExternalStartupData("");
  Platform* platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();
  context_ = new context;

  process_list();

  // Dispose the isolate_ and tear down V8.
  delete context_;
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;

  return 0;
}

int V8_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;

		/// gate-->game
		if ((buf = game_gate_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				GAME_CLIENT_MESSAGER->process_block(*buf);
			} else {
				MSG_USER("buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			GAME_GATE_SERVER->push_block(cid, buf);
		}

		if (all_empty)
			Time_Value::sleep(SLEEP_TIME);
	}
	return 0;
}

int V8_Manager::process_script(int msg_id, Block_Buffer &buf, Game_Player *player) {
	v8::HandleScope scope(context_->isolate());
	std::ostringstream ostr;
	ostr << "js/" << msg_id << ".js";
	Local<Value> result = context_->run_file(ostr.str());
	String::Utf8Value utf8(result);
	std::cout << *utf8 << std::endl;
	return 0;
}
