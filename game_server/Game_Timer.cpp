/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#include "Msg_Define.h"
#include "Game_Manager.h"
#include "Game_Timer.h"

Game_Timer::Game_Timer(void) { }

Game_Timer::~Game_Timer(void) { }

Game_Timer *Game_Timer::instance_;

Game_Timer *Game_Timer::instance(void) {
	if (instance_ == 0)
		instance_ = new Game_Timer;
	return instance_;
}

void Game_Timer::run_handler(void) {
	register_handler();
	watcher_.loop();
}

Epoll_Watcher &Game_Timer::watcher(void) {
	return watcher_;
}

void Game_Timer::register_handler(void) {
	Time_Value timeout_tv(0, 100 * 1000);
	watcher_.add(&timer_handler_, Epoll_Watcher::EVENT_TIMEOUT, &timeout_tv);
}

Game_Timer_Handler::Game_Timer_Handler(void) {
	init_tick_msg_buf();
}

Game_Timer_Handler::~Game_Timer_Handler(void) { }

void Game_Timer_Handler::init_tick_msg_buf(void) {
	tick_msg_buf_.reset();
	tick_msg_buf_.make_inner_message(SYNC_INNER_TIMER_TICK);
	tick_msg_buf_.finish_message();
}

int Game_Timer_Handler::handle_timeout(const Time_Value &tv) {
	return GAME_MANAGER->push_self_loop_message(tick_msg_buf_);
}
