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

int DB_Worker::load_player_data(int64_t role_id, Block_Buffer &buffer) {
	DB_Definition *player_def = DB_MANAGER->get_player_data_definition();
	for(std::vector<DB_Type_Description>::iterator iter = player_def->type_vec().begin();
			iter != player_def->type_vec().end(); iter++){
		DB_Manager::DB_Name_Definition_Map::iterator it = DB_MANAGER->db_name_definition_map().find((*iter).type_name);
		if(it == DB_MANAGER->db_name_definition_map().end()){
			LOG_ERROR("Can not find the module %s", (*iter).type_name.c_str());
			return -1;
		}
		DB_Definition *def = it->second;
		def->load_data(role_id, buffer);
	}
	return 0;
}

int DB_Worker::save_player_data(Block_Buffer &buffer) {
	DB_Definition *role_def = DB_MANAGER->get_player_data_definition();
	for(std::vector<DB_Type_Description>::iterator iter = role_def->type_vec().begin();
			iter != role_def->type_vec().end(); iter++){
		std::string type_name = (*iter).type_name;
		DB_Manager::DB_Name_Definition_Map::iterator it = DB_MANAGER->db_name_definition_map().find(type_name);
		if(it == DB_MANAGER->db_name_definition_map().end()){
			LOG_ERROR("Can not find the module %s", type_name.c_str());
			return -1;
		}
		DB_Definition *def = it->second;
		def->save_data(buffer);
	}
	return 0;
}

int DB_Worker::init_player_table(int64_t role_id) {
	DB_Definition *role_def = DB_MANAGER->get_player_data_definition();
	for(std::vector<DB_Type_Description>::iterator iter = role_def->type_vec().begin();
			iter != role_def->type_vec().end(); iter++){
		DB_Manager::DB_Name_Definition_Map::iterator it = DB_MANAGER->db_name_definition_map().find((*iter).type_name);
		if((*iter).type_name == "Game_Player_Info")
			continue;
		if(it == DB_MANAGER->db_name_definition_map().end()){
			LOG_ERROR("Can not find the module %s", (*iter).type_name.c_str());
			return -1;
		}
		DB_Definition *def = it->second;
		def->init_table(role_id);
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
		process_save_player(cid, *buf);
		break;
	}
	case SYNC_GAME_DB_SAVE_MAIL_INFO: {
		MSG_150004 msg;
		msg.deserialize(*buf);
		//process_save_mail(msg);
		break;
	}
	case SYNC_MASTER_DB_LOAD_PUBLIC_DATA:{
		MSG_150101 msg;
		msg.deserialize(*buf);
		process_load_public_data(cid, *buf);
		break;
	}
	case SYNC_MASTER_DB_DELETE_DATA:
	case SYNC_GAME_DB_DELETE_DATA: {
		std::string msg_type = buf->read_string();
		DB_Manager::DB_Name_Definition_Map::iterator iter = DB_MANAGER->db_name_definition_map().find(msg_type);
		if(iter == DB_MANAGER->db_name_definition_map().end()){
			LOG_ERROR("Can not find the module %s", msg_type.c_str());
			break;
		}
		DB_Definition *def = iter->second;
		def->delete_data(*buf);
		break;
	}
	default: {
		DB_Manager::DB_Id_Definition_Map::iterator iter = DB_MANAGER->db_id_definition_map().find(msg_id);
		if(iter == DB_MANAGER->db_id_definition_map().end()){
			LOG_ERROR("Can not find the msg_id %d", msg_id);
			break;
		}
		DB_Definition *def = iter->second;
		def->save_data_vector(*buf);
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
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_LOAD_PLAYER_INFO);
	Game_Player_Info player_info;
	int has_role = CACHED_INSTANCE->has_role_by_account(account_info.account, account_info.agent_num, account_info.server_num);
	if (! has_role) {
		buf.write_int32(ROLE_NOT_EXIST);
		player_info.account = account_info.account;
		player_info.agent_num = account_info.agent_num;
		player_info.server_num = account_info.server_num;
	}	else {
		buf.write_int32(SUCCESS_LOADED);
		int64_t role_id = CACHED_INSTANCE->get_role_id(account_info.account, account_info.agent_num, account_info.server_num);
		load_player_data(role_id, buf);
	}

	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_create_player(int cid, Game_Player_Info &player_info) {
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_CREATE_PLAYER);
	int32_t status;
	if (CACHED_INSTANCE->create_init_player(player_info) < 0) {
		status = ROLE_HAS_EXIST;
	} else {
		status = SUCCESS_CREATED;
		//创建所有玩家表
		init_player_table(player_info.role_id);
	}
	buf.write_int32(status);
	load_player_data(player_info.role_id, buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_save_player(int cid, Block_Buffer &buffer) {
	int32_t status = buffer.read_int32();
	if (status == 1) {
		int rdx = buffer.get_read_idx();
		buffer.read_int32();
		int64_t role_id = buffer.read_int64();
		buffer.set_read_idx(rdx);

		save_player_data(buffer);

		Block_Buffer buf;
		buf.make_inner_message(SYNC_DB_GAME_SAVE_PLAYER_INFO);
		MSG_550003 msg;
		msg.role_id = role_id;
		msg.serialize(buf);
		buf.finish_message();
		DB_MANAGER->send_data_block(cid, buf);
	} else {
		save_player_data(buffer);
	}
	return 0;
}

int DB_Worker::process_load_public_data(int cid, Block_Buffer &buffer) {
	DB_Definition *public_data = DB_MANAGER->get_public_data_definition();

	for(std::vector<DB_Type_Description>::iterator iter = public_data->type_vec().begin();
				iter != public_data->type_vec().end(); iter++){
		std::string type_name = (*iter).type_name;
		DB_Manager::DB_Name_Definition_Map::iterator it = DB_MANAGER->db_name_definition_map().find(type_name);
		if(it == DB_MANAGER->db_name_definition_map().end()){
			LOG_ERROR("Can not find the module %s", type_name.c_str());
			return -1;
		}

		DB_Definition *def = it->second;
		Block_Buffer buf;
		buf.make_inner_message(def->cmdid() + 400000);
		def->load_data(0, buf);
		buf.finish_message();
		DB_MANAGER->send_data_block(cid, buf);
	}

	return 0;
}
