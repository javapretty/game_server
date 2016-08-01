/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Game_Inner_Messager.h"
#include "Game_Manager.h"

Game_Inner_Messager::Game_Inner_Messager(void) { }

Game_Inner_Messager::~Game_Inner_Messager(void) { }

Game_Inner_Messager *Game_Inner_Messager::instance_;

Game_Inner_Messager *Game_Inner_Messager::instance(void) {
	if (! instance_)
		instance_ = new Game_Inner_Messager;
	return instance_;
}

int Game_Inner_Messager::process_self_loop_block(Block_Buffer &buf) {
	/*int16_t len*/ buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case SYNC_INNER_TIMER_TICK: {
		GAME_MANAGER->tick();
		break;
	}
	default:
		break;
	}

	return 0;
}
