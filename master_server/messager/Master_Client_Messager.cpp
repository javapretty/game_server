/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Master_Manager.h"
#include "Master_Player.h"
#include "Common_Func.h"
#include "Master_Client_Messager.h"

Master_Client_Messager::Master_Client_Messager(void) { }

Master_Client_Messager::~Master_Client_Messager(void) { }

Master_Client_Messager *Master_Client_Messager::instance_;

Master_Client_Messager *Master_Client_Messager::instance(void) {
	if (! instance_)
		instance_ = new Master_Client_Messager;
	return instance_;
}

int Master_Client_Messager::process_block(Block_Buffer &buf) {
	int32_t gate_cid = 0;
	uint16_t len = 0;
	uint32_t msg_id = 0;
	int32_t  status = 0;
	int32_t player_cid = 0;

	buf.read_int32(gate_cid);
	buf.read_uint16(len);
	buf.read_uint32(msg_id);
	buf.read_int32(status);
	buf.read_int32(player_cid);

	//客户端发到服务器的消息在这里处理
	Perf_Mon perf_mon(msg_id);
	int ret = 0;
	if (ret) {
		MSG_USER("error msg buf:%d", msg_id);
	}

	return 0;
}
