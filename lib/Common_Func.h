/*
 * Comm_Func.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef COMMON_FUNC_H_
#define COMMON_FUNC_H_

#include <execinfo.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <stdint.h>
#include "Date_Time.h"
#include "Lib_Log.h"

inline void set_nonblock(int fd) {
	int flags = ::fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		LIB_LOG_ERROR("fcntl erro, fd = %d", fd);
		return;
	}
	::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

inline int max_fd(void) {
	struct rlimit rl;
	int ret = 0;

	if ((ret = getrlimit(RLIMIT_NOFILE, &rl)) == -1)
		LIB_LOG_FATAL("getrlimit");
	else
		return rl.rlim_cur;

	return sysconf (_SC_OPEN_MAX);
}

/// 返回大于1.5 * num的第一个素数
inline size_t get_hash_table_size(unsigned int num) {
	size_t return_num = 1.5 * num, i, j;
	while (1) {
		/// 素数判断
		j = (size_t)sqrt(return_num++);
		for (i = 2; i < j; ++i) {
			if (return_num % i == 0)
				break;
		}
		if (i == j)
			return return_num;
	}
	return 0;
}

inline void backstrace_string(std::string &res) {
	static const int backtrace_size = 512;
	int nptrs = 0;
	void *buffer[backtrace_size] = {0};
	char **strings = 0;

	nptrs = backtrace(buffer, backtrace_size);
	strings = backtrace_symbols(buffer, nptrs);
	if (strings) {
		for (int i = 0; i < nptrs; ++i) {
			res.append(strings[i]);
			res.append("\n");
		}
		free(strings);
	}
}

void set_color(int fd, Color color);
void reset_color(int fd);

int elf_hash(const char *str, int len);
void make_session(std::string& account, std::string& session);
int validate_md5(const char *key, const char *account, const char *time, const char *flag);

void set_date_to_day(Date_Time &date_time, int time);
void set_date_time(Date_Time &date_time, int time);
void set_date_to_hour(Date_Time &date_time, int time);

//get_time_zero — 获取明天0点时间戳
int get_time_zero(void);
//获取指定时间的零点时间戳(不是24点)
int get_zero_time(const int sec);
int get_today_zero(void);
int set_time_to_zero(const Time_Value &time_src, Time_Value &time_des);

int get_sunday_time_zero(void);
Time_Value get_week_time(int week, int hour = 0, int minute = 0, int second = 0);
// 获取的是相对时间
Time_Value spec_next_day_relative_time(int hour, int minute, int second);
// 获取的是绝对时间
Time_Value spec_today_absolute_time(unsigned int hour, unsigned int minute, unsigned int second);
Time_Value get_day_begin(const Time_Value &now);
void get_next_cycle_time(const Time_Value &begin, const Time_Value &now, const Time_Value &offset, const Time_Value &cycle, Time_Value &next_time);
void get_date_day_gap(const Time_Value &date1, const Time_Value &date2, int &day);
//get_days_delta:获取两个时间之间相差的天数（此天数以日期的天为单位），即1号的任意钟头和2号的任意钟头都是相差一天
int get_days_delta(Time_Value time1, Time_Value time2);

//数字比较函数
inline int max(int first, int second) { return first >= second ? first : second; }
inline int min(int first, int second) { return first <= second ? first : second; }
inline bool is_double_zero(double value) { return value >= -0.0000001 && value <= 0.0000001; }
inline bool is_double_gt_zero(double value) { return value > 0.0000001; }
inline bool is_double_lt_zero(double value) { return value < -0.0000001; }
//除法
inline int division(int dividend, int divisor) { return !divisor ? 0 : dividend/divisor; }
inline double division(double dividend, double divisor) { return is_double_zero(divisor) ? 0.0 : dividend/divisor; }

//base64编码解码
inline bool is_base64(unsigned char c) { return (isalnum(c) || (c == '+') || (c == '/')); }
std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif /* COMMON_FUNC_H_ */
