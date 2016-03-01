/*
 * Configurator.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "Configurator.h"
#include "Common_Func.h"
#include "Log.h"

Configurator::Configurator(void) {
	load_server_config();
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
		LOG_FATAL("Failed to open file %s\n error: %s\n.\n", file_path, reader_.getFormatedErrorMessages().c_str());
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0)
		perror("fstat"), exit(1);

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		LOG_ERROR("mmap");
		LOG_FATAL();
	}

	if (! reader_.parse(static_cast<const char *>(src), static_cast<const char *>(src) + statbuf.st_size, conf_value)) {
        LOG_FATAL("Failed to parse file %s\n error: %s\n.\n", file_path, reader_.getFormatedErrorMessages().c_str());
	}

	if (munmap(src, statbuf.st_size) < 0) {
		LOG_ERROR("munmap");
		LOG_FATAL();
	}

	close(fd);

	return 0;
}
