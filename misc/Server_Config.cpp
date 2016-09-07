/*
 * Server_Config.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#include "Server_Config.h"

Server_Config::Server_Config(void) { }

Server_Config::~Server_Config(void) { }

Server_Config * Server_Config::instance_;

Server_Config *Server_Config::instance() {
	if (! instance_)
		instance_ = new Server_Config;
	return instance_;
}

void Server_Config::load_server_config(void) {
	GUARD_WRITE(Config_Lock, mon, server_config_.lock);
	{
		std::string path("./config/server/server_conf.json");
		load_json_file(path.c_str(), server_config_.server_conf.get_cur_json());
	}
	{
		std::string path("./config/server/server_misc.json");
		load_json_file(path.c_str(), server_config_.server_misc.get_cur_json());
	}
}

const Json::Value &Server_Config::server_conf(void) {
	return get_json_value_with_rlock(server_config_.lock, server_config_.server_conf);
}

const Json::Value &Server_Config::server_misc(void) {
	return get_json_value_with_rlock(server_config_.lock, server_config_.server_misc);
}
