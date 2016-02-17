/*
 * Time_Value.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef TIME_VALUE_H_
#define TIME_VALUE_H_

#include <string>
#include <time.h>
#include <sys/time.h>
#include <cstdlib>

class Block_Buffer;

class Time_Value {
public:
	suseconds_t const static ONE_SECOND_IN_USECS = 1000000;
	long int const static ONE_MINUTE_IN_SECS = 60;
	long int const static ONE_HOURS_MINUTE = 60;
	long int const static ONE_HOUR_IN_SECS = (ONE_MINUTE_IN_SECS * ONE_HOURS_MINUTE);
	long int const static ONE_DAY_HOURS = 24;
	long int const static ONE_DAY_IN_SECS = (ONE_HOUR_IN_SECS * ONE_DAY_HOURS);
	long int const static ONE_WEEK_DAYS = 7;
	long int const static ONE_WEEK_IN_SECS = (ONE_DAY_IN_SECS * ONE_WEEK_DAYS);

	friend bool operator > (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator < (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator >= (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator <= (const Time_Value &tv1, const Time_Value &tv2);
	friend Time_Value operator - (const Time_Value &tv1, const Time_Value &tv2);
	friend Time_Value operator + (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator == (const Time_Value &tv1, const Time_Value &tv2);
	friend bool operator != (const Time_Value &tv1, const Time_Value &tv2);

public:
	static const Time_Value zero;
	static const Time_Value max;
	static Time_Value gettimeofday();
	static int sleep(const Time_Value &tv);

	explicit Time_Value(long sec = 0, long usec = 0);
	explicit Time_Value(const struct timeval &t);
	Time_Value(const Time_Value &tv);

	void set(long sec, long usec) {	this->tv_.tv_sec = sec; this->tv_.tv_usec = usec; }
	inline void set(const timeval &t) { this->tv_ = t; }
	inline void set(const Time_Value &tv) { this->tv_ = tv.get_tv(); }

	inline long sec(void) const { return this->tv_.tv_sec; }
	inline void sec(long sec)  { this->tv_.tv_sec = sec; }
	inline long usec(void) const  { return this->tv_.tv_usec; }
	inline void usec(long usec)  { this->tv_.tv_usec = usec; }

	inline long msec(void) const {return this->tv_.tv_sec * 1000 + this->tv_.tv_usec / 1000;}

	inline const timeval & get_tv(void) const { return this->tv_;	}

	Time_Value & operator += (const Time_Value &tv);
	Time_Value & operator -= (const Time_Value &tv);
	operator timespec() const;

	void normalize(void);

	void debug_dump(void) const;
	void debug_dump(char *str, size_t *sl);

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);

private:
	timeval tv_;
};

inline bool is_same_week(const Time_Value &tv1, const Time_Value &tv2) {
	tm tm1, tm2;
	time_t t1 = tv1.sec(), t2 = tv2.sec();

	if (std::abs(t1 - t2) > Time_Value::ONE_WEEK_IN_SECS) {
		return false;
	}

	if (tv1 > tv2) {
		std::swap(t1, t2);
	}

	localtime_r(&t1, &tm1);
	localtime_r(&t2, &tm2);

	if (tm1.tm_wday < tm2.tm_wday) {
		return true;
	} else if (tm1.tm_wday == tm2.tm_wday) {
		return tm1.tm_yday == tm2.tm_yday;
	}

	return false;
}

inline bool is_same_month(const Time_Value &tv1, const Time_Value &tv2) {
	tm tm1, tm2;
	time_t t1 = tv1.sec(), t2 = tv2.sec();

	localtime_r(&t1, &tm1);
	localtime_r(&t2, &tm2);

	if (tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon)
		return true;
	else
		return false;
}

inline bool is_same_day(const Time_Value &tv1, const Time_Value &tv2) {
	tm tm1, tm2;
	time_t t1 = tv1.sec(), t2 = tv2.sec();

	localtime_r(&t1, &tm1);
	localtime_r(&t2, &tm2);

	if (tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon && tm1.tm_mday == tm2.tm_mday)
		return true;
	else
		return false;
}

inline bool is_same_hour(const Time_Value &tv1, const Time_Value &tv2) {
	tm tm1, tm2;
	time_t t1 = tv1.sec(), t2 = tv2.sec();

	localtime_r(&t1, &tm1);
	localtime_r(&t2, &tm2);

	if (tm1.tm_year == tm2.tm_year
			&& tm1.tm_mon == tm2.tm_mon
			&& tm1.tm_mday == tm2.tm_mday
			&& tm1.tm_hour == tm2.tm_hour)
		return true;
	else
		return false;
}

inline void get_zero_time(const Time_Value &tv1, Time_Value &tv2) {
	int zeor_time = tv1.sec() + (86400 - (tv1.sec() + 28800) % 86400);
	tv2.set(zeor_time, 0);
}

/// tv2和tv1相隔天数
inline int get_time_subdays(const Time_Value &tv1, const Time_Value &tv2) {
	int zero_time1 = tv1.sec() + (86400 - (tv1.sec() + 28800) % 86400);
	int zero_time2 = tv2.sec() + (86400 - (tv2.sec() + 28800) % 86400);
	return (zero_time2 - zero_time1) / 86400;
}

#endif /* TIME_VALUE_H_ */
