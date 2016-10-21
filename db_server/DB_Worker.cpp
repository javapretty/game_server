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

void DB_Worker::run_handler(void) {
	process_list();
}

void DB_Worker::process_list(void) {
	Block_Buffer *buffer = 0;

	while (1) {
		buffer = 0;
		bool all_empty = true;

		if ((buffer = load_player_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buffer);
		}
		if ((buffer = create_player_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buffer);
		}
		if ((buffer = save_player_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buffer);
		}
		if ((buffer = data_list_.pop_front()) != 0) {
			all_empty = false;
			process_data_block(buffer);
		}

		if (all_empty)
			Time_Value::sleep(Time_Value(0,100));
	}
}

int DB_Worker::process_data_block(Block_Buffer *buffer) {
	if (! buffer || !buffer->is_legal()) {
		LOG_ERROR("buffer is not legal, buffer.read_index = %ld, buffer.write_index = %ld",
				buffer->get_read_idx(), buffer->get_write_idx());
		return -1;
	}

	int32_t cid = 0;
	int16_t len = 0;
	int32_t msg_id = 0;
	int32_t status = 0;
	buffer->read_int32(cid);
	buffer->read_int16(len);
	buffer->read_int32(msg_id);
	buffer->read_int32(status);

	switch (msg_id) {
	case SYNC_GAME_DB_LOAD_PLAYER: {
		std::string account;
		buffer->read_string(account);
		process_load_player(cid, account);
		break;
	}
	case SYNC_GAME_DB_CREATE_PLAYER: {
		Create_Role_Info role_info;
		role_info.deserialize(*buffer);
		process_create_player(cid, role_info);
		break;
	}
	case SYNC_GAME_DB_SAVE_PLAYER: {
		process_save_player(cid, status, *buffer);
		break;
	}
	case SYNC_MASTER_DB_CREATE_GUILD:{
		Create_Guild_Info guild_info;
		guild_info.deserialize(*buffer);
		process_create_guild(cid, guild_info);
		break;
	}
	case SYNC_MASTER_DB_LOAD_DATA:{
		process_load_master(cid);
		break;
	}
	case SYNC_MASTER_DB_DELETE_DATA: {
		std::string struct_name;
		buffer->read_string(struct_name);
		Struct_Name_Map::iterator iter = DB_MANAGER->db_struct_name_map().find(struct_name);
		if(iter == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the struct_name: %s", struct_name.c_str());
			break;
		}
		Base_Struct *def = iter->second;
		def->delete_data(*buffer);
		break;
	}
	default: {
		Struct_Id_Map::iterator iter = DB_MANAGER->db_struct_id_map().find(msg_id);
		if(iter == DB_MANAGER->db_struct_id_map().end()){
			LOG_ERROR("Can not find the msg_id %d", msg_id);
			break;
		}
		Base_Struct *def = iter->second;
		def->save_data_vector(*buffer);
		break;
	}
	}

	DB_SERVER->push_block(cid, buffer);
	return 0;
}

int DB_Worker::process_create_player(int cid, Create_Role_Info &role_info) {
	int64_t role_id = 0;
	Block_Buffer buf;
	if ((role_id = DB_MANAGER->create_player(role_info)) > 0) {
		buf.make_server_message(SYNC_DB_GAME_CREATE_PLAYER, ROLE_SUCCESS_CREATE);
		buf.write_string(role_info.account);
		//创建所有玩家表,构建登录消息buf
		create_player_data(role_id, buf);
	} else {
		buf.make_server_message(SYNC_DB_GAME_CREATE_PLAYER, ROLE_HAS_EXIST);
		buf.write_string(role_info.account);
	}
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_load_player(int cid, std::string &account) {
	int32_t status = 0;
	int64_t role_id = 0;
	DB_Manager::DB_Cache_Account_Map::iterator iter = DB_MANAGER->db_cache_account_map().find(account);
	if (iter != DB_MANAGER->db_cache_account_map().end())	{
		//角色存在，加载数据
		status = ROLE_SUCCESS_LOAD;
		role_id = iter->second.role_id;
	} else {
		//角色不存在，直接返回
		status = ROLE_NOT_EXIST;
	}

	Block_Buffer buf;
	buf.make_server_message(SYNC_DB_GAME_LOAD_PLAYER, status);
	buf.write_string(account);
	if (role_id > 0) {
		load_player_data(role_id, buf);
	}
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_save_player(int cid, int status, Block_Buffer &buffer) {
	//先把两个额外字段读出来，再保存玩家数据，防止buffer错乱
	bool logout = false;
	std::string account = "";
	buffer.read_bool(logout);
	buffer.read_string(account);
	if (logout) {
		//离线保存
		save_player_data(buffer);

		Block_Buffer buf;
		buf.make_server_message(SYNC_DB_GAME_SAVE_PLAYER, 0);
		buf.write_string(account);
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
	buf.make_server_message(SYNC_DB_MASTER_CREATE_GUILD, 0);
	guild_info.guild_id = DB_MANAGER->create_guild(guild_info);
	guild_info.serialize(buf);
	buf.finish_message();
	DB_MANAGER->send_data_block(cid, buf);
	return 0;
}

int DB_Worker::process_load_master(int cid) {
	Base_Struct *master_struct = DB_MANAGER->get_master_data_struct();
	for(std::vector<Field_Info>::const_iterator iter = master_struct->field_vec().begin();
				iter != master_struct->field_vec().end(); iter++){
		Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(iter->field_type);
		if(it == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the struct_name:%s", iter->field_type.c_str());
			return -1;
		}

		Block_Buffer buf;
		buf.make_server_message(it->second->msg_id() + 400000, 0);
		it->second->load_data(0, buf);
		buf.finish_message();
		DB_MANAGER->send_data_block(cid, buf);
	}

	return 0;
}

int DB_Worker::create_player_data(int64_t role_id,  Block_Buffer &buffer) {
	Base_Struct *role_def = DB_MANAGER->get_player_data_struct();
	for(std::vector<Field_Info>::const_iterator iter = role_def->field_vec().begin();
			iter != role_def->field_vec().end(); iter++) {
		Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(iter->field_type);
		if(it == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the struct_name %s", iter->field_type.c_str());
			return -1;
		}

		if (iter->field_type == "Game_Player_Info") {
			it->second->load_data(role_id, buffer);
		} else {
			it->second->create_data(role_id, buffer);
		}
	}
	return 0;
}

int DB_Worker::load_player_data(int64_t role_id, Block_Buffer &buffer) {
	Base_Struct *player_def = DB_MANAGER->get_player_data_struct();
	for(std::vector<Field_Info>::const_iterator iter = player_def->field_vec().begin();
			iter != player_def->field_vec().end(); iter++){
		Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(iter->field_type);
		if(it == DB_MANAGER->db_struct_name_map().end()){
			LOG_ERROR("Can not find the struct_name %s", iter->field_type.c_str());
			return -1;
		}
		it->second->load_data(role_id, buffer);
	}
	return 0;
}

int DB_Worker::save_player_data(Block_Buffer &buffer) {
	Base_Struct *role_def = DB_MANAGER->get_player_data_struct();
	for(std::vector<Field_Info>::const_iterator iter = role_def->field_vec().begin();
			iter != role_def->field_vec().end(); iter++) {
		Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(iter->field_type);
		if(it == DB_MANAGER->db_struct_name_map().end()) {
			LOG_ERROR("Can not find the struct_name %s", iter->field_type.c_str());
			return -1;
		}
		it->second->save_data(buffer);
	}
	return 0;
}
