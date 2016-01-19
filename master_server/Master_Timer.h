/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef MASTER_TIMER_H_
#define MASTER_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Block_Buffer.h"

class Master_Timer_Handler: public Event_Handler {
public:
	Master_Timer_Handler(void);
	virtual ~Master_Timer_Handler(void);

	void init_tick_msg_buf(void);

	virtual int handle_timeout(const Time_Value &tv);

private:
	Block_Buffer tick_msg_buf_;
};

class Master_Timer: public Thread {
public:
	static Master_Timer *instance(void);

	virtual void run_handler(void);

	Epoll_Watcher &watcher(void);

	void register_handler(void);

private:
	Master_Timer(void);
	virtual ~Master_Timer(void);
	Master_Timer(const Master_Timer &);
	const Master_Timer &operator=(const Master_Timer &);

private:
	Epoll_Watcher watcher_;
	Master_Timer_Handler timer_handler_;
	static Master_Timer *instance_;
};

#define MASTER_TIMER Master_Timer::instance()

#endif /* MASTER_TIMER_H_ */
