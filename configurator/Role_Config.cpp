/*
 * Role_Config.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#include "Configurator.h"

void Configurator::load_role_config(void) {
	GUARD_WRITE(Config_Lock, mon, role_config_.lock);

	/*不需要热更新加载方法
	{
		std::string experience_path("config/role/experience.json");
		load_json_file(experience_path.c_str(), role_config_.experience.get_cur_json());
	}*/
	//热更新加载方法
	{
		role_config_.bename.clear_next_json();
		load_json_file("config/role/bename.json", role_config_.bename.get_next_json());
		role_config_.bename.update_version();
	}
	{
		role_config_.experience.clear_next_json();
		load_json_file("config/role/experience.json", role_config_.experience.get_next_json());
		role_config_.experience.update_version();
	}
}

const Json::Value &Configurator::bename_config(void) {
	return get_json_value_with_rlock(server_config_.lock, role_config_.bename);
}

const Json::Value &Configurator::exp_config(void) {
	return get_json_value_with_rlock(server_config_.lock, role_config_.experience);
}
