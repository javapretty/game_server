/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Master_Inner_Messager.h"
#include "Master_Manager.h"
#include "Server_Config.h"

Master_Inner_Messager::Master_Inner_Messager(void) { }

Master_Inner_Messager::~Master_Inner_Messager(void) { }

Master_Inner_Messager *Master_Inner_Messager::instance_;

Master_Inner_Messager *Master_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Master_Inner_Messager;
	return instance_;
}

int Master_Inner_Messager::process_game_block(Block_Buffer &buf) {
	int32_t game_cid = buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();
	int32_t player_cid = buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_GAME_MASTER_PLYAER_SIGNIN: {
		game_master_player_signin(game_cid, player_cid, buf);
		break;
	}
	case SYNC_GAME_MASTER_PLAYER_SIGNOUT: {
		game_master_player_signout(buf);
		break;
	}
	default:
		break;
	}

	return 0;
}

int Master_Inner_Messager::process_self_loop_block(Block_Buffer &buf) {
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_INNER_TIMER_TICK: {
		MASTER_MANAGER->tick();
		break;
	}
	default:
		break;
	}

	return 0;
}

int Master_Inner_Messager::game_master_player_signin(int game_cid, int player_cid, Block_Buffer &buf) {
	MSG_160000 msg;
	msg.deserialize(buf);
	Master_Player *player = MASTER_MANAGER->pop_master_player();
	if (! player) {
		LOG_INFO("master_player_pool_.pop() return 0.");
		return -1;
	}

	player->reset();
	player->set_game_cid(game_cid);
	player->set_player_cid(player_cid);
	player->sign_in(msg.player_info);
	MASTER_MANAGER->bind_game_cid_master_player(game_cid * 10000 + player_cid, *player);
	MASTER_MANAGER->bind_role_id_master_player(msg.player_info.role_id, *player);
	return 0;
}

int Master_Inner_Messager::game_master_player_signout(Block_Buffer &buf) {
	MSG_160001 msg;
	msg.deserialize(buf);
	Master_Player *player = MASTER_MANAGER->find_role_id_master_player(msg.role_id);
	if (!player) {
		LOG_INFO("can't find role_id:%ld master player", msg.role_id);
		return -1;
	}
	MASTER_MANAGER->unbind_master_player(*player);
	player->sign_out();
	MASTER_MANAGER->push_master_player(player);
	return 0;
}
