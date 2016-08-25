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

	Perf_Mon perf_mon(msg_id);
	GATE_MANAGER->inner_msg_count(msg_id);

	if (msg_id == SYNC_LOGIN_GATE_PLAYER_ACCOUNT) {
		MSG_140001 msg;
		msg.deserialize(buf);
		Block_Buffer player_buf;
		player_buf.make_server_message(RES_CONNECT_GATE, status);
		MSG_500101 player_msg;
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

	Perf_Mon perf_mon(msg_id);
	GATE_MANAGER->inner_msg_count(msg_id);

	//玩家登录game成功，更新gate玩家信息，同步消息到master
	if (msg_id == 520001 && status == 0) {
		//RES_FETCH_ROLE_INFO消息第一个字段是64位role_id
		int read_idx = buf.get_read_idx();
		int64_t role_id = 0;
		buf.read_int64(role_id);
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
	player_buf.make_server_message(msg_id, status);
	player_buf.copy(&buf);
	player_buf.finish_message();
	GATE_MANAGER->send_to_client(player_cid, player_buf);
	return 0;
}

int Gate_Inner_Messager::process_master_block(Block_Buffer &buf) {
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

	Perf_Mon perf_mon(msg_id);
	GATE_MANAGER->inner_msg_count(msg_id);

	if(msg_id == 510300) { //切换场景消息
		int32_t game_server_id = 0;
		buf.read_int32(game_server_id);
		Gate_Player *player = GATE_MANAGER->find_cid_gate_player(player_cid);
		if(player == NULL)
			return -1;
		int game_cid = GATE_MANAGER->get_game_cid(game_server_id);
		player->set_game_cid(game_cid);
		Block_Buffer buffer;
		buffer.make_player_message(120001, 0, player_cid);
		buffer.write_string(player->get_account());
		buffer.finish_message();
		GATE_MANAGER->send_to_game(game_cid, buffer);
		return 0;
	}

	Block_Buffer player_buf;
	player_buf.make_server_message(msg_id, status);
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
	player->set_game_cid(GATE_MANAGER->get_lowest_player_game_cid());
	GATE_MANAGER->bind_cid_gate_player(player_cid, *player);
	GATE_MANAGER->bind_account_gate_player(account, *player);

	return 0;
}
