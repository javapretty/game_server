/*
 * Log_Manager.h
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include "Log_File.h"
#include "Thread.h"
#include "List.h"
#include "Block_List.h"

class Log_Manager: public Thread {
public:
	typedef Block_List<Thread_Mutex> Data_List;
	typedef List<int, Thread_Mutex> Int_List;

	static Log_Manager *instance(void);
	int init(int server_id);
	void run_handler(void);

	int push_data_block(Block_Buffer *buf);
	int push_tick(int x);

	int process_list(void);
	int process_block(Block_Buffer &buf);

	int tick(void);

private:
	Log_Manager(void);
	virtual ~Log_Manager(void);

private:
	static Log_Manager *instance_;
	Data_List block_list_;
	Int_List tick_list_;

	Log_File log_file_;
};

#define LOG_MANAGER Log_Manager::instance()

#endif /* LOG_MANAGER_H_ */
