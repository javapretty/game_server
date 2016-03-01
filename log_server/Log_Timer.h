/*
 * Log_Timer.h
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#ifndef LOG_SERVER_TIMER_H_
#define LOG_SERVER_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Block_Buffer.h"

class Log_Timer_Handler: public Event_Handler {
public:
	virtual int handle_timeout(const Time_Value &tv);
};

class Log_Timer: public Thread {
public:
	static Log_Timer *instance(void);
	static void destroy(void);

	virtual void run_handler(void);

	void register_handler(void);

private:
	Log_Timer(void);
	virtual ~Log_Timer(void);

private:
	static Log_Timer *instance_;
	Epoll_Watcher watcher_;
	Log_Timer_Handler timer_handler_;
};

#define LOG_TIMER Log_Timer::instance()

#endif /* LOG_SERVER_TIMER_H_ */
