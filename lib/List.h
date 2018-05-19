/*
 * List.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef LIST_H_
#define LIST_H_

#include <list>
#include "Mutex_Guard.h"

template <typename Obj, typename LOCK = NULL_MUTEX>
class List {
public:
	typedef std::list<Obj> TList;
	typedef typename TList::const_iterator const_iterator;

	List(void);
	~List(void);

	void push_back(const Obj &v);
	Obj &front();
	Obj pop_front();

	bool empty();
	void clear();
	size_t size() const;
	size_t size();

	const_iterator begin() const;
	const_iterator end() const;

	const std::list<Obj> &get_list();
	const std::list<Obj> &get_list() const;

private:
	TList list_;
	size_t size_;
	LOCK lock_;
};

////////////////////////////////////////////////////////////////////////////////

template <typename Obj, typename LOCK>
List<Obj, LOCK>::List(void) : size_(0) { }

template <typename Obj, typename LOCK>
List<Obj, LOCK>::~List(void) { }

template <typename Obj, typename LOCK>
void List<Obj, LOCK>::push_back(const Obj &v) {
	GUARD(LOCK, mon, this->lock_);
	++size_;
	list_.push_back(v);
	return ;
}

template <typename Obj, typename LOCK>
Obj &List<Obj, LOCK>::front() {
	GUARD(LOCK, mon, this->lock_);
	return list_.front();
}

template <typename Obj, typename LOCK>
Obj List<Obj, LOCK>::pop_front() {
	GUARD(LOCK, mon, this->lock_);
	Obj val = list_.front();
	--size_;
	list_.pop_front();
	return val;
}

template <typename Obj, typename LOCK>
bool List<Obj, LOCK>::empty() {
	GUARD(LOCK, mon, this->lock_);
	return list_.empty();
}

template <typename Obj, typename LOCK>
void List<Obj, LOCK>::clear() {
	GUARD(LOCK, mon, this->lock_);
	size_ = 0;
	list_.clear();
	return ;
}

template <typename Obj, typename LOCK>
size_t List<Obj, LOCK>::size() const {
	GUARD(LOCK, mon, this->lock_);
	return size_;
}

template <typename Obj, typename LOCK>
size_t List<Obj, LOCK>::size() {
	GUARD(LOCK, mon, this->lock_);
	return size_;
}

template <typename Obj, typename LOCK>
const std::list<Obj> &List<Obj, LOCK>::get_list() {
	GUARD(LOCK, mon, this->lock_);
	return list_;
}

template <typename Obj, typename LOCK>
const std::list<Obj> &List<Obj, LOCK>::get_list() const {
	GUARD(LOCK, mon, this->lock_);
	return list_;
}

template <typename Obj, typename LOCK>
typename List<Obj, LOCK>::const_iterator List<Obj, LOCK>::begin() const {
	return list_.begin();
}

template <typename Obj, typename LOCK>
typename List<Obj, LOCK>::const_iterator List<Obj, LOCK>::end() const {
	return list_.end();
}

#endif /* LIST_H_ */
