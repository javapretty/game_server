/*
 * File_Record.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: zhangyalei
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <cstdio>
#include <string>
#include <sstream>
#include "File_Record.h"
#include "Log.h"
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
	case Log::LOG_LOGIN: {
		file_info.file_path.append("./log/login-");
		break;
	}
	case Log::LOG_GATE: {
		file_info.file_path.append("./log/gate-");
		break;
	}
	case Log::LOG_GAME: {
		file_info.file_path.append("./log/game");
		break;
	}
	case Log::LOG_MASTER: {
		file_info.file_path.append("./log/master-");
		break;
	}
	case Log::LOG_DB: {
		file_info.file_path.append("./log/db-");
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

int File_Record::logging_file(MSG_180000 &msg) {
	File_Info *file_info = 0;
	int key = get_key_val(msg.log_type, msg.log_sub_type);

	File_Info_Map::iterator file_it = file_info_map_.find(key);
	if (file_it == file_info_map_.end()) {
		make_log_dir();

		File_Info info;

		info.type = msg.log_type;
		info.sub_type = msg.log_sub_type;
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

	fputs(msg.log_str.c_str(), file_info->fp);
	fflush(file_info->fp);

	return 0;
}

int File_Record::buffer_file(MSG_180001 &msg) {
	Buf_File_Info *file_info = 0;

	Buf_File_Info_Map::iterator file_it = buf_file_info_map_.find(msg.role_id);
	if (file_it == buf_file_info_map_.end()) {
		Buf_File_Info info;
		info.file_name = msg.file_name;
		info.file_path.clear();
		std::stringstream filepath_stream;
		filepath_stream << "./buffer_file/" << msg.file_name << ".buf";
		info.file_path.append(filepath_stream.str().c_str());

		if ((info.fp = fopen(info.file_path.c_str(), "a")) == NULL) {
			perror("fopen error");
			return -1;
		}

		buf_file_info_map_[msg.role_id] = info;
		file_info = &(buf_file_info_map_[msg.role_id]);
	} else {
		file_info = &(file_it->second);
		if (file_it->second.file_name != msg.file_name) {
			fclose(file_it->second.fp);

			file_it->second.file_name = msg.file_name;
			file_it->second.file_path.clear();
			std::stringstream filepath_stream;
			filepath_stream << "./buffer_file/" << msg.file_name << ".buf";
			file_it->second.file_path.append(filepath_stream.str().c_str());

			if ((file_it->second.fp = fopen(file_it->second.file_path.c_str(), "a")) == NULL) {
				perror("fopen error");
				return -1;
			}
		}
	}

	fwrite(msg.buf.c_str(), msg.buf.size(), 1, file_info->fp);
	fflush(file_info->fp);

	return 0;
}

int File_Record::process_log_file_block(Block_Buffer &buf) {
	MSG_180000 msg;
	msg.deserialize(buf);
	logging_file(msg);
	return 0;
}

int File_Record::process_buf_file_block(Block_Buffer &buf) {
	MSG_180001 msg;
	msg.deserialize(buf);
	buffer_file(msg);
	return 0;
}
