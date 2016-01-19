/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_TIMER_H_
#define GAME_TIMER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Block_Buffer.h"

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
