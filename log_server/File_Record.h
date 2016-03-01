/*
 * File_Record.h
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#ifndef FILE_RECORD_H_
#define FILE_RECORD_H_

#include "Time_Value.h"
#include "boost/unordered_map.hpp"

struct File_Info {
	int type;
	int sub_type;
	Time_Value tv;
	std::string file_path;
	FILE *fp;
};

class File_Record {
public:
	typedef boost::unordered_map<int, File_Info> File_Info_Map;

	enum {
		LOG_LOGIN	= 1,
		LOG_GATE		= 2,
		LOG_GAME		= 3,
		LOG_MASTER	= 4,
		LOG_DB 		= 5,
		LOG_MISC 	= 6,
	};

	File_Record(void);
	virtual ~File_Record(void);

	int process_log_file_block(Block_Buffer &buf);

private:
	int get_key_val(int type, int sub_type);

	void make_log_dir(void);
	int make_log_filepath(File_Info &file_info);
	int logging_file(int log_type, int log_sub_type, std::string &log_str);

private:
	File_Info_Map file_info_map_;
};

#endif /* FILE_RECORD_H_ */
