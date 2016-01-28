// -*- C++ -*-
/*
 * Configurator.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#include "Configurator.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "Common_Func.h"
#include "Log.h"

Configurator::Configurator(void) {
	load_all_config();
}

Configurator::~Configurator(void) { }

Configurator * Configurator::instance_;

Configurator *Configurator::instance() {
	if (! instance_)
		instance_ = new Configurator;
	return instance_;
}

int Configurator::load_json_file(const char *file_path, Json::Value &conf_value) {
	int fd = 0;
	if ((fd = open(file_path, O_RDONLY)) < 0) {
		MSG_ABORT("Failed to open file %s\n error: %s\n.\n", file_path, reader_.getFormatedErrorMessages().c_str());
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		perror("fstat"), exit(1);

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		MSG_SYS("mmap");
		MSG_ABORT();
	}

	if (! reader_.parse(static_cast<const char *>(src), static_cast<const char *>(src) + statbuf.st_size, conf_value)) {
        MSG_ABORT("Failed to parse file %s\n error: %s\n.\n", file_path, reader_.getFormatedErrorMessages().c_str());
	}

	if (munmap(src, statbuf.st_size) < 0) {
		MSG_SYS("munmap");
		MSG_ABORT();
	}

	close(fd);

	return 0;
}

int Configurator::hot_update_conf(std::string &module, int server) {
	MSG_DEBUG("server:%d self update %s", server, module.c_str());
	if (module == "server") {
		load_server_config();
		const Json::Value &log_misc_json = CONFIG_INSTANCE->log_misc();
		if (log_misc_json == Json::Value::null) {
			MSG_ABORT("configure file error.");
		}
		Lib_Log::instance()->set_switcher(log_misc_json["lib_log_switcher"].asInt());
		Log::instance()->set_switcher(log_misc_json["msg_log_switcher"].asInt());
	} else if (module == "role") {
		load_role_config();
	} else {
		MSG_USER("hot update fail module:%s not init", module.c_str());
		return -1;
	}

	return 0;
}

void Configurator::load_all_config(void) {
	load_server_config();
	load_role_config();
	load_bag_config();
	load_mail_config();
}
