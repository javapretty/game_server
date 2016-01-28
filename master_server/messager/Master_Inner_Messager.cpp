/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "Master_Manager.h"
#include "Log.h"
#include "Configurator.h"
#include "Master_Player.h"
#include "Master_Inner_Messager.h"

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
	/*uint16_t len*/ buf.read_uint16();
	uint32_t msg_id = buf.read_uint32();
	/*int32_t status*/ buf.read_int32();
	int32_t player_cid = buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_GAME_MASTER_PLYAER_SIGNIN: {
		process_160001(game_cid, player_cid, buf);
		break;
	}
	case SYNC_GAME_MASTER_PLAYER_SIGNOUT: {
		process_160002(buf);
		break;
	}
	default:
		break;
	}

	return 0;
}

int Master_Inner_Messager::process_self_loop_block(Block_Buffer &buf) {
	/*uint16_t len*/ buf.read_uint16();
	uint32_t msg_id = buf.read_uint32();
	/*int32_t status*/ buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_INNER_TIMER_TICK: {
		MASTER_MANAGER->tick();
		break;
	}
	case SYNC_INNER_CONFIG_HOTUPDATE: {
		process_400001(buf);
		break;
	}
	default:
		break;
	}

	return 0;
}

int Master_Inner_Messager::process_160001(int game_cid, int player_cid, Block_Buffer &buf) {
	MSG_160001 msg;
	msg.deserialize(buf);
	Master_Player *player = MASTER_MANAGER->pop_master_player();
	if (! player) {
		MSG_USER("master_player_pool_.pop() return 0.");
		return -1;
	}

	player->reset();
	Cid_Info cid_info(0, game_cid, player_cid);
	player->set_cid_info(cid_info);
	MASTER_MANAGER->bind_role_id_master_player(msg.player_info.role_id, *player);
	player->sign_in(msg.player_info);
	return 0;
}

int Master_Inner_Messager::process_160002(Block_Buffer &buf) {
	MSG_160002 msg;
	msg.deserialize(buf);
	Master_Player *player = MASTER_MANAGER->find_role_id_master_player(msg.role_id);
	if (!player) {
		MSG_USER("process_160002 can't find role_id = %ld", msg.role_id);
	}
	MASTER_MANAGER->unbind_master_player(*player);
	player->sign_out();
	MASTER_MANAGER->push_master_player(player);

	MSG_USER("process_160002 role sign out role_id = %ld", msg.role_id);
	return 0;
}

int Master_Inner_Messager::process_400001(Block_Buffer &buf) {
	MSG_400001 msg;
	msg.deserialize(buf);
	CONFIG_INSTANCE->hot_update_conf(msg.module, 1);
	return 0;
}
