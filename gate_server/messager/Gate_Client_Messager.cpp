/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#include "Gate_Client_Messager.h"
#include "Gate_Manager.h"
#include "Msg_Define.h"
#include "Common_Func.h"

Gate_Client_Messager::Gate_Client_Messager(void) { }

Gate_Client_Messager::~Gate_Client_Messager(void) { }

Gate_Client_Messager *Gate_Client_Messager::instance_;

Gate_Client_Messager *Gate_Client_Messager::instance(void) {
	if (! instance_)
		instance_ = new Gate_Client_Messager;
	return instance_;
}

int Gate_Client_Messager::process_block(Block_Buffer &buf) {
	int32_t player_cid = buf.read_int32();
	int16_t len = buf.read_int16();
	int32_t serial_cipher = buf.read_int32();
	int32_t msg_time_cipher = buf.read_int32();
	int32_t msg_id = buf.read_int32();
	int32_t  status = buf.read_int32();

	LOG_DEBUG("player_cid:%d, len:%d, serial_cipher:%llu, msg_time_cipher:%llu, msg_id:%ld, status:%d", player_cid, len, serial_cipher, msg_time_cipher, msg_id, status);
	if (msg_id >= CLIENT_GATE_MESSAGE_START && msg_id <= CLIENT_GATE_MESSAGE_END) {
		return process_gate_block(player_cid, msg_id, buf);
	}

	Gate_Player *player = 0;
	if ((player = GATE_MANAGER->find_cid_gate_player(player_cid)) == 0) {
		LOG_DEBUG("cannot find cid = %d player object. msg_id = %d ", player_cid, msg_id);
		return GATE_MANAGER->close_client(player_cid);
	}

	 /// 校验包, 用于防截包/自组包/重复发包
	int ret = player->verify_msg_info(serial_cipher, msg_time_cipher);
	if (ret < 0) {
		return -1;
	}

	if (msg_id >= CLIENT_GAME_MESSAGE_START && msg_id <= CLIENT_GAME_MESSAGE_END) {
		Block_Buffer player_buf;
		player_buf.reset();
		player_buf.make_player_message(msg_id, status, player_cid);
		player_buf.copy(&buf);
		player_buf.finish_message();
		process_game_block(msg_id, player_buf);
	}
	else
	{
		LOG_INFO("error msg_id:%d", msg_id);
	}

	return 0;
}

int Gate_Client_Messager::process_gate_block(int cid, int msg_id, Block_Buffer &buf) {
	Perf_Mon perf_mon(msg_id);
	int ret = 0;
	switch (msg_id) {
	case REQ_CONNECT_GATE: {
		connect_gate(cid, buf);
		break;
	}
	case REQ_HEARTBEAT: {
		refresh_heartbeat(cid, buf);
		break;
	}
	default:
		break;
	}
	return ret;
}

int Gate_Client_Messager::process_game_block(int msg_id, Block_Buffer &buf) {
	Perf_Mon perf_mon(msg_id);
	return GATE_MANAGER->send_to_game(buf);
}

int Gate_Client_Messager::connect_gate(int cid, Block_Buffer &buf) {
	MSG_100002 msg;
	msg.deserialize(buf);

	Gate_Player *player = 0;
	//校验是否重复登录
	if((player = GATE_MANAGER->find_account_gate_player(msg.account) )== 0) {
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
		LOG_DEBUG("connect_gate, repeat login, cid:%d, account:%s, session:%s", cid, msg.account.c_str(), msg.session.c_str());
		Block_Buffer res_buf;
		res_buf.make_inner_message(RES_CLIENT_LOGIN, ERROR_LOGIN_VERIFY_FAIL);
		res_buf.finish_message();
		GATE_MANAGER->send_to_client(cid, res_buf);
		GATE_MANAGER->close_client(cid);
	}

	return 0;
}

int Gate_Client_Messager::refresh_heartbeat(int cid, Block_Buffer &buf) {
	MSG_100003 msg;
	msg.deserialize(buf);
	MSG_500003 res_msg;
	res_msg.client_time = msg.client_time;
	res_msg.server_time = GATE_MANAGER->tick_time().sec();
	Block_Buffer res_buf;
	res_buf.make_inner_message(RES_HEARTBEAT);
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	GATE_MANAGER->send_to_client(cid, res_buf);
	return 0;
}
