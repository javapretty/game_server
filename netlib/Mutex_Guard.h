/*
 * Mutex_Guard.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef MUTEX_GUARD_H_
#define MUTEX_GUARD_H_

#include "Thread_Mutex.h"

template <typename LOCK>
class Mutex_Guard {
public:
	Mutex_Guard(LOCK &l);
	virtual ~Mutex_Guard();

	int acquire(void);
	int release(void);

	int acquire_read(void);
	int acquire_write(void);

private:
	LOCK *lock_;
};

////////////////////////////////////////////////////////////////

template <typename LOCK>
class Read_Guard {
public:
	Read_Guard(LOCK &l);
	virtual ~Read_Guard();

	int release(void);
	int acquire_read(void);

private:
	LOCK *lock_;
};

template <typename LOCK>
Read_Guard<LOCK>::Read_Guard(LOCK &l) : lock_(&l) {
	acquire_read();
}

template <typename LOCK>
Read_Guard<LOCK>::~Read_Guard(void) { this->release(); }

template <typename LOCK>
int Read_Guard<LOCK>::release(void) { return lock_->release(); }

template <typename LOCK>
int Read_Guard<LOCK>::acquire_read(void) { return lock_->acquire_read(); }

template <typename LOCK>
class Write_Guard {
public:
	Write_Guard(LOCK &l);
	virtual ~Write_Guard();

	int release(void);
	int acquire_write(void);

private:
	LOCK *lock_;
};

template <typename LOCK>
Write_Guard<LOCK>::Write_Guard(LOCK &l) : lock_(&l) {
	acquire_write();
}

template <typename LOCK>
Write_Guard<LOCK>::~Write_Guard(void) { this->release(); }

template <typename LOCK>
int Write_Guard<LOCK>::release(void) { return lock_->release(); }

template <typename LOCK>
int Write_Guard<LOCK>::acquire_write(void) { return lock_->acquire_write(); }

////////////////////////////////////////////////////////////////

template <typename LOCK>
Mutex_Guard<LOCK>::Mutex_Guard(LOCK &l) : lock_(&l) {
	//lock_ = &l;
	this->acquire();
}

template <typename LOCK>
Mutex_Guard<LOCK>::~Mutex_Guard(void) {
	this->release();
}

template <typename LOCK>
int Mutex_Guard<LOCK>::acquire(void) {
	return lock_->acquire();
}

template <typename LOCK>
int Mutex_Guard<LOCK>::release(void) {
	return lock_->release();
}

template <typename LOCK>
int Mutex_Guard<LOCK>::acquire_read(void) {
	return lock_->acquire_read();
}

template <typename LOCK>
int Mutex_Guard<LOCK>::acquire_write(void) {
	return lock_->acquire_write();
}

////////////////////////////////////////////////////////////////

#define GUARD(MUTEX, OBJ, LOCK) Mutex_Guard< MUTEX > OBJ(LOCK);
#define GUARD_READ(MUTEX, OBJ, LOCK) Read_Guard< MUTEX > OBJ(LOCK);
#define GUARD_WRITE(MUTEX, OBJ, LOCK) Write_Guard< MUTEX > OBJ(LOCK);

#define REACTOR_MUTEX MUTEX

#endif /* MUTEX_GUARD_H_ */
