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
	int32_t gate_cid =  buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();
	int32_t player_cid = buf.read_int32();

	//客户端发到服务器的消息在这里处理
	Perf_Mon perf_mon(msg_id);
	switch(msg_id) {
	case SYNC_GATE_MASTER_PLAYER_SIGNIN:
		gate_master_player_signin(gate_cid, player_cid, buf);
		break;
	case REQ_SEND_CHAT_INFO:
		break;
	default:
		LOG_ERROR("msg_id:%d error", msg_id);
	}

	return 0;
}

int Master_Client_Messager::gate_master_player_signin(int gate_cid, int player_cid, Block_Buffer &buf) {
	MSG_140200 msg;
	msg.deserialize(buf);
	Master_Player *player = MASTER_MANAGER->find_role_id_master_player(msg.role_id);
	if (!player) {
		LOG_INFO("can't find role_id:%ld master player", msg.role_id);
		return -1;
	}

	LOG_INFO("player signin master from gate success, role_id:%ld gate_cid:%d, player_cid:%d", msg.role_id, gate_cid, player_cid);
	player->set_gate_cid(gate_cid);
	MASTER_MANAGER->bind_gate_cid_master_player(gate_cid * 10000 + player_cid, *player);
	return 0;
}
