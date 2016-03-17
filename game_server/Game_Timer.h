/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_TIMER_H_
#define GAME_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Block_Buffer.h"

struct V8_Timer_Handler {
	int timer_id; //js层定时器编号
	int interval; //定时时间间隔
	int next_time; //下一次执行时间(UNIX时间戳)
};

class V8_Timer_Compare {
public:
	bool operator()(V8_Timer_Handler *t1, V8_Timer_Handler *t2);
};

//优先队列
template <typename Obj, typename Compare, typename LOCK = NULL_MUTEX>
class Priority_Queue {
typedef std::vector<Obj> CONTAINER;
public:
	Obj top(){
		GUARD(LOCK, mon, this->lock_);
		return container_.front();
	}
	void push(Obj obj){
		GUARD(LOCK, mon, this->lock_);
		container_.push_back(obj);
		std::push_heap(container_.begin(), container_.end(), Compare());
	}
	void pop(){
		GUARD(LOCK, mon, this->lock_);
		std::pop_heap(container_.begin(), container_.end(), Compare());
		container_.pop_back();
	}
	bool empty(){
		GUARD(LOCK, mon, this->lock_);
		return container_.empty();
	}
private:
	CONTAINER container_;
	LOCK lock_;
};

class Game_Timer_Handler: public Event_Handler {
public:
	Game_Timer_Handler(void);
	virtual ~Game_Timer_Handler(void);

	void init_tick_msg_buf(void);

	virtual int handle_timeout(const Time_Value &tv);

private:
	Block_Buffer tick_msg_buf_;
};

class Game_Timer: public Thread {
public:
	static Game_Timer *instance(void);

	virtual void run_handler(void);

	Epoll_Watcher &watcher(void);

	void register_handler(void);

private:
	Game_Timer(void);
	virtual ~Game_Timer(void);
	Game_Timer(const Game_Timer &);
	const Game_Timer &operator=(const Game_Timer &);

private:
	Epoll_Watcher watcher_;
	Game_Timer_Handler timer_handler_;
	static Game_Timer *instance_;
};

#define GAME_TIMER Game_Timer::instance()

#endif /* GAME_TIMER_H_ */
