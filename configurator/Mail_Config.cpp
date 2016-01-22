/*
* Mail_Config.cpp
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#include "Configurator.h"
#include "Mutex_Guard.h"
#include <string>

void Configurator::load_mail_config(void) {
	GUARD_WRITE(Config_Lock, mon, mail_config_.lock);

	mail_config_.mail.clear_next_json();
	load_json_file("config/mail/mail.json", mail_config_.mail.get_next_json());
	mail_config_.mail.update_version();
}

const Json::Value &Configurator::mail_config(const std::string &type_name) {
	GUARD_READ(Config_Lock, mon, mail_config_.lock);
	return mail_config_.mail.get_cur_json()[type_name];
}
