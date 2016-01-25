/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#include "Gate_Client_Messager.h"
#include "Gate_Manager.h"
#include "Gate_Player.h"
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
	int32_t player_cid = 0;		//玩家连接到gate的cid
	uint16_t len = 0;
	uint32_t serial_cipher = 0;
	uint32_t msg_time_cipher = 0;
	uint32_t msg_id = 0;
	int32_t  status = 0;

	buf.read_int32(player_cid);
	buf.read_uint16(len);
	buf.read_uint32(serial_cipher);
	buf.read_uint32(msg_time_cipher);
	buf.read_uint32(msg_id);
	buf.read_int32(status);
	LOG_DEBUG("player_cid:%d, len:%d, serial_cipher:%llu, msg_time_cipher:%llu, msg_id:%ld, status:%d", player_cid, len, serial_cipher, msg_time_cipher, msg_id, status);
	if (msg_id >= CLIENT_GATE_MESSAGE_START && msg_id <= CLIENT_GATE_MESSAGE_END) {
		return process_gate_block(player_cid, msg_id, buf);
	}

	Gate_Player *player = 0;
	if ((player = GATE_MANAGER->find_cid_gate_player(player_cid)) == 0) {
		MSG_DEBUG("cannot find cid = %d player object. msg_id = %d ", player_cid, msg_id);
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
		player_buf.make_message(msg_id, status, player_cid);
		player_buf.copy(&buf);
		player_buf.finish_message();
		process_game_block(msg_id, player_buf);
	}
	else
	{
		MSG_USER("error msg_id:%d", msg_id);
	}

	return 0;
}

int Gate_Client_Messager::process_gate_block(int cid, int msg_id, Block_Buffer &buf) {
	Perf_Mon perf_mon(msg_id);
	int ret = 0;
	switch (msg_id) {
	case REQ_HEARTBEAT: {
		MSG_111000 msg;
		if ((ret = msg.deserialize(buf)) == 0) {
			refresh_heartbeat(cid, msg);
		}
		break;
	}
	case REQ_CONNECT_GATE: {
		MSG_111001 msg;
		if ((ret = msg.deserialize(buf)) == 0) {
			connect_gate(cid, msg);
		}
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

int Gate_Client_Messager::refresh_heartbeat(int cid, MSG_111000 &msg) {
	MSG_511000 res_msg;
	res_msg.client_time = msg.client_time;
	res_msg.server_time = GATE_MANAGER->tick_time().sec();
	Block_Buffer res_buf;
	res_buf.make_message(RES_HEARTBEAT);
	res_msg.serialize(res_buf);
	res_buf.finish_message();
	GATE_MANAGER->send_to_client(cid, res_buf);
	return 0;
}

int Gate_Client_Messager::connect_gate(int cid, MSG_111001 &msg) {
	MSG_USER("client connect gate cid = %d, account:%s, session:%s", cid, msg.account.c_str(), msg.session.c_str());

	Gate_Player *player = 0;
	//校验是否重复登录
	if((player = GATE_MANAGER->find_account_gate_player(msg.account) )== 0){
		MSG_DEBUG("Repeat login check success");
		MSG_112000 login_msg;
		login_msg.account = msg.account;
		login_msg.session = msg.session;
		Block_Buffer login_buf;
		login_buf.make_message(SYNC_GATE_LOGIN_PLAYER_ACCOUNT, 0, cid);
		login_msg.serialize(login_buf);
		login_buf.finish_message();
		GATE_MANAGER->send_to_login(login_buf);
	}
	//重复登录
	else
	{
		MSG_DEBUG("Repeat login check fail");
		Block_Buffer res_buf;
		res_buf.make_message(RES_CLIENT_LOGIN, ERROR_LOGIN_VERIFY_FAIL);
		res_buf.finish_message();
		GATE_MANAGER->send_to_client(cid, res_buf);
		GATE_MANAGER->close_client(cid);
	}

	return 0;
}
