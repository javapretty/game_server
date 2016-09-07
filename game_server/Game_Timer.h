/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef GAME_TIMER_H_
#define GAME_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Priority_Queue.h"
#include "List.h"
#include "Public_Struct.h"

class Game_Timer_Handler: public Event_Handler {
public:
	Game_Timer_Handler(void);
	virtual ~Game_Timer_Handler(void);

	virtual int handle_timeout(const Time_Value &tv);
};

class Game_Timer: public Thread {
public:
	typedef Object_Pool<V8_Timer_Handler, Thread_Mutex> V8_Timer_Pool;
	typedef Priority_Queue<V8_Timer_Handler*, V8_Timer_Compare, Thread_Mutex> V8_Timer_Queue;
	typedef List<int, Thread_Mutex> Timer_List;

	static Game_Timer *instance(void);
	virtual void run_handler(void);
	Epoll_Watcher &watcher(void);

	int v8_tick(const Time_Value &now);

	void register_handler(void);
	void register_v8_handler(int timer_id, int internal, int first_tick_internal); //注册js层定时器
	int pop_v8_timer(void); //js层获取超时定时器编号

private:
	Game_Timer(void);
	virtual ~Game_Timer(void);
	Game_Timer(const Game_Timer &);
	const Game_Timer &operator=(const Game_Timer &);

private:
	Epoll_Watcher watcher_;
	Game_Timer_Handler timer_handler_;
	V8_Timer_Pool v8_timer_pool_;
	V8_Timer_Queue v8_timer_queue_;
	Timer_List v8_timer_list_;

	static Game_Timer *instance_;
};

#define GAME_TIMER Game_Timer::instance()

inline int Game_Timer::pop_v8_timer(void){
	if (v8_timer_list_.empty()) {
		return 0;
	}
	return v8_timer_list_.pop_front();
}

#endif /* GAME_TIMER_H_ */
