/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Gate_Manager.h"
#include "Gate_Timer.h"

Gate_Timer_Handler::Gate_Timer_Handler(void) { }

Gate_Timer_Handler::~Gate_Timer_Handler(void) { }

int Gate_Timer_Handler::handle_timeout(const Time_Value &tv) {
	return GATE_MANAGER->push_tick(tv.sec());
}

Gate_Timer::Gate_Timer(void) { }

Gate_Timer::~Gate_Timer(void) { }

Gate_Timer *Gate_Timer::instance_;

Gate_Timer *Gate_Timer::instance(void) {
	if (instance_ == 0)
		instance_ = new Gate_Timer;
	return instance_;
}

void Gate_Timer::run_handler(void) {
	register_handler();
	watcher_.loop();
}

Epoll_Watcher &Gate_Timer::watcher(void) {
	return watcher_;
}

void Gate_Timer::register_handler(void) {
	Time_Value timeout_tv(0, 100 * 1000);
	watcher_.add(&timer_handler_, EVENT_TIMEOUT, &timeout_tv);
}
