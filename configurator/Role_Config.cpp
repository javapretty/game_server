/*
 * Role_Config.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#include "Configurator.h"
#include "Mutex_Guard.h"
#include <string>

void Configurator::load_role_config(void) {
	GUARD_WRITE(Config_Lock, mon, role_config_.lock);

	/*不需要热更新加载方法
	{
		std::string experience_path("config/role/experience.json");
		load_json_file(experience_path.c_str(), role_config_.experience.get_cur_json());
	}*/
	//热更新加载方法
	{
		role_config_.experience.clear_next_json();
		load_json_file("config/role/experience.json", role_config_.experience.get_next_json());
		role_config_.experience.update_version();
	}
}
