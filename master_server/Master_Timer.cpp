/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Master_Manager.h"
#include "Master_Timer.h"
#include "Msg_Define.h"

Master_Timer::Master_Timer(void) { }

Master_Timer::~Master_Timer(void) { }

Master_Timer *Master_Timer::instance_;

Master_Timer *Master_Timer::instance(void) {
	if (instance_ == 0)
		instance_ = new Master_Timer;
	return instance_;
}

void Master_Timer::run_handler(void) {
	register_handler();
	watcher_.loop();
}

Epoll_Watcher &Master_Timer::watcher(void) {
	return watcher_;
}

void Master_Timer::register_handler(void) {
	Time_Value timeout_tv(0, 100 * 1000);
	watcher_.add(&timer_handler_, Epoll_Watcher::EVENT_TIMEOUT, &timeout_tv);
}

Master_Timer_Handler::Master_Timer_Handler(void) {
	init_tick_msg_buf();
}

Master_Timer_Handler::~Master_Timer_Handler(void) { }

void Master_Timer_Handler::init_tick_msg_buf(void) {
	tick_msg_buf_.reset();
	tick_msg_buf_.make_inner_message(SYNC_INNER_TIMER_TICK);
	tick_msg_buf_.finish_message();
}

int Master_Timer_Handler::handle_timeout(const Time_Value &tv) {
	return MASTER_MANAGER->push_self_loop_message(tick_msg_buf_);
}
