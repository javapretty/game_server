/*
 * Log_File.h
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#ifndef LOG_FILE_H_
#define LOG_FILE_H_

#include "Time_Value.h"
#include "boost/unordered_map.hpp"
#include "Public_Struct.h"

struct File_Info {
	int type;
	int sub_type;
	Time_Value tv;
	std::string file_path;
	FILE *fp;
};

class Log_File {
public:
	typedef boost::unordered_map<int, File_Info> File_Info_Map;

	Log_File(void);
	virtual ~Log_File(void);

	int process_log_file_block(Block_Buffer &buf);

private:
	int get_key_val(int type, int sub_type);

	void make_log_dir(void);
	int make_log_filepath(File_Info &file_info);
	int logging_file(int log_type, int log_sub_type, std::string &log_str);

private:
	File_Info_Map file_info_map_;
};

#endif /* LOG_FILE_H_ */
