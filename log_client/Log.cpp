/*
 * Log.cpp
 *
 *  Created on: May 10, 2012
 *      Author: zhangyalei
 */

#include <stdarg.h>
#include <execinfo.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include "Log.h"
#include "Log_Connector.h"
#include "Block_Buffer.h"
#include "Msg_Define.h"

Log::Log(void):
  log_type_(LOG_MISC),
  log_sub_type_(0),
  msg_time_(2000)
{
	Time_Value now = Time_Value::gettimeofday();
	get_zero_time(now, show_time_);
	show_time_ += Time_Value(3600 + random()%120);
}

Log::~Log(void) { }

int Log::msg_buf_size = 4096;
int Log::backtrace_size = 512;

std::string Log::msg_head[] = {
		"[LOG_TRACE] ",
		"[LOG_DEBUG] ",
		"[LOG_INFO] ",
		"[LOG_WARN] ",
		"[LOG_ERROR] ",
		"[LOG_FATAL] ",
		"[LOG_LOGIN] ",
		"[LOG_GATE] ",
		"[LOG_GAME]",
		"[LOG_MASTER]",
		"[LOG_DB]",
		"[LOG_MISC]"
};

Log *Log::instance_ = 0;
Log *Log::instance(void) {
	if (!instance_)
		instance_ = new Log;
	return instance_;
}

void Log::log_trace(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_TRACE, fmt, ap);
	va_end(ap);
}

void Log::log_debug(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_DEBUG, fmt, ap);
	va_end(ap);
}

void Log::log_info(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_INFO, fmt, ap);
	va_end(ap);
}

void Log::log_warn(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_WARN, fmt, ap);
	va_end(ap);
}

void Log::log_error(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_ERROR, fmt, ap);
	va_end(ap);
}

void Log::log_fatal(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_FATAL, fmt, ap);
	va_end(ap);
}

void Log::assembly_msg(int log_flag, const char *fmt, va_list ap) {
	std::ostringstream msg_stream;

	struct tm tm_v;
	time_t time_v = time(NULL);
	localtime_r(&time_v, &tm_v);
	msg_stream << "(" << (tm_v.tm_hour) << ":" << (tm_v.tm_min) << ":" << (tm_v.tm_sec) << ")";

	msg_stream << msg_head[log_flag];

	char line_buf[msg_buf_size];
	memset(line_buf, 0, sizeof(line_buf));
	vsnprintf(line_buf, sizeof(line_buf), fmt, ap);
	msg_stream << line_buf;

	switch (log_flag) {
	case LOG_TRACE: {
		int nptrs;
		void *buffer[backtrace_size];
		char **strings;

		nptrs = backtrace(buffer, backtrace_size);
		strings = backtrace_symbols(buffer, nptrs);
		if (strings == NULL)
			return ;

		msg_stream << std::endl;
		for (int i = 0; i < nptrs; ++i) {
			msg_stream << (strings[i]) << std::endl;
		}
		free(strings);
		logging_file(msg_stream);
		break;
	}
	case LOG_DEBUG:
	case LOG_INFO:
	case LOG_WARN: {
		msg_stream << std::endl;
		logging_file(msg_stream);
		break;
	}
	case LOG_ERROR: {
		msg_stream << ", errno = " << errno;
		memset(line_buf, 0, sizeof(line_buf));
		strerror_r(errno, line_buf, sizeof(line_buf));
		msg_stream << ", errstr=[" << line_buf << "]" << std::endl;

		logging_file(msg_stream);
		break;
	}
	case LOG_FATAL: {
		msg_stream << "errno = " << errno;
		memset(line_buf, 0, sizeof(line_buf));
		strerror_r(errno, line_buf, sizeof(line_buf));
		msg_stream << ", errstr=[" << line_buf << "]" << std::endl;
		logging_file(msg_stream);
		abort();
		break;
	}
	default: {
		break;
	}
	}

	return ;
}

void Log::logging_file(std::ostringstream &msg_stream) {
#ifdef LOCAL_DEBUG
	std::cerr << msg_stream.str();
#else
	MSG_180000 msg;
	msg.log_type = log_type_;
	msg.log_sub_type = log_sub_type_;
	msg.pid = getpid();
	msg.tid = pthread_self();
	msg.time = time(NULL);
	msg.log_str = msg_stream.str();

	Block_Buffer msg_buf;
	msg_buf.make_inner_message(INNER_LOG_FILE_RECORD);
	msg.serialize(msg_buf);
	msg_buf.finish_message();

	LOG_CONNECTOR->push_data_block(msg_buf);
#endif
}

int Log::logging_db(Block_Buffer &buf) {
	LOG_CONNECTOR->push_data_block(buf);
	return 0;
}

void Log::free_cache(void) {
	LOG_CONNECTOR->free_cache();
}

void Log::msg_time(int msg_id, Time_Value &time) {
	msg_time_[msg_id].add_time(time);
}

void Log::show_msg_time(Time_Value &now) {
	if (now < show_time_) return ;

	show_msg_time();

	show_time_ += Time_Value(86400);
}

struct Msg_Time_Sort_Struct {
	bool operator() (Msg_Process_Time elem1, Msg_Process_Time elem2) const {
		return elem1.tv > elem2.tv;
	}
};

void Log::show_msg_time(void) {
	Msg_Process_Time_Vec msg_time_vec;
	for (Msg_Process_Time_Map::iterator it = msg_time_.begin(); it != msg_time_.end(); ++it) {
		it->second.msg_id = it->first;
		msg_time_vec.push_back(it->second);
	}

	std::sort(msg_time_vec.begin(), msg_time_vec.end(), Msg_Time_Sort_Struct());

	LOG_DEBUG("msg process time start --------------------------------------");
	for (Msg_Process_Time_Vec::iterator it = msg_time_vec.begin(); it != msg_time_vec.end(); ++it) {
		double avg = (*it).times ? (*it).tv.sec()/(*it).times : 0.0;
		LOG_DEBUG("msg process time msg_id = %d, times = %d sec = %ld usec = %ld avg:%ld", (*it).msg_id, (*it).times, (*it).tv.sec(), (*it).tv.usec(), avg);
	}
	LOG_DEBUG("msg process time end --------------------------------------");
}
