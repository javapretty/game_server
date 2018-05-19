/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <openssl/md5.h>

void set_color(int fd, Color color) {
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "\x1b[%d%sm",
			color >= LRED ? (color - 10) : color,
			color >= LRED ? ";1" : ""
			);
	write(fd, buffer, strlen(buffer));
}

void reset_color(int fd) {
	const char* s = "\x1b[0m";
	write(fd, s, strlen(s));
}

int elf_hash(const char *str, int len) {
	int hash = 0, x = 0;

	for(int i = 0; i < len; ++i) {
		hash = (hash << 4) + str[i];
		if((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}

	return hash;
}

void make_session(std::string& account, std::string& session){
	long timesamp = Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec();
	int hash = elf_hash(account.c_str(), account.size());
	int rand = random() % hash;

	std::stringstream stremsession;
	stremsession << timesamp;
	stremsession << hash;
	stremsession << rand;

	session =  stremsession.str();
}

int validate_md5(const char *key, const char *account, const char *time, const char *session) {
	char mine_src[256 + 1], mine_md5[256 + 1];
	memset(mine_src, 0x00, 256 + 1);
	memset(mine_md5, 0x00, 256 + 1);

	snprintf(mine_src, sizeof(mine_src), "%s%s%s", account, time, key);
	const unsigned char *tmp_md5 = MD5((const unsigned char *) mine_src, strlen(mine_src), 0);

	for (uint i = 0; i < 16; i++) {
		sprintf(&mine_md5[i * 2], "%.2x", tmp_md5[i]);
    }

	return strncmp(session, mine_md5, strlen(session));
}

void set_date_to_day(Date_Time &date_time, int time) {
	date_time.year(time / 10000);
	time = time % 10000;
	date_time.month(time / 100);
	time = time % 100;
	date_time.day(time);
	date_time.hour(0);
	date_time.minute(0);
	date_time.second(0);
}

void set_date_time(Date_Time &date_time, int time) {
	int int_tmp1, int_tmp2, int_tmp3;
	int_tmp1 = time;
	int_tmp3 = int_tmp1 % 100;
	int_tmp1 = int_tmp1 / 100;
	int_tmp2 = int_tmp1 % 100;
	int_tmp1 = int_tmp1 / 100;
	date_time.hour(int_tmp1);
	date_time.minute(int_tmp2);
	date_time.second(int_tmp3);
}

void set_date_to_hour(Date_Time &date_time, int time) {
	date_time.year(time / 1000000);
	time = time % 1000000;
	date_time.month(time / 10000);
	time = time % 10000;
	date_time.day(time / 100);
	date_time.hour(time % 100);
	date_time.minute(0);
	date_time.second(0);
}

int get_time_zero(void) {
	Date_Time date_time;
	date_time.hour(0);
	date_time.minute(0);
	date_time.second(0);

	return date_time.time_sec() + 86400;
}

int get_time_zero(const int sec) {
	int zeor_time = sec - (sec + 28800) % 86400;
	return zeor_time;
}

int get_today_zero(void) {
	Date_Time date_time;
	date_time.hour(0);
	date_time.minute(0);
	date_time.second(0);

	return date_time.time_sec();
}

int set_time_to_zero(const Time_Value &time_src, Time_Value &time_des) {
	Date_Time date_tmp(time_src);
	date_tmp.hour(0);
	date_tmp.minute(0);
	date_tmp.second(0);
	time_des.sec(date_tmp.time_sec());
	return 0;
}

int get_sunday_time_zero(void) {
	Time_Value time = get_week_time(7, 23, 59, 59);
	return time.sec();
}

Time_Value get_week_time(int week, int hour, int minute, int second) {
	Time_Value time(Time_Value::gettimeofday());
	Date_Time date_now(time);
	int day_gap = 0;
	int weekday = date_now.weekday();
	if (weekday > 0) {
		day_gap = week - weekday;
	}
	int hour_gap = hour - date_now.hour();
	int minute_gap = minute - date_now.minute();
	int second_gap = second - date_now.second();

	int sec = time.sec();
	int rel_sec = 0;
	rel_sec += day_gap*Time_Value::ONE_DAY_IN_SECS;
	rel_sec += hour_gap*Time_Value::ONE_HOUR_IN_SECS;
	rel_sec += minute_gap*Time_Value::ONE_MINUTE_IN_SECS;
	rel_sec += second_gap;
	if (rel_sec <= 0) {
		rel_sec += Time_Value::ONE_DAY_IN_SECS * 7;
	}

	time.set(sec + rel_sec, 0);
	return time;
}

Time_Value spec_next_day_relative_time(int hour, int minute, int second) {
	Time_Value time(Time_Value::gettimeofday());
	Date_Time date_now(time);
	int hour_gap = hour - date_now.hour();
	int minute_gap = minute - date_now.minute();
	int second_gap = second - date_now.second();

	int sec = 0;
	sec += hour_gap * Time_Value::ONE_HOUR_IN_SECS;
	sec += minute_gap * Time_Value::ONE_MINUTE_IN_SECS;
	sec += second_gap;

	if (sec <= 0) {
		sec += Time_Value::ONE_DAY_IN_SECS;
	}

	time.set(sec, 0);
	return time;
}

Time_Value spec_today_absolute_time(unsigned int hour, unsigned int minute, unsigned int second) {
	Time_Value time(Time_Value::gettimeofday());
	Date_Time date_now(time);
	int hour_gap = hour - date_now.hour();
	int minute_gap = minute - date_now.minute();
	int second_gap = second - date_now.second();

	int sec = 0;
	sec += hour_gap * Time_Value::ONE_HOUR_IN_SECS;
	sec += minute_gap * Time_Value::ONE_MINUTE_IN_SECS;
	sec += second_gap;
	sec += time.sec();

	time.set(sec, 0);

	return time;
}

Time_Value get_day_begin(const Time_Value &now) {
	Date_Time date_now(now);
	date_now.hour(0);
	date_now.minute(0);
	date_now.second(0);
	date_now.microsec(0);
	Time_Value day_begin(date_now.time_sec(), 0);
	return day_begin;
}

void get_date_day_gap(const Time_Value &date1, const Time_Value &date2, int &day) {
	Time_Value local_date1 = get_day_begin(date1);
	Time_Value local_date2 = get_day_begin(date2);
	int64_t sec_gap = local_date2.sec() - local_date1.sec();
	day = sec_gap / Time_Value::ONE_DAY_IN_SECS;
}

void get_next_cycle_time(const Time_Value &begin, const Time_Value &now, const Time_Value &offset,
		const Time_Value &cycle, Time_Value &next_time) {
	Time_Value first_begin_time = begin + offset;
	if (now > first_begin_time) {
		if (cycle <= Time_Value::zero) {
			next_time = Time_Value::max;
			return;
		}
		int cycle_sec = cycle.sec();
		int sec_gap = now.sec() - first_begin_time.sec();
		int cycle_nums = sec_gap / cycle_sec;
		++cycle_nums;
		next_time.set(first_begin_time.sec() + cycle.sec() * cycle_nums, 0);
	} else {
		next_time = first_begin_time;
	}
}

int get_days_delta(Time_Value time1, Time_Value time2) {
	Time_Value time_tmp;
	if (time1 < time2) {
		time_tmp = time1;
		time1 = time2;
		time2 = time_tmp;
	}

	Date_Time date1(time1), date2(time2);
	date1.hour(0);
	date1.minute(0);
	date1.second(0);
	date2.hour(0);
	date2.minute(0);
	date2.second(0);

	return (date1.time_sec() - date2.time_sec()) / Time_Value::ONE_DAY_IN_SECS;
}

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  size_t in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
