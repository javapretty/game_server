/*
 * DB_Worker.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "DB_Server.h"
#include "DB_Worker.h"
#include "DB_Manager.h"

DB_Worker::DB_Worker(void) { }

DB_Worker::~DB_Worker(void) { }

int DB_Worker::load_player_data(int64_t role_id, Block_Buffer &buffer) {
	DB_Struct *player_def = DB_MANAGER->get_player_data_struct();
	for(std::vector<Field_Info>::iterator iter = player_def->field_vec().begin();
			iter != player_def->field_vec().end(); iter++){
		DB_Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find((*iter).field_type);
		if(it == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the module %s", (*iter).field_type.c_str());
			return -1;
		}
		DB_Struct *def = it->second;
		def->load_data(role_id, buffer);
	}
	return 0;
}

int DB_Worker::create_player_data(int64_t role_id) {
	DB_Struct *role_def = DB_MANAGER->get_player_data_struct();
	for(std::vector<Field_Info>::iterator iter = role_def->field_vec().begin();
			iter != role_def->field_vec().end(); iter++){
		DB_Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find((*iter).field_type);
		if((*iter).field_type == "Game_Player_Info")
			continue;
		if(it == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the module %s", (*iter).field_type.c_str());
			return -1;
		}
		DB_Struct *def = it->second;
		def->create_data(role_id);
	}
	return 0;
}

int DB_Worker::save_player_data(Block_Buffer &buffer) {
	DB_Struct *role_def = DB_MANAGER->get_player_data_struct();
	for(std::vector<Field_Info>::iterator iter = role_def->field_vec().begin();
			iter != role_def->field_vec().end(); iter++){
		std::string type_name = (*iter).field_type;
		DB_Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(type_name);
		if(it == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the module %s", type_name.c_str());
			return -1;
		}
		DB_Struct *def = it->second;
		def->save_data(buffer);
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
	int32_t status = buf->read_int32();

	switch (msg_id) {
	case SYNC_GAME_DB_LOAD_PLAYER: {
		MSG_150001 msg;
		msg.deserialize(*buf);
		process_load_player(cid, msg.account, msg.client_ip);
		break;
	}
	case SYNC_GAME_DB_CREATE_PLAYER: {
		MSG_150002 msg;
		msg.deserialize(*buf);
		process_create_player(cid, msg.role_info);
		break;
	}
	case SYNC_GAME_DB_SAVE_PLAYER: {
		process_save_player(cid, status, *buf);
		break;
	}
	case SYNC_MASTER_DB_CREATE_GUILD:{
		MSG_150100 msg;
		msg.deserialize(*buf);
		process_create_guild(cid, msg.guild_info);
		break;
	}
	case SYNC_MASTER_DB_LOAD_DATA:{
		process_master_load_data(cid);
		break;
	}
	case SYNC_MASTER_DB_DELETE_DATA: {
		std::string struct_name = buf->read_string();
		DB_Struct_Name_Map::iterator iter = DB_MANAGER->db_struct_name_map().find(struct_name);
		if(iter == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the struct_name: %s", struct_name.c_str());
			break;
		}
		DB_Struct *def = iter->second;
		def->delete_data(*buf);
		break;
	}
	default: {
		DB_Struct_Id_Map::iterator iter = DB_MANAGER->db_struct_id_map().find(msg_id);
		if(iter == DB_MANAGER->db_struct_id_map().end()){
			LOG_ERROR("Can not find the msg_id %d", msg_id);
			break;
		}
		DB_Struct *def = iter->second;
		def->save_data_vector(*buf);
		break;
	}
	}

	DB_SERVER->push_block(cid, buf);
	return 0;
}

int DB_Worker::process_load_player(int cid, std::string &account, std::string &client_ip) {
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_LOAD_PLAYER);
	buf.write_string(account);
	buf.write_string(client_ip);
	DB_Manager::DB_Cache_Account_Map::iterator iter = DB_MANAGER->db_cache_account_map().find(account);
	if (iter != DB_MANAGER->db_cache_account_map().end())	{
		//角色存在，开始加载数据
		buf.write_int8(SUCCESS_LOADED);
		int64_t role_id = iter->second.role_id;
		load_player_data(role_id, buf);
	} else {
		//角色不存在，直接返回
		buf.write_int8(ROLE_NOT_EXIST);
	}
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_create_player(int cid, Create_Role_Info &role_info) {
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_GAME_CREATE_PLAYER);
	int32_t status = 0;
	int64_t role_id = 0;
	if ((role_id = DB_MANAGER->create_player(role_info)) < 0) {
		status = ROLE_HAS_EXIST;
	} else {
		status = SUCCESS_CREATED;
		//创建所有玩家表
		create_player_data(role_id);
	}
	buf.write_string(role_info.account);
	buf.write_string(role_info.client_ip);
	buf.write_int8(status);
	load_player_data(role_id, buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_save_player(int cid, int status, Block_Buffer &buffer) {
	if (status == 1) {
		//离线保存
		int rdx = buffer.get_read_idx();
		int64_t role_id = buffer.read_int64();
		buffer.set_read_idx(rdx);

		save_player_data(buffer);

		Block_Buffer buf;
		buf.make_inner_message(SYNC_DB_GAME_SAVE_PLAYER);
		buf.write_int64(role_id);
		buf.finish_message();
		DB_MANAGER->send_data_block(cid, buf);
	} else {
		//在线定时保存
		save_player_data(buffer);
	}
	return 0;
}

int DB_Worker::process_create_guild(int cid, Create_Guild_Info &guild_info) {
	Block_Buffer buf;
	buf.make_inner_message(SYNC_DB_MASTER_CREATE_GUILD);
	guild_info.guild_id = DB_MANAGER->create_guild(guild_info);
	guild_info.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_master_load_data(int cid) {
	DB_Struct *master_data = DB_MANAGER->get_master_data_struct();
	for(std::vector<Field_Info>::iterator iter = master_data->field_vec().begin();
				iter != master_data->field_vec().end(); iter++){
		std::string type_name = (*iter).field_type;
		DB_Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(type_name);
		if(it == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the module %s", type_name.c_str());
			return -1;
		}

		DB_Struct *def = it->second;
		Block_Buffer buf;
		buf.make_inner_message(def->msg_id() + 400000);
		def->load_data(0, buf);
		buf.finish_message();
		DB_MANAGER->send_data_block(cid, buf);
	}

	return 0;
}
