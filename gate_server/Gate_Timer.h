/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef GATE_TIMER_H_
#define GATE_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Block_Buffer.h"

class Gate_Timer_Handler: public Event_Handler {
public:
	Gate_Timer_Handler(void);
	virtual ~Gate_Timer_Handler(void);

	virtual int handle_timeout(const Time_Value &tv);
};

class Gate_Timer: public Thread {
public:
	static Gate_Timer *instance(void);
	virtual void run_handler(void);

	Epoll_Watcher &watcher(void);
	void register_handler(void);

private:
	Gate_Timer(void);
	virtual ~Gate_Timer(void);
	Gate_Timer(const Gate_Timer &);
	const Gate_Timer &operator=(const Gate_Timer &);

private:
	Epoll_Watcher watcher_;
	Gate_Timer_Handler timer_handler_;
	static Gate_Timer *instance_;
};

#define GATE_TIMER Gate_Timer::instance()

#endif /* GATE_TIMER_H_ */
