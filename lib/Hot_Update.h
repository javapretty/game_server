/*
 * Hot_Update.h
 *
 *  Created on: Mar 1,2016
 *      Author: zhangyalei
 */

#ifndef HOT_UPDATE_H_
#define HOT_UPDATE_H_

#include <vector>
#include <set>
#include "Thread.h"
#include "boost/unordered_map.hpp"

class Hot_Update: public Thread {
public:
	typedef std::set<std::string> MD5_STR_SET;
	typedef boost::unordered_map<std::string, MD5_STR_SET> MD5_STR_MAP;

	Hot_Update();
	virtual ~Hot_Update();

	static Hot_Update *instance(void);

	virtual void run_handler(void);

	int notice_update(const std::string module);

	std::string file_md5(std::string file_name);

	void check_config(std::string module);
	void init_all_module(void);
	void get_md5_str(std::string path, MD5_STR_SET &md5_str_set);

private:
	std::vector<std::string> update_module_;
	Time_Value update_time_;
	static Hot_Update *instance_;
	MD5_STR_MAP md5_str_map_;

	Time_Value notify_interval_;
};
#define HOT_UPDATE Hot_Update::instance()

#endif /* HOTUPDATE_H_ */
