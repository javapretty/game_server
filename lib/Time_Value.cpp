/*
 * Time_Value.cpp
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#include <limits.h>
#include "Time_Value.h"
#include "Date_Time.h"
#include "Block_Buffer.h"

const Time_Value Time_Value::zero;
const Time_Value Time_Value::max(LONG_MAX, 0);

Time_Value Time_Value::gettimeofday(void) {
	timeval now_t;
	::gettimeofday(&now_t, NULL);
	Time_Value now_tv(now_t);

	return now_tv;
}

int Time_Value::sleep(const Time_Value &tv) {
	timespec ts = tv;
	return ::nanosleep (&ts, 0);
}

Time_Value::Time_Value(long sec, long usec) {
	set(sec, usec);
}

Time_Value::Time_Value(const struct timeval &t) {
	set(t);
}

Time_Value::Time_Value(const Time_Value &tv) {
	set(tv);
}

void Time_Value::debug_dump(void) const {
/*	char str[512];
	memset(str, 0, sizeof(str));
	snprintf(str, sizeof(str), "sec = [%d], usec = [%d]", (int)this->tv_.tv_sec, (int)this->tv_.tv_usec);
	std::cerr << str;*/
}

void Time_Value::debug_dump(char *str, size_t *sl) {
	memset(str, 0, *sl);
	snprintf(str, *sl, "%d-%d", (int)this->tv_.tv_sec, (int)this->tv_.tv_usec);
}

void Time_Value::normalize(void) {
	if (this->tv_.tv_usec >= ONE_SECOND_IN_USECS) {
		do {
			++this->tv_.tv_sec;
			this->tv_.tv_usec -= ONE_SECOND_IN_USECS;
		} while (this->tv_.tv_usec >= ONE_SECOND_IN_USECS);
	} else if (this->tv_.tv_usec <= -ONE_SECOND_IN_USECS) {
		do {
			--this->tv_.tv_sec;
			this->tv_.tv_usec += ONE_SECOND_IN_USECS;
		} while (this->tv_.tv_usec <= -ONE_SECOND_IN_USECS);
	}

	if (this->tv_.tv_sec >= 1 && this->tv_.tv_usec < 0) {
		--this->tv_.tv_sec;
		this->tv_.tv_usec += ONE_SECOND_IN_USECS;
	}
}

bool operator > (const Time_Value &tv1, const Time_Value &tv2) {
	if (tv1.sec() > tv2.sec())
		return 1;
	else if (tv1.sec() == tv2.sec() && tv1.usec() > tv2.usec())
		return 1;
	else
		return 0;
}

bool operator < (const Time_Value &tv1, const Time_Value &tv2) {
  return tv2 > tv1;
}

bool operator >= (const Time_Value &tv1, const Time_Value &tv2) {
	if (tv1.sec() > tv2.sec())
		return 1;
	else if (tv1.sec() == tv2.sec() && tv1.usec() >= tv2.usec())
		return 1;
	else
		return 0;
}

bool operator <= (const Time_Value &tv1, const Time_Value &tv2) {
  return tv2 >= tv1;
}

Time_Value operator - (const Time_Value &tv1, const Time_Value &tv2) {
	Time_Value delta(tv1);
	delta -= tv2;

	return delta;
}

Time_Value operator + (const Time_Value &tv1, const Time_Value &tv2) {
	Time_Value sum (tv1);
	sum += tv2;

	return sum;
}

Time_Value &Time_Value::operator -= (const Time_Value &tv) {
	this->sec (this->sec () - tv.sec ());
	this->usec (this->usec () - tv.usec ());
	this->normalize ();
	return *this;
}

Time_Value &Time_Value::operator += (const Time_Value &tv) {
	this->sec (this->sec () + tv.sec ());
	this->usec (this->usec () + tv.usec ());
	this->normalize ();
	return *this;
}

bool operator == (const Time_Value &tv1, const Time_Value &tv2) {
	return tv1.sec() == tv2.sec() && tv1.usec() == tv2.usec();
}

bool operator != (const Time_Value &tv1, const Time_Value &tv2) {
	return !(tv1 == tv2);
}

Time_Value::operator timespec() const {
	timespec tv;
	tv.tv_sec = this->sec ();
	tv.tv_nsec = this->tv_.tv_usec * 1000;
	return tv;
}

int Time_Value::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(tv_.tv_sec);
	buffer.write_int64(tv_.tv_usec);
	return 0;
}

int Time_Value::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(tv_.tv_sec);
	buffer.read_int64(tv_.tv_usec);
	return 0;
}
