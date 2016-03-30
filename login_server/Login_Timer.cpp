/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#include "Msg_Define.h"
#include "Login_Manager.h"
#include "Login_Timer.h"

Login_Timer_Handler::Login_Timer_Handler(void) {
	init_tick_msg_buf();
}

Login_Timer_Handler::~Login_Timer_Handler(void) { }

void Login_Timer_Handler::init_tick_msg_buf(void) {
	tick_msg_buf_.reset();
	tick_msg_buf_.make_inner_message(SYNC_INNER_TIMER_TICK);
	tick_msg_buf_.finish_message();
}

int Login_Timer_Handler::handle_timeout(const Time_Value &tv) {
	return LOGIN_MANAGER->push_self_loop_message(tick_msg_buf_);
}

Login_Timer::Login_Timer(void) { }

Login_Timer::~Login_Timer(void) { }

Login_Timer *Login_Timer::instance_;

Login_Timer *Login_Timer::instance(void) {
	if (instance_ == 0)
		instance_ = new Login_Timer;
	return instance_;
}

void Login_Timer::run_handler(void) {
	register_handler();
	watcher_.loop();
}

Epoll_Watcher &Login_Timer::watcher(void) {
	return watcher_;
}

void Login_Timer::register_handler(void) {
	Time_Value timeout_tv(0, 100 * 1000);
	watcher_.add(&timer_handler_, Epoll_Watcher::EVENT_TIMEOUT, &timeout_tv);
}
