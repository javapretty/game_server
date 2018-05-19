/*
 * Config.cpp
 *
 *  Created on: Mar 1,2016
 *      Author: zhangyalei
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "Config.h"
#include "Lib_Log.h"

Config::Config(void) { }

Config::~Config(void) { }

int Config::load_json_file(const char *file_path, Json::Value &conf_value) {
	int fd = 0;
	if ((fd = open(file_path, O_RDONLY)) < 0) {
		LIB_LOG_FATAL("Failed to open file %s\n error: %s\n.\n", file_path, reader_.getFormatedErrorMessages().c_str());
	}

	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0) {
		LIB_LOG_FATAL("fstat error file %s\n", file_path);
	}

	void *src = 0;
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		LIB_LOG_FATAL("mmap error file %s\n", file_path);
	}

	if (! reader_.parse(static_cast<const char *>(src), static_cast<const char *>(src) + statbuf.st_size, conf_value)) {
		LIB_LOG_FATAL("Failed to parse file %s\n error: %s\n.\n", file_path, reader_.getFormatedErrorMessages().c_str());
	}

	if (munmap(src, statbuf.st_size) < 0) {
		LIB_LOG_FATAL("munmap error file %s\n", file_path);
	}

	close(fd);

	return 0;
}
