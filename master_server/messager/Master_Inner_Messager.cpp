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
