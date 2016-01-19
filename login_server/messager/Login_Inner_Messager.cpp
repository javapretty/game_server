/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#include "Msg_Define.h"
#include "Login_Inner_Messager.h"
#include "Login_Manager.h"
#include "Log.h"

Login_Inner_Messager::Login_Inner_Messager(void) { }

Login_Inner_Messager::~Login_Inner_Messager(void) { }

Login_Inner_Messager *Login_Inner_Messager::instance_;

Login_Inner_Messager *Login_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Login_Inner_Messager;
	return instance_;
}

int Login_Inner_Messager::process_gate_block(Block_Buffer &buf) {
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

	Perf_Mon perf_mon(msg_id);
	LOGIN_MANAGER->inner_msg_count(msg_id);

	switch (msg_id) {
	case SYNC_GATE_LOGIN_PLAYER_ACCOUNT:{
		MSG_112000 msg;
		if (msg.deserialize(buf) ==0){
			process_112000(gate_cid, player_cid, msg);
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

int Login_Inner_Messager::process_self_loop_block(Block_Buffer &buf) {
	uint16_t len = 0;
	uint32_t msg_id = 0;
	int32_t status = 0;

	buf.read_uint16(len);
	buf.read_uint32(msg_id);
	buf.read_int32(status);

	Perf_Mon perf_mon(msg_id);

	switch (msg_id) {
	case SYNC_INNER_TIMER_TICK: {
		LOGIN_MANAGER->tick();
		break;
	}
	default:
		break;
	}

	return 0;
}

int Login_Inner_Messager::process_112000(int gate_cid, int32_t player_cid, MSG_112000& msg) {
	MSG_112001 gate_msg;
	gate_msg.account = msg.account;

	Block_Buffer gate_buf;
	//session check ok
	if (LOGIN_MANAGER->find_account_session(msg.account, msg.session) == 0){
		LOGIN_MANAGER->unbind_account_session(msg.account);
		gate_buf.make_message(SYNC_LOGIN_GATE_PLAYER_ACCOUNT, 0, player_cid);
	}
	//session check error
	else
	{
		LOG_DEBUG("can not find the session begin send message to gate");
		gate_buf.make_message(SYNC_LOGIN_GATE_PLAYER_ACCOUNT, ERROR_CLIENT_SESSION, player_cid);
	}
	gate_msg.serialize(gate_buf);
	gate_buf.finish_message();
	LOGIN_MANAGER->send_to_gate(gate_cid, gate_buf);

	return 0;
}

