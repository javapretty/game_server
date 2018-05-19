/*
 * Event_Handler.cpp
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include "Time_Value.h"

class Epoll_Watcher;
class Event_Handler {
public:
	class greater {
	public:
		inline bool operator()(const Event_Handler *a, const Event_Handler *b) {
			return (a->get_absolute_tv()) > (b->get_absolute_tv());
		}
	};

	Event_Handler(void) : io_flag(0), fd_(0), timer_flag(0), reactor_(0), heart_idx_(0) { }
	virtual ~Event_Handler(void) { }

	/// callback interface.
	virtual int handle_input(void) {
		return 0;
	}

	virtual int handle_output(void) {
		return 0;
	}

	virtual int handle_close(void) {
		return 0;
	}

	virtual int handle_timeout(const Time_Value &tv) {
		return 0;
	}

	/// getter/setter
	int get_fd(void) const {
		return fd_;
	}

	void set_fd(int fd) {
		fd_ = fd;
	}

	void set_tv(Time_Value &ab_tv) {
		this->absolute_tv_ = ab_tv;
	}

	void set_tv(Time_Value &ab_tv, Time_Value &re_tv) {
		this->absolute_tv_ = ab_tv;
		this->relative_tv_ = re_tv;
	}

	const Time_Value &get_absolute_tv(void) const {
		return absolute_tv_;
	}

	Time_Value &get_relative_tv(void) {
		return relative_tv_;
	}

	int get_timer_flag(void) const {
		return timer_flag;
	}

	void set_timer_flag(int flag) {
		timer_flag |= flag;
	}

	int get_io_flag(void) const {
		return io_flag;
	}

	void set_io_flag(int flag) {
		io_flag |= flag;
	}

	int get_heart_idx(void) const {
		return heart_idx_;
	}

	void set_heart_idx(int idx) {
		heart_idx_ = idx;
	}

	Epoll_Watcher *get_reactor(void) {
		return reactor_;
	}

	void set_reactor(Epoll_Watcher *r) {
		reactor_ = r;
	}

	void reset(void) {
		io_flag = 0;
		fd_ = 0;
		timer_flag = 0;
		absolute_tv_ = relative_tv_ = Time_Value::zero;
		reactor_ = 0;
		heart_idx_ = 0;
	}

private:
	int io_flag;
	int fd_;

	int timer_flag;
	Time_Value absolute_tv_, relative_tv_;

	Epoll_Watcher *reactor_;

	int heart_idx_;
};


#endif /* EVENT_HANDLER_H_ */
