/*
 * DB_Worker.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "DB_Server.h"
#include "DB_Worker.h"
#include "DB_Manager.h"
#include "DB_Operator.h"

DB_Worker::DB_Worker(void) { }

DB_Worker::~DB_Worker(void) { }

int DB_Worker::load_player_data(int64_t role_id, Player_Data &player_data) {
	CACHED_INSTANCE->load_player_info(role_id, player_data.player_info);
	CACHED_INSTANCE->load_hero_info(role_id, player_data.hero_info);
	CACHED_INSTANCE->load_bag_info(role_id, player_data.bag_info);
	CACHED_INSTANCE->load_mail_info(role_id, player_data.mail_info);
	CACHED_INSTANCE->load_shop_info(role_id, player_data.shop_info);
	return 0;
}

int DB_Worker::save_player_data(Player_Data &player_data) {
	for (std::vector<int>::iterator iter = player_data.change_module.begin(); iter != player_data.change_module.end(); ++iter) {
		switch(*iter) {
		case PLAYER_CHANGE:
			CACHED_INSTANCE->save_player_info(player_data.player_info.role_id, player_data.player_info);
			break;
		case HERO_CHANGE:
			CACHED_INSTANCE->save_hero_info(player_data.player_info.role_id, player_data.hero_info);
			break;
		case BAG_CHANGE:
			CACHED_INSTANCE->save_bag_info(player_data.player_info.role_id, player_data.bag_info);
			break;
		case MAIL_CHANGE:
			CACHED_INSTANCE->save_mail_info(player_data.player_info.role_id, player_data.mail_info);
			break;
		case SHOP_CHANGE:
			CACHED_INSTANCE->save_shop_info(player_data.player_info.role_id, player_data.shop_info);
			break;
		}
	}
	return 0;
}

int DB_Worker::set_player_data_change(bool change, Player_Data &player_data) {
	if (change) {
		for (int i = PLAYER_CHANGE; i < CHANGE_END; ++i) {
			player_data.change_module.push_back(i);
		}
	} else {
		player_data.change_module.clear();
	}
	return 0;
}

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
			Time_Value::sleep(Time_Value(0,100));
	}
}

int DB_Worker::process_data_block(Block_Buffer *buf) {
	if (! buf || !buf->is_legal()) {
		LOG_ERROR("buf error, buf.read_index = %ld, buf.write_index = %ld", buf->get_read_idx(), buf->get_write_idx());
		return -1;
	}

	int32_t cid = buf->read_int32();
	/*int16_t len*/ buf->read_int16();
	int32_t msg_id = buf->read_int32();
	/*int32_t status*/ buf->read_int32();

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
	case SYNC_GAME_DB_SAVE_MAIL_INFO: {
		MSG_150004 msg;
		msg.deserialize(*buf);
		process_save_mail(msg);
		break;
	}
	case SYNC_MASTER_DB_LOAD_PUBLIC_INFO: {
		MSG_150101 msg;
		msg.deserialize(*buf);
		process_load_public_info(cid, msg);
		break;
	}
	case SYNC_MASTER_DB_SAVE_GUILD_INFO: {
		MSG_150102 msg;
		msg.deserialize(*buf);
		process_save_guild(msg);
		break;
	}
	case SYNC_MASTER_DB_DROP_GUILD_INFO: {
		MSG_150103 msg;
		msg.deserialize(*buf);
		process_drop_guild(msg);
		break;
	}
	case SYNC_MASTER_DB_SAVE_OFFLINE_MSG: {
		MSG_150104 msg;
		msg.deserialize(*buf);
		process_save_offline_msg(msg);
		break;
	}
	case SYNC_MASTER_DB_DROP_OFFLINE_MSG: {
		MSG_150105 msg;
		msg.deserialize(*buf);
		process_drop_offline_msg(msg);
		break;
	}
	default: {
		LOG_ERROR("msg_id = %d error", msg_id);
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
		msg.player_data.status = ROLE_NOT_EXIST;
		msg.player_data.player_info.account = account_info.account;
		msg.player_data.player_info.agent_num = account_info.agent_num;
		msg.player_data.player_info.server_num = account_info.server_num;
	}	else {
		msg.player_data.status = SUCCESS_LOADED;
		int64_t role_id = CACHED_INSTANCE->get_role_id(account_info.account, account_info.agent_num, account_info.server_num);
		load_player_data(role_id, msg.player_data);
	}
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_LOAD_PLAYER_INFO);
	msg.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_create_player(int cid, Game_Player_Info &player_info) {
	MSG_550002 msg;
	if (CACHED_INSTANCE->create_init_player(player_info) < 0) {
		msg.player_data.status = ROLE_HAS_EXIST;
	} else {
		msg.player_data.status = SUCCESS_CREATED;
		//此处保存所有数据是为了创建所有玩家表
		set_player_data_change(true, msg.player_data);
		save_player_data(msg.player_data);
		set_player_data_change(false, msg.player_data);
	}
	msg.player_data.player_info = player_info;
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_CREATE_PLAYER);
	msg.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_save_player(int cid, Player_Data &player_data) {
	if (player_data.status == ROLE_SAVE_OFFLINE) {
		set_player_data_change(true, player_data);
		save_player_data(player_data);

		Block_Buffer buf;
		buf.make_inner_message(SYNC_DB_GAME_SAVE_PLAYER_INFO);
		MSG_550003 msg;
		msg.role_id = player_data.player_info.role_id;
		msg.serialize(buf);
		buf.finish_message();
		DB_MANAGER->send_data_block(cid, buf);
	} else {
		save_player_data(player_data);
	}
	return 0;
}

int DB_Worker::process_save_mail(MSG_150004 &msg) {
	Mail_Info mail_info;
	CACHED_INSTANCE->load_mail_info(msg.role_id, mail_info);
	mail_info.total_count++;
	msg.mail_detail.mail_id = 1000000 + mail_info.total_count;
	msg.mail_detail.send_time = Time_Value::gettimeofday().sec();
	mail_info.mail_map.insert(std::make_pair(msg.mail_detail.mail_id, msg.mail_detail));

	if (mail_info.mail_map.size() > 100) {
		for (std::map<int,Mail_Detail>::iterator iter = mail_info.mail_map.begin();
				iter != mail_info.mail_map.end(); ++iter) {
			mail_info.mail_map.erase(iter);
			break;
		}
	}
	CACHED_INSTANCE->save_mail_info(msg.role_id, mail_info);
	return 0;
}

int DB_Worker::process_load_public_info(int cid, MSG_150101 &msg){
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_MASTER_LOAD_PUBLIC_INFO);
	MSG_550101 res;
	
	CACHED_INSTANCE->load_guild_info(res.guild_info);
	CACHED_INSTANCE->load_offline_msg(res.offline_msg);
	
	res.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
};

int DB_Worker::process_save_guild(MSG_150102 &msg) {
	CACHED_INSTANCE->save_guild_info(msg.guild_info);
	return 0;
}

int DB_Worker::process_drop_guild(MSG_150103 &msg) {
	CACHED_INSTANCE->drop_guild_info(msg.guild_list);
	return 0;
}

int DB_Worker::process_save_offline_msg(MSG_150104 &msg) {
	CACHED_INSTANCE->save_offline_msg(msg.offline_msg);
	return 0;
}

int DB_Worker::process_drop_offline_msg(MSG_150105 &msg) {
	CACHED_INSTANCE->drop_offline_msg(msg.offline_msg_list);
	return 0;
}
