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
	GATE_MANAGER->add_recv_bytes(buf.readable_bytes());

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

	GATE_MANAGER->add_msg_count(msg_id);
	if (msg_id == SYNC_LOGIN_GATE_PLAYER_ACCOUNT) {
		if (status == 0) {
			//玩家成功登录
			MSG_140001 msg;
			msg.deserialize(buf);
			Block_Buffer player_buf;
			player_buf.make_server_message(RES_CONNECT_GATE, status);
			MSG_500101 player_msg;
			player_msg.account = msg.account;
			player_msg.serialize(player_buf);
			player_buf.finish_message();
			GATE_MANAGER->send_to_client(player_cid, player_buf);

			process_success_login(player_cid, msg.account);
		} else {
			GATE_MANAGER->close_client(0, player_cid, status);
		}
	}
	return 0;
}

int Gate_Inner_Messager::process_game_block(Block_Buffer &buf) {
	GATE_MANAGER->add_recv_bytes(buf.readable_bytes());

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

	GATE_MANAGER->add_msg_count(msg_id);
	//玩家登录game成功，更新gate玩家信息，同步消息到master
	if (msg_id == RES_FETCH_ROLE_INFO && status == 0) {
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
		return GATE_MANAGER->close_client(0, player_cid, status);
	}

	Block_Buffer player_buf;
	player_buf.make_server_message(msg_id, status);
	player_buf.copy(&buf);
	player_buf.finish_message();
	GATE_MANAGER->send_to_client(player_cid, player_buf);
	return 0;
}

int Gate_Inner_Messager::process_master_block(Block_Buffer &buf) {
	GATE_MANAGER->add_recv_bytes(buf.readable_bytes());

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

	GATE_MANAGER->add_msg_count(msg_id);
	//切换场景
	if(msg_id == SYNC_MASTER_GATE_PLAYER_CHANGE_SCENE) {
		int32_t game_server_id = 0;
		buf.read_int32(game_server_id);
		Gate_Player *player = dynamic_cast<Gate_Player*>(GATE_MANAGER->find_cid_player(player_cid));
		if(!player)
			return -1;
		int game_cid = GATE_MANAGER->get_game_cid(game_server_id);
		player->set_game_cid(game_cid);
		Block_Buffer buffer;
		buffer.make_player_message(REQ_FETCH_ROLE_INFO, 0, player_cid);
		buffer.write_string(player->account());
		buffer.finish_message();
		GATE_MANAGER->send_to_game(game_cid, buffer);
		return 0;
	} else if (msg_id == ACTIVE_DISCONNECT) {
		return GATE_MANAGER->close_client(0, player_cid, status);
	}

	Block_Buffer player_buf;
	player_buf.make_server_message(msg_id, status);
	player_buf.copy(&buf);
	player_buf.finish_message();
	GATE_MANAGER->send_to_client(player_cid, player_buf);

	return 0;
}

int Gate_Inner_Messager::process_success_login(int player_cid, std::string &account) {
	Gate_Player *player = GATE_MANAGER->pop_player();
	if (! player) {
		LOG_ERROR("player_pool_ pop error");
		return -1;
	}

	player->reset();
	player->set_player_cid(player_cid);
	player->set_account(account);
	player->set_game_cid(GATE_MANAGER->get_lowest_player_game_cid());
	GATE_MANAGER->bind_cid_player(player_cid, player);
	GATE_MANAGER->bind_account_player(account, player);

	return 0;
}
