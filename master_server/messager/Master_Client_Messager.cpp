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

	if (msg_id == SYNC_GATE_MASTER_PLAYER_SIGNIN) {
		return gate_master_player_signin(gate_cid, player_cid, buf);
	}

	Master_Player *player = 0;
	if ((player = MASTER_MANAGER->find_gate_cid_master_player(gate_cid * 10000 + player_cid)) == 0) {
		LOG_DEBUG("cannot find gate_cid = %d, player_cid = %d msg_id = %d ", gate_cid, player_cid, msg_id);
		return MASTER_MANAGER->close_client(gate_cid, player_cid, ERROR_DISCONNECT_MASTER);
	}

	//客户端发到服务器的消息在这里处理
	Perf_Mon perf_mon(msg_id);
	switch(msg_id) {
	case REQ_SEND_CHAT_INFO:
		player->send_chat_info(buf);
		break;
	default:
		LOG_ERROR("msg_id:%d error", msg_id);
	}

	return 0;
}

int Master_Client_Messager::gate_master_player_signin(int gate_cid, int player_cid, Block_Buffer &buf) {
	MSG_140200 msg;
	msg.deserialize(buf);
	Master_Player *master_player = MASTER_MANAGER->find_role_id_master_player(msg.player_info.role_id);
	if (master_player) {
		LOG_INFO("master player sign in exist, gate_cid:%d, playe_cid:%d, role_id:%ld, role_name:%s",
				gate_cid, player_cid, msg.player_info.role_id, msg.player_info.role_name.c_str());

		master_player->set_gate_cid(gate_cid);
		master_player->set_player_cid(player_cid);
		master_player->load_player(msg.player_info);
		master_player->sign_in();
		MASTER_MANAGER->bind_gate_cid_master_player(gate_cid * 10000 + player_cid, *master_player);
		MASTER_MANAGER->bind_role_name_master_player(msg.player_info.role_name, *master_player);
	} else {
		Master_Player *player = MASTER_MANAGER->pop_master_player();
		if (! player) {
			LOG_ERROR("master_player_pool_ pop error");
			return -1;
		}

		player->reset();
		player->set_gate_cid(gate_cid);
		player->set_player_cid(player_cid);
		player->load_player(msg.player_info);
		player->sign_in();
		MASTER_MANAGER->bind_role_id_master_player(msg.player_info.role_id, *player);
		MASTER_MANAGER->bind_gate_cid_master_player(gate_cid * 10000 + player_cid, *player);
		MASTER_MANAGER->bind_role_name_master_player(msg.player_info.role_name, *player);
	}
	return 0;
}
