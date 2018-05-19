/*
 * Thread_Mutex.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 *
 *  	非递归线程锁
 */

#ifndef THREAD_MUTEX_H_
#define THREAD_MUTEX_H_

#include <pthread.h>

class Null_Mutex {
public:
	Null_Mutex(void) { }

	virtual ~Null_Mutex(void) { }

	int acquire(void) {
		return 0;
	}

	int release(void) {
		return 0;
	}

	int acquire_read(void) {
		return 0;
	}

	int acquire_write(void) {
		return 0;
	}

private:
	  void operator=(const Null_Mutex &);
	  Null_Mutex(const Null_Mutex &);
};

///////////////////////////////////////////

class Thread_Mutex {
public:
	Thread_Mutex(void) {
		::pthread_mutex_init(&lock_, NULL);
	}

	virtual ~Thread_Mutex(void) {
		::pthread_mutex_destroy(&lock_);
	}

	int acquire(void) {
		return ::pthread_mutex_lock(&lock_);
	}

	int release(void) {
		return ::pthread_mutex_unlock(&lock_);
	}

	int acquire_read(void) {
		return this->acquire();
	}

	int acquire_write(void) {
		return this->acquire();
	}

private:
	  void operator=(const Thread_Mutex &);
	  Thread_Mutex(const Thread_Mutex &);

private:
	pthread_mutex_t lock_;
};

///////////////////////////////////////////

class RW_Mutex {
public:
	RW_Mutex(void) {
		::pthread_rwlock_init(&lock_, NULL);
	}

	virtual ~RW_Mutex(void) {
		::pthread_rwlock_destroy(&lock_);
	}

	int acquire(void) {
		return ::pthread_rwlock_wrlock(&lock_);
	}

	int release(void) {
		return ::pthread_rwlock_unlock(&lock_);
	}

	int acquire_read(void) {
		return ::pthread_rwlock_rdlock(&lock_);
	}

	int acquire_write(void) {
		return ::pthread_rwlock_wrlock(&lock_);
	}

private:
	void operator=(const RW_Mutex &);
	RW_Mutex(const RW_Mutex &);

private:
	pthread_rwlock_t lock_;
};

///////////////////////////////////////////

class RE_Mutex {
public:
	RE_Mutex(void) {
		::pthread_mutexattr_init(&attr);
		::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		::pthread_mutex_init(&lock_, &attr);
	}

	virtual ~RE_Mutex(void) {
		::pthread_mutex_destroy(&lock_);
		::pthread_mutexattr_destroy(&attr);
	}

	int acquire(void) {
		return ::pthread_mutex_lock(&lock_);
	}

	int release(void) {
		return ::pthread_mutex_unlock(&lock_);
	}

	int acquire_read(void) {
		return this->acquire();
	}

	int acquire_write(void) {
		return this->acquire();
	}

private:
	void operator=(const RE_Mutex &);
	RE_Mutex(const RE_Mutex &);

private:
	pthread_mutexattr_t attr;
	pthread_mutex_t lock_;
};

///////////////////////////////////////////

class Spin_Lock {
public:
	Spin_Lock(void) {
		::pthread_spin_init(&lock_, 0);
	}

	virtual ~Spin_Lock(void) {
		::pthread_spin_destroy(&lock_);
	}

	int acquire(void) {
		return ::pthread_spin_lock(&lock_);
	}

	int release(void) {
		return ::pthread_spin_unlock(&lock_);
	}

	int acquire_read(void) {
		return this->acquire();
	}

	int acquire_write(void) {
		return this->acquire();
	}

private:
	void operator=(const RE_Mutex &);
	Spin_Lock(const Spin_Lock &);

private:
	pthread_spinlock_t lock_;
};

///////////////////////////////////////////
class Thread_Notify
{
public:
	Thread_Notify(){
		::pthread_mutexattr_init(&mutexattr_);
		::pthread_mutexattr_settype(&mutexattr_, PTHREAD_MUTEX_RECURSIVE);
		::pthread_mutex_init(&mutex_, &mutexattr_);
		::pthread_cond_init(&cond_, NULL);
	}

	~Thread_Notify(){
		::pthread_mutexattr_destroy(&mutexattr_);
		::pthread_mutex_destroy(&mutex_);
		::pthread_cond_destroy(&cond_);
	}

	void lock() { pthread_mutex_lock(&mutex_); }
	void unlock() { pthread_mutex_unlock(&mutex_); }
	void wait() { pthread_cond_wait(&cond_, &mutex_); }
	void signal() { pthread_cond_signal(&cond_); }

private:
	pthread_mutex_t mutex_;
	pthread_mutexattr_t	mutexattr_;
	pthread_cond_t cond_;
};
//////////////////////////////////////////

#define NULL_MUTEX Null_Mutex
#define RW_MUTEX RW_Mutex
#define RE_MUTEX RE_Mutex
#define MUTEX Thread_Mutex

#endif /* THREAD_MUTEX_H_ */
