/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef MASTER_TIMER_H_
#define MASTER_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Priority_Queue.h"
#include "List.h"
#include "Public_Struct.h"

class Master_Timer_Handler: public Event_Handler {
public:
	Master_Timer_Handler(void);
	virtual ~Master_Timer_Handler(void);

	virtual int handle_timeout(const Time_Value &tv);
};

class Master_Timer: public Thread {
public:
	typedef Object_Pool<V8_Timer_Handler, Thread_Mutex> V8_Timer_Pool;
	typedef Priority_Queue<V8_Timer_Handler*, V8_Timer_Compare, Thread_Mutex> V8_Timer_Queue;
	typedef List<int, Thread_Mutex> Timer_List;

	static Master_Timer *instance(void);
	virtual void run_handler(void);
	Epoll_Watcher &watcher(void);

	int v8_tick(const Time_Value &now);

	void register_handler(void);
	void register_v8_handler(int timer_id, int internal, int first_tick_internal); //注册js层定时器
	int pop_v8_timer(void); //js层获取超时定时器编号

private:
	Master_Timer(void);
	virtual ~Master_Timer(void);
	Master_Timer(const Master_Timer &);
	const Master_Timer &operator=(const Master_Timer &);

private:
	Epoll_Watcher watcher_;
	Master_Timer_Handler timer_handler_;
	V8_Timer_Pool v8_timer_pool_;
	V8_Timer_Queue v8_timer_queue_;
	Timer_List v8_timer_list_;

	static Master_Timer *instance_;
};

#define MASTER_TIMER Master_Timer::instance()

inline int Master_Timer::pop_v8_timer(void){
	if (v8_timer_list_.empty()) {
		return 0;
	}
	return v8_timer_list_.pop_front();
}

#endif /* MASTER_TIMER_H_ */
