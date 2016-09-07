/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef LOGIN_TIMER_H_
#define LOGIN_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"

class Login_Timer_Handler: public Event_Handler {
public:
	Login_Timer_Handler(void);
	virtual ~Login_Timer_Handler(void);

	virtual int handle_timeout(const Time_Value &tv);
};

class Login_Timer: public Thread {
public:
	static Login_Timer *instance(void);
	virtual void run_handler(void);

	Epoll_Watcher &watcher(void);
	void register_handler(void);

private:
	Login_Timer(void);
	virtual ~Login_Timer(void);
	Login_Timer(const Login_Timer &);
	const Login_Timer &operator=(const Login_Timer &);

private:
	Epoll_Watcher watcher_;
	Login_Timer_Handler timer_handler_;
	static Login_Timer *instance_;
};

#define LOGIN_TIMER Login_Timer::instance()

#endif /* LOGIN_TIMER_H_ */
