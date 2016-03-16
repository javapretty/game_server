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
	bool isUseful; //是否继续使用
};

class Compare {
public:
	bool operator()(V8_Timer_Handler *t1, V8_Timer_Handler *t2);
};

//脚本层定时器优先队列
class V8_Timer_Queue {
typedef std::vector<V8_Timer_Handler*> CONTAINER;
public:
	V8_Timer_Handler* top();
	void push(V8_Timer_Handler* handler);
	void pop();
	void del(V8_Timer_Handler* handler);
	bool empty();
private:
	CONTAINER container_;
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
