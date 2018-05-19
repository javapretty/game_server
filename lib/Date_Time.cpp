/*
 * DateTime.cpp
  *
 *  Created on: Jan 12,2016
 *      Author: "zhangyalei"
*/

#include "Date_Time.h"

Date_Time::Date_Time(void) : day_(0), month_(0), year_(0), hour_(0), minute_(0), second_(0), microsec_(0), wday_(0) {
	this->update ();
}


Date_Time::Date_Time(const Time_Value &timevalue) : day_(0), month_(0), year_(0), hour_(0), minute_(0), second_(0), microsec_(0), wday_(0){
	this->update (timevalue);
}

Date_Time::Date_Time(long day, long month, long year, long hour, long minute, long second, long microsec, long wday)
 : day_ (day), month_ (month), year_ (year), hour_ (hour), minute_ (minute), second_ (second), microsec_ (microsec), wday_ (wday) {

}

void Date_Time::update(void) {
	update(Time_Value::gettimeofday ());
}

void Date_Time::update(const Time_Value &timevalue) {
	time_t time = timevalue.sec();
	struct tm tm_time;
	::localtime_r(&time, &tm_time);
	this->day_ = tm_time.tm_mday;
	this->month_ = tm_time.tm_mon + 1; // localtime's months are 0-11
	this->year_ = tm_time.tm_year + 1900; // localtime reports years since 1900
	this->hour_ = tm_time.tm_hour;
	this->minute_ = tm_time.tm_min;
	this->second_ = tm_time.tm_sec;
	this->microsec_ = timevalue.usec();
	this->wday_ = tm_time.tm_wday;
}

long Date_Time::day(void) const {
	return day_;
}

void Date_Time::day(long day) {
	day_ = day;
}

long Date_Time::month(void) const {
	return month_;
}

void Date_Time::month(long month) {
	month_ = month;
}

long Date_Time::year(void) const {
	return year_;
}

void Date_Time::year(long year) {
	year_ = year;
}

long Date_Time::hour(void) const {
	return hour_;
}

void Date_Time::hour(long hour) {
	hour_ = hour;
}

long Date_Time::minute(void) const {
	return minute_;
}

void Date_Time::minute(long minute) {
	minute_ = minute;
}

long Date_Time::second(void) const {
	return second_;
}

void Date_Time::second(long second) {
	second_ = second;
}

long Date_Time::microsec(void) const {
	return microsec_;
}

void Date_Time::microsec(long microsec) {
	microsec_ = microsec;
}

long Date_Time::weekday(void) const {
	return wday_;
}

void Date_Time::weekday(long wday) {
	wday_ = wday;
}

time_t Date_Time::time_sec(void) {
	struct tm t;
	t.tm_year = year_ - 1900;
	t.tm_mon = month_ - 1;
	t.tm_mday = day_;
	t.tm_hour = hour_;
	t.tm_min = minute_;
	t.tm_sec = second_;
	t.tm_isdst = 0;
	return mktime(&t);
}
