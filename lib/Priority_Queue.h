/*
 * Priority_Queue.h
 *
 *  Created on: Mar 22,2016
 *      Author: zhangyalei
 */

#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

#include "Mutex_Guard.h"

//优先队列
template <typename Obj, typename Compare, typename LOCK = NULL_MUTEX>
class Priority_Queue {
public:
	typedef std::vector<Obj> Container;

	Obj top(){
		GUARD(LOCK, mon, this->lock_);
		return container_.front();
	}
	void push(Obj obj){
		GUARD(LOCK, mon, this->lock_);
		container_.push_back(obj);
		std::push_heap(container_.begin(), container_.end(), Compare());
	}
	void pop(){
		GUARD(LOCK, mon, this->lock_);
		std::pop_heap(container_.begin(), container_.end(), Compare());
		container_.pop_back();
	}
	bool empty(){
		GUARD(LOCK, mon, this->lock_);
		return container_.empty();
	}
private:
	Container container_;
	LOCK lock_;
};

#endif /* PRIORITY_QUEUE_H_ */
