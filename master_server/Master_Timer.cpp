/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Master_Manager.h"
#include "Master_Timer.h"

Master_Timer_Handler::Master_Timer_Handler(void) { }

Master_Timer_Handler::~Master_Timer_Handler(void) { }

int Master_Timer_Handler::handle_timeout(const Time_Value &tv) {
	MASTER_MANAGER->push_tick(tv.sec());
	MASTER_TIMER->v8_tick(tv);
	return 0;
}

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
	watcher_.add(&timer_handler_, EVENT_TIMEOUT, &timeout_tv);
}

void Master_Timer::register_v8_handler(int timer_id, int internal, int first_tick_internal) {
	V8_Timer_Handler *handler = v8_timer_pool_.pop();
	handler->timer_id = timer_id;
	handler->interval = internal;
	handler->next_tick = Time_Value::gettimeofday() + Time_Value(first_tick_internal);
	v8_timer_queue_.push(handler);
}

int Master_Timer::v8_tick(const Time_Value &now){
	while(!v8_timer_queue_.empty() && (now > v8_timer_queue_.top()->next_tick)) {
		V8_Timer_Handler *handler = v8_timer_queue_.top();
		v8_timer_queue_.pop();
		v8_timer_list_.push_back(handler->timer_id);
		handler->next_tick += Time_Value(handler->interval / 1000, handler->interval % 1000 * 1000);
		v8_timer_queue_.push(handler);
	}
	return 0;
}
