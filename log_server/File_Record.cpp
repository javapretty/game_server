/*
 * File_Record.cpp
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <cstdio>
#include <string>
#include <sstream>
#include "Log.h"
#include "Block_Buffer.h"
#include "File_Record.h"
#include "Time_Value.h"

File_Record::File_Record(void) { }

File_Record::~File_Record(void) { }

void File_Record::make_log_dir(void) {
	int ret = mkdir("./log", 0775);
	if (ret == -1 && errno != EEXIST) {
		perror("mkdir error");
	}
}

int File_Record::make_log_filepath(File_Info &file_info) {
	time_t time_v = file_info.tv.sec();
	struct tm tm_v;
	localtime_r(&time_v, &tm_v);

	file_info.file_path.clear();

	switch (file_info.type) {
	case Log::LOG_MISC: {
		file_info.file_path.append("./log/misc-");
		break;
	}
	case Log::LOG_LOG_SERVER: {
		file_info.file_path.append("./log/logserver-");
		break;
	}
	case Log::LOG_DB_SERVER: {
		file_info.file_path.append("./log/dbserver-");
		break;
	}
	case Log::LOG_LOGIN_SERVER: {
		file_info.file_path.append("./log/loginserver-");
		break;
	}
	case Log::LOG_MASTER_SERVER: {
		file_info.file_path.append("./log/masterserver-");
		break;
	}
	case Log::LOG_GAME_SERVER: {
		file_info.file_path.append("./log/gameserver-");
		break;
	}
	case Log::LOG_GATE_SERVER: {
		file_info.file_path.append("./log/gateserver-");
		break;
	}
	default: {
		file_info.file_path.append("./log/misc-");
		break;
	}
	}

	std::stringstream stream;
	stream << (file_info.sub_type) << "-";
	stream 	<< (tm_v.tm_year + 1900) << "-" << (tm_v.tm_mon + 1) << "-" << (tm_v.tm_mday)
			<< "-" << (tm_v.tm_hour) << ".log";

	file_info.file_path.append(stream.str());

	return 0;
}

int File_Record::get_key_val(int type, int sub_type) {
	return type * 1000 + sub_type;
}

int File_Record::logging_file(int log_type, int log_sub_type, std::string &log_str) {
	File_Info *file_info = 0;
	int key = get_key_val(log_type, log_sub_type);

	File_Info_Map::iterator file_it = file_info_map_.find(key);
	if (file_it == file_info_map_.end()) {
		make_log_dir();

		File_Info info;
		info.type = log_type;
		info.sub_type = log_sub_type;
		info.tv = Time_Value::gettimeofday();
		make_log_filepath(info);

		if ((info.fp = fopen(info.file_path.c_str(), "a")) == NULL) {
			perror("fopen error");
			return -1;
		}

		file_info_map_[key] = info;
		file_info = &(file_info_map_[key]);
	} else {
		file_info = &(file_it->second);
		if (! is_same_hour(file_it->second.tv, Time_Value::gettimeofday())) {
			fclose(file_it->second.fp);
			file_it->second.tv = Time_Value::gettimeofday();
			make_log_filepath(file_it->second);

			if ((file_it->second.fp = fopen(file_it->second.file_path.c_str(), "a")) == NULL) {
				perror("fopen error");
				return -1;
			}
		}
	}

	fputs(log_str.c_str(), file_info->fp);
	fflush(file_info->fp);

	return 0;
}

int File_Record::process_log_file_block(Block_Buffer &buf) {
	int log_type = buf.read_int32();
	int log_sub_type = buf.read_int32();
	std::string log_str = buf.read_string();
	logging_file(log_type, log_sub_type, log_str);
	return 0;
}
