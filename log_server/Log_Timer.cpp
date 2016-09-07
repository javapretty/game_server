/*
 * Log_Timer.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#include "Log_Timer.h"
#include "Log_Manager.h"

int Log_Timer_Handler::handle_timeout(const Time_Value &tv) {
	LOG_MANAGER->push_tick(1);
	return 0;
}

Log_Timer::Log_Timer(void) { }

Log_Timer::~Log_Timer(void) { }

Log_Timer *Log_Timer::instance_ = 0;

Log_Timer *Log_Timer::instance(void) {
	if (! instance_)
		instance_ = new Log_Timer;
	return instance_;
}

void Log_Timer::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Log_Timer::run_handler(void) {
	register_handler();
	watcher_.loop();
}

void Log_Timer::register_handler(void) {
	Time_Value timeout_tv(0, 100 * 1000);
	watcher_.add(&timer_handler_, EVENT_TIMEOUT, &timeout_tv);
}
