/*
 * DB_Worker.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "DB_Server.h"
#include "DB_Worker.h"
#include "DB_Manager.h"
#include "DB_Operator.h"

DB_Worker::DB_Worker(void) { }

DB_Worker::~DB_Worker(void) { }

void DB_Worker::run_handler(void) {
	process_list();
}

void DB_Worker::process_list(void) {
	Block_Buffer *buf = 0;

	while (1) {
		buf = 0;
		bool all_empty = true;

		if ((buf = load_player_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buf);
		}
		if ((buf = create_player_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buf);
		}
		if ((buf = save_player_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buf);
		}
		if ((buf = data_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buf);
		}

		if (all_empty)
			Time_Value::sleep(SLEEP_TIME);
	}
}

int DB_Worker::process_data_block(Block_Buffer *buf) {
	if (! buf || !buf->is_legal()) {
		MSG_USER("buf error, buf.read_index = %ld, buf.write_index = %ld",
						buf->get_read_idx(), buf->get_write_idx());
		return -1;
	}

	int32_t cid = 0;
	uint16_t len = 0;
	uint32_t msg_id = 0;
	int32_t  status = 0;

	buf->read_int32(cid);
	buf->read_uint16(len);
	buf->read_uint32(msg_id);
	buf->read_int32(status);

	switch (msg_id) {
	case SYNC_GAME_DB_LOAD_DB_CACHE: {
		process_load_db_cache(cid);
		break;
	}
	case SYNC_GAME_DB_LOAD_PLAYER_INFO: {
		MSG_150001 msg;
		msg.deserialize(*buf);
		process_load_player(cid, msg.account_info);
		break;
	}
	case SYNC_GAME_DB_CREATE_PLAYER: {
		MSG_150002 msg;
		msg.deserialize(*buf);
		process_create_player(cid, msg.player_info);
		break;
	}
	case SYNC_GAME_DB_SAVE_PLAYER_INFO: {
		MSG_150003 msg;
		msg.deserialize(*buf);
		process_save_player(cid, msg.player_data);
		break;
	}
	default: {
		MSG_USER("unknown msg_id = %d.", msg_id);
		break;
	}
	}

	DB_SERVER->push_block(cid, buf);
	return 0;
}

/// 加载db_cache
int DB_Worker::process_load_db_cache(int cid) {
	CACHED_INSTANCE->load_db_cache(cid);
	return 0;
}

int DB_Worker::process_load_player(int cid, Account_Info &account_info) {
	MSG_550001 msg;
	int has_role = CACHED_INSTANCE->has_role_by_account(account_info.account, account_info.agent_num, account_info.server_num);
	if (! has_role) {
		msg.player_data.status = Player_Data::ROLE_NOT_EXIST;
		msg.player_data.game_player_info.account = account_info.account;
		msg.player_data.game_player_info.agent_num = account_info.agent_num;
		msg.player_data.game_player_info.server_num = account_info.server_num;
	}	else {
		msg.player_data.status = Player_Data::SUCCESS_LOADED;
		msg.player_data.role_id = CACHED_INSTANCE->get_role_id(account_info.account, account_info.agent_num, account_info.server_num);
		msg.player_data.load();
	}
	Block_Buffer buf;
	buf.make_message(SYNC_DB_GAME_LOAD_PLAYER_INFO);
	msg.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_create_player(int cid, Game_Player_Info &player_info) {
	MSG_550002 msg;
	if (CACHED_INSTANCE->create_init_player(player_info) < 0) {
		msg.player_data.status = Player_Data::ROLE_HAS_EXIST;
	} else {
		msg.player_data.status = Player_Data::SUCCESS_CREATED;
		//此处保存所有数据是为了创建所有玩家表
		msg.player_data.set_all_role_id(player_info.role_id);
		msg.player_data.set_all_detail_change_state(true);
		msg.player_data.save();
		msg.player_data.set_all_detail_change_state(false);
	}
	msg.player_data.game_player_info = player_info;
	Block_Buffer buf;
	buf.make_message(SYNC_DB_GAME_CREATE_PLAYER);
	msg.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_save_player(int cid, Player_Data &player_data) {
	if (player_data.status == Player_Data::ROLE_SAVE_OFFLINE) {
		player_data.set_all_detail_change_state(true);
		player_data.save();

		Block_Buffer buf;
		buf.make_message(SYNC_DB_GAME_SAVE_PLAYER_INFO);
		MSG_550003 msg;
		msg.role_id = player_data.game_player_info.role_id;
		msg.serialize(buf);
		buf.finish_message();
		DB_MANAGER->send_data_block(cid, buf);
	} else {
		player_data.save();
	}
	return 0;
}
