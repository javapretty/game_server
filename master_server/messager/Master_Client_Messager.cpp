/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Master_Client_Messager.h"
#include "Master_Manager.h"

Master_Client_Messager::Master_Client_Messager(void) { }

Master_Client_Messager::~Master_Client_Messager(void) { }

Master_Client_Messager *Master_Client_Messager::instance_;

Master_Client_Messager *Master_Client_Messager::instance(void) {
	if (! instance_)
		instance_ = new Master_Client_Messager;
	return instance_;
}

int Master_Client_Messager::process_block(Block_Buffer &buf) {
	/*int32_t gate_cid*/ buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();
	/*int32_t player_cid*/ buf.read_int32();

	//客户端发到服务器的消息在这里处理
	Perf_Mon perf_mon(msg_id);
	int ret = 0;
	if (ret) {
		MSG_USER("error msg buf:%d", msg_id);
	}

	return 0;
}
