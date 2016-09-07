/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Login_Manager.h"
#include "Login_Timer.h"

Login_Timer_Handler::Login_Timer_Handler(void) { }

Login_Timer_Handler::~Login_Timer_Handler(void) { }

int Login_Timer_Handler::handle_timeout(const Time_Value &tv) {
	return LOGIN_MANAGER->push_tick(tv.sec());
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
	watcher_.add(&timer_handler_, EVENT_TIMEOUT, &timeout_tv);
}
