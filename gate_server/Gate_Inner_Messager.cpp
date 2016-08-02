/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Gate_Inner_Messager.h"
#include "Gate_Manager.h"

Gate_Inner_Messager::Gate_Inner_Messager(void) { }

Gate_Inner_Messager::~Gate_Inner_Messager(void) { }

Gate_Inner_Messager *Gate_Inner_Messager::instance_;

Gate_Inner_Messager *Gate_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Gate_Inner_Messager;
	return instance_;
}

int Gate_Inner_Messager::process_login_block(Block_Buffer &buf) {
	/*int32_t gate_cid*/ buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	int32_t status = buf.read_int32();
	int32_t player_cid = buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	GATE_MANAGER->inner_msg_count(msg_id);

	if (msg_id == SYNC_LOGIN_GATE_PLAYER_ACCOUNT) {
		MSG_140001 msg;
		msg.deserialize(buf);
		Block_Buffer player_buf;
		player_buf.make_inner_message(RES_CONNECT_GATE, status);
		MSG_500002 player_msg;
		player_msg.account = msg.account;
		player_msg.serialize(player_buf);
		player_buf.finish_message();
		GATE_MANAGER->send_to_client(player_cid, player_buf);

		if (status == 0) {
			//玩家成功登录
			process_success_login(player_cid, msg.account);
		}
		else
		{
			GATE_MANAGER->close_client(player_cid);
		}
	}
	return 0;
}

int Gate_Inner_Messager::process_game_block(Block_Buffer &buf) {
	/*int32_t gate_cid*/ buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	int32_t status = buf.read_int32();
	int32_t player_cid = buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	GATE_MANAGER->inner_msg_count(msg_id);

	//玩家登录game成功，更新gate玩家信息，同步消息到master
	if (msg_id == RES_FETCH_ROLE_INFO && status == 0) {
		//RES_FETCH_ROLE_INFO消息第一个字段是64位role_id
		int read_idx = buf.get_read_idx();
		int64_t role_id = buf.read_int64();
		buf.set_read_idx(read_idx);

		Block_Buffer master_buf;
		master_buf.make_player_message(SYNC_GATE_MASTER_PLAYER_LOGIN, 0, player_cid);
		master_buf.write_int64(role_id);
		master_buf.finish_message();
		GATE_MANAGER->send_to_master(master_buf);
	} else if (msg_id == ACTIVE_DISCONNECT) {
		return GATE_MANAGER->close_client(player_cid);
	}

	Block_Buffer player_buf;
	player_buf.make_inner_message(msg_id, status);
	player_buf.copy(&buf);
	player_buf.finish_message();
	GATE_MANAGER->send_to_client(player_cid, player_buf);
	return 0;
}

int Gate_Inner_Messager::process_master_block(Block_Buffer &buf) {
	/*int32_t gate_cid*/ buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	int32_t status = buf.read_int32();
	int32_t player_cid = buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	GATE_MANAGER->inner_msg_count(msg_id);

	Block_Buffer player_buf;
	player_buf.make_inner_message(msg_id, status);
	player_buf.copy(&buf);
	player_buf.finish_message();
	GATE_MANAGER->send_to_client(player_cid, player_buf);

	return 0;
}

int Gate_Inner_Messager::process_success_login(int player_cid, std::string &account) {
	Gate_Player *player = GATE_MANAGER->pop_gate_player();
	if (! player) {
		LOG_ERROR("game_player_pool_ pop error");
		return -1;
	}

	player->reset();
	player->set_player_cid(player_cid);
	player->set_account(account);
	GATE_MANAGER->bind_cid_gate_player(player_cid, *player);
	GATE_MANAGER->bind_account_gate_player(account, *player);

	return 0;
}
