/*
 * Msg_Manager.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Common_Func.h"
#include "Server_Config.h"
#include "Msg_Manager.h"

Msg_Manager::Msg_Manager(void) :
	msg_struct_id_map_(get_hash_table_size(512)),
	msg_struct_name_map_(get_hash_table_size(512)) { }

Msg_Manager::~Msg_Manager(void) { }

Msg_Manager *Msg_Manager::instance_;

Msg_Manager *Msg_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Msg_Manager;
	return instance_;
}

int Msg_Manager::init(void) {
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	if (server_misc == Json::Value::null) {
		LOG_FATAL("msg_struct init, server_misc null");
		return -1;
	}

	load_struct(server_misc["msg_struct_path"].asString().c_str(), MSG_STRUCT, msg_struct_id_map_, msg_struct_name_map_);
	load_struct(server_misc["game_db_struct_path"].asString().c_str(), MSG_STRUCT, msg_struct_id_map_, msg_struct_name_map_);
	return 0;
}
