/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Login_Inner_Messager.h"
#include "Login_Manager.h"

Login_Inner_Messager::Login_Inner_Messager(void) { }

Login_Inner_Messager::~Login_Inner_Messager(void) { }

Login_Inner_Messager *Login_Inner_Messager::instance_;

Login_Inner_Messager *Login_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Login_Inner_Messager;
	return instance_;
}

int Login_Inner_Messager::process_gate_block(Block_Buffer &buf) {
	LOGIN_MANAGER->add_recv_bytes(buf.readable_bytes());

	int32_t cid = 0;
	int16_t len = 0;
	int32_t msg_id = 0;
	int32_t status = 0;
	int32_t player_cid = 0;
	buf.read_int32(cid);
	buf.read_int16(len);
	buf.read_int32(msg_id);
	buf.read_int32(status);
	buf.read_int32(player_cid);

	LOGIN_MANAGER->add_msg_count(msg_id);
	switch (msg_id) {
	case SYNC_GATE_LOGIN_PLAYER_ACCOUNT:{
		MSG_140000 msg;
		if (msg.deserialize(buf) ==0){
			gate_login_player_account(cid, player_cid, msg);
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

int Login_Inner_Messager::gate_login_player_account(int gate_cid, int32_t player_cid, MSG_140000& msg) {
	Block_Buffer gate_buf;
	Login_Player *player = dynamic_cast<Login_Player*>(LOGIN_MANAGER->find_account_player(msg.account));
	if (player && player->session_info().session == msg.session
			&& player->session_info().gate_ip == msg.gate_ip
			&& player->session_info().gate_port == msg.gate_port) {
		//验证玩家session成功，关闭玩家与login的连接，开启玩家与gate的连接
		gate_buf.make_player_message(SYNC_LOGIN_GATE_PLAYER_ACCOUNT, 0, player_cid);
		LOGIN_MANAGER->close_client(0, player->player_cid(), ERROR_SESSION_SUCCESS);
	} else {
		LOG_DEBUG("login check session error, session:%s, account:%s, gate_ip:%s, gate_port:%d, gate_cid:%d, player_cid:%d",
				msg.session.c_str(), msg.account.c_str(), msg.gate_ip.c_str(), msg.gate_port, gate_cid, player_cid);
		gate_buf.make_player_message(SYNC_LOGIN_GATE_PLAYER_ACCOUNT, ERROR_CLIENT_SESSION, player_cid);
	}
	MSG_140001 gate_msg;
	gate_msg.account = msg.account;
	gate_msg.serialize(gate_buf);
	gate_buf.finish_message();
	LOGIN_MANAGER->send_to_gate(gate_cid, gate_buf);

	return 0;
}
