/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Login_Inner_Messager.h"
#include "Login_Manager.h"
#include "Login_Player.h"

Login_Inner_Messager::Login_Inner_Messager(void) { }

Login_Inner_Messager::~Login_Inner_Messager(void) { }

Login_Inner_Messager *Login_Inner_Messager::instance_;

Login_Inner_Messager *Login_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Login_Inner_Messager;
	return instance_;
}

int Login_Inner_Messager::process_gate_block(Block_Buffer &buf) {
	int32_t gate_cid = buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();
	int32_t player_cid = buf.read_int32();

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
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();

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
	Block_Buffer gate_buf;

	Login_Player *player = 0;
	//session check ok
	if ((player = LOGIN_MANAGER->find_account_login_player(msg.account)) != 0
			&& player->login_player_info().session == msg.session
			&& player->login_player_info().gate_ip == msg.gate_ip
			&& player->login_player_info().gate_port == msg.gate_port) {
		gate_buf.make_player_message(SYNC_LOGIN_GATE_PLAYER_ACCOUNT, 0, player_cid);
		LOGIN_MANAGER->close_client(player->get_cid());
	}
	//session check error
	else
	{
		MSG_DEBUG("login check session wrong, gate_cid:%d, player_cid:%d",gate_cid, player_cid);
		gate_buf.make_player_message(SYNC_LOGIN_GATE_PLAYER_ACCOUNT, ERROR_CLIENT_SESSION, player_cid);
	}
	MSG_112001 gate_msg;
	gate_msg.account = msg.account;
	gate_msg.serialize(gate_buf);
	gate_buf.finish_message();
	LOGIN_MANAGER->send_to_gate(gate_cid, gate_buf);

	return 0;
}

