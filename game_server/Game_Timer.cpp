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

bool Compare::operator()(V8_Timer_Handler *t1, V8_Timer_Handler *t2){
	return t1->next_time > t2->next_time;
}

void V8_Timer_Queue::push(V8_Timer_Handler *handler){
	container_.push_back(handler);
	std::push_heap(container_.begin(), container_.end(), Compare());
}

V8_Timer_Handler *V8_Timer_Queue::top(){
	return container_.front();
}

void V8_Timer_Queue::pop(){
	std::pop_heap(container_.begin(), container_.end(), Compare());
	container_.pop_back();
}

void V8_Timer_Queue::del(V8_Timer_Handler *handler){
	for(CONTAINER::iterator iter = container_.begin(); iter!= container_.end(); iter++){
		if((*iter)->timer_id == handler->timer_id){
			(*iter)->isUseful = false;
		}
	}
}

bool V8_Timer_Queue::empty(){
	return container_.empty();
}
