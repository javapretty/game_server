/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Gate_Client_Messager.h"
#include "Gate_Manager.h"

Gate_Client_Messager::Gate_Client_Messager(void) { }

Gate_Client_Messager::~Gate_Client_Messager(void) { }

Gate_Client_Messager *Gate_Client_Messager::instance_;

Gate_Client_Messager *Gate_Client_Messager::instance(void) {
	if (! instance_)
		instance_ = new Gate_Client_Messager;
	return instance_;
}

int Gate_Client_Messager::process_block(Block_Buffer &buf) {
	GATE_MANAGER->add_recv_bytes(buf.readable_bytes());

	int32_t cid = 0;
	int16_t len = 0;
	int32_t msg_id = 0;
	int32_t status = 0;
	int32_t serial_cipher = 0;
	int32_t msg_time_cipher = 0;
	buf.read_int32(cid);
	buf.read_int16(len);
	buf.read_int32(msg_id);
	buf.read_int32(status);
	buf.read_int32(serial_cipher);
	buf.read_int32(msg_time_cipher);

	GATE_MANAGER->add_msg_count(msg_id);
	if (msg_id >= CLIENT_GATE_MESSAGE_START && msg_id <= CLIENT_GATE_MESSAGE_END) {
		return process_gate_block(cid, msg_id, buf);
	}

	Gate_Player *player = dynamic_cast<Gate_Player*>(GATE_MANAGER->find_cid_player(cid));
	if (!player) {
		LOG_ERROR("cannot find player_cid = %d msg_id = %d ", cid, msg_id);
		return GATE_MANAGER->close_client(0, cid, ERROR_NOT_LOGIN);
	}

	 /// 校验包, 用于防截包/自组包/重复发包
	if (GATE_MANAGER->verify_pack()) {
		int result = player->verify_msg_info(serial_cipher, msg_time_cipher);
		if (result != 0) {
			LOG_ERROR("msg verify error, player_cid:%d, len:%d, serial_cipher:%d, msg_time_cipher:%d, msg_id:%d, status:%d", cid, len, serial_cipher, msg_time_cipher, msg_id, status);
			return GATE_MANAGER->close_client(0, cid, result);
		}
	}

	Block_Buffer player_buf;
	player_buf.reset();
	player_buf.make_player_message(msg_id, status, cid);
	player_buf.copy(&buf);
	player_buf.finish_message();

	if (msg_id >= CLIENT_MASTER_MESSAGE_START && msg_id <= CLIENT_MASTER_MESSAGE_END) {
		GATE_MANAGER->send_to_master(player_buf);
	} else if (msg_id >= CLIENT_GAME_MESSAGE_START && msg_id <= CLIENT_GAME_MESSAGE_END) {
		GATE_MANAGER->send_to_game(player->game_cid(), player_buf);
	}
	else {
		LOG_ERROR("msg_id:%d error", msg_id);
	}

	return 0;
}

int Gate_Client_Messager::process_gate_block(int cid, int msg_id, Block_Buffer &buf) {
	int ret = 0;
	switch (msg_id) {
	case REQ_CONNECT_GATE: {
		connect_gate(cid, buf);
		break;
	}
	case REQ_SEND_HEARTBEAT: {
		send_heartbeat(cid, buf);
		break;
	}
	default:
		break;
	}
	return ret;
}

int Gate_Client_Messager::connect_gate(int cid, Block_Buffer &buf) {
	MSG_100101 msg;
	msg.deserialize(buf);

	Gate_Player *player = dynamic_cast<Gate_Player*>(GATE_MANAGER->find_account_player(msg.account));
	//校验是否重复登录
	if(!player) {
		MSG_140000 login_msg;
		login_msg.account = msg.account;
		login_msg.session = msg.session;
		GATE_MANAGER->get_server_ip_port(cid, login_msg.gate_ip, login_msg.gate_port);
		Block_Buffer login_buf;
		login_buf.make_player_message(SYNC_GATE_LOGIN_PLAYER_ACCOUNT, 0, cid);
		login_msg.serialize(login_buf);
		login_buf.finish_message();
		GATE_MANAGER->send_to_login(login_buf);
	}
	//重复登录
	else
	{
		LOG_WARN("connect_gate, repeat connect, cid:%d, account:%s, session:%s", cid, msg.account.c_str(), msg.session.c_str());
		GATE_MANAGER->close_client(0, cid, ERROR_DISCONNECT_RELOGIN);
	}

	return 0;
}

int Gate_Client_Messager::send_heartbeat(int cid, Block_Buffer &buf) {
	MSG_100102 msg;
	msg.deserialize(buf);
	MSG_500102 res_msg;
	res_msg.client_time = msg.client_time;
	res_msg.server_time = GATE_MANAGER->tick_time().sec();
	Block_Buffer res_buf;
	res_buf.make_server_message(RES_SEND_HEARTBEAT, 0);
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	GATE_MANAGER->send_to_client(cid, res_buf);
	return 0;
}
