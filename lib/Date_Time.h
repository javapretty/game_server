/*
 * DateTime.h
  *
 * 	Created on: Jan 12,2016
 *      Author: zhangyalei
*/

#ifndef DATE_TIME_H_
#define DATE_TIME_H_

#include "Time_Value.h"

class Date_Time {
public:
	Date_Time(void);

	explicit Date_Time(const Time_Value &timevalue);

	Date_Time(long day,
			long month = 0,
			long year = 0,
			long hour = 0,
			long minute = 0,
			long second = 0,
			long microsec = 0,
			long wday = 0);

	/// Update to the current time/date.
	void update(void);

	/// Update to the given Time_Value
	void update(const Time_Value &timevalue);

	long day(void) const;
	void day(long day);

	long month(void) const;
	void month(long month);

	long year(void) const;
	void year(long year);

	long hour(void) const;
	void hour(long hour);

	long minute(void) const;
	void minute(long minute);

	long second(void) const;
	void second(long second);

	long microsec(void) const;
	void microsec(long microsec);

	long weekday(void) const;
	void weekday(long wday);

	time_t time_sec(void);

private:
	long day_;
	long month_;
	long year_;
	long hour_;
	long minute_;
	long second_;
	long microsec_;
	long wday_;
};

#endif /* DATE_TIME_H_ */
