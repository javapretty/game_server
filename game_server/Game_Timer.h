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
	int timer_id; 				//js层定时器编号
	int interval; 				//定时时间间隔(毫秒为单位)
	Time_Value next_tick;	//下一次执行时间
};

class V8_Timer_Compare {
public:
	inline bool operator()(V8_Timer_Handler *t1, V8_Timer_Handler *t2) {
		return t1->next_tick > t2->next_tick;
	}
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
