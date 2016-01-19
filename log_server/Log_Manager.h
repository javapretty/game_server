/*
 * Log_Manager.h
 *
 *  Created on: Aug 28, 2012
 *      Author: zhangyalei
 */

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include "Thread.h"
#include "Thread_Mutex.h"
#include "List.h"
#include "File_Record.h"
#include "DB_Record.h"
#include "Public_Define.h"

class Block_Buffer;

class Log_Manager: public Thread {
public:
	typedef List<Block_Buffer *, Thread_Mutex> Block_List;
	typedef List<int, Thread_Mutex> Tick_List;

	static Log_Manager *instance(void);

	int init(void);

	int push_data_block(Block_Buffer *buf);

	int push_tick(int x);

	void run_handler(void);

	int process_list(void);

	int process_block(Block_Buffer &buf);

private:
	Log_Manager(void);
	virtual ~Log_Manager(void);

	int tick(void);

private:
	static Log_Manager *instance_;
	Block_List block_list_;
	Tick_List tick_list_;

	File_Record file_record_;
	DB_Record db_record_;
};

#define LOG_MANAGER Log_Manager::instance()

#endif /* LOG_MANAGER_H_ */
