/*
 * Server_Config.cpp
 *
 *  Created on: 2015年12月31日
 *      Author: zhangyalei
 */

#include "Configurator.h"
#include "Mutex_Guard.h"
#include <string>

void Configurator::load_server_config(void) {
	GUARD_WRITE(Config_Lock, mon, server_config_.lock);
	{
		std::string path("./config/server/log_misc.json");
		load_json_file(path.c_str(), server_config_.log_misc.get_cur_json());
	}
	{ 	/// server_conf.json
		std::string path("./config/server/server_conf.json");
		load_json_file(path.c_str(), server_config_.server_conf.get_cur_json());
	}
	{ 	/// server_list.json
		std::string path("./config/server/server_list.json");
		load_json_file(path.c_str(), server_config_.server_list.get_cur_json());
	}
	{ 	/// maintainer.json
		std::string path("./config/server/server_maintainer.json");
		load_json_file(path.c_str(), server_config_.server_maintainer.get_cur_json());
	}
	{ 	/// server_misc.json
		std::string path("./config/server/server_misc.json");
		load_json_file(path.c_str(), server_config_.server_misc.get_cur_json());
	}
}

const Json::Value &Configurator::log_misc(void) {
	return get_json_value_with_rlock(server_config_.lock, server_config_.log_misc);
}

const Json::Value &Configurator::server_conf(void) {
	return get_json_value_with_rlock(server_config_.lock, server_config_.server_conf);
}

const Json::Value &Configurator::server_list(void) {
	return get_json_value_with_rlock(server_config_.lock, server_config_.server_list);
}

const Json::Value &Configurator::server_maintainer(void) {
	return get_json_value_with_rlock(server_config_.lock, server_config_.server_maintainer);
}

const Json::Value &Configurator::server_misc(void) {
	return get_json_value_with_rlock(server_config_.lock, server_config_.server_misc);
}
