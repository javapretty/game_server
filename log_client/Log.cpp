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

Log::Log(void)
: switcher_(F_SYS_ABORT | F_ABORT | F_EXIT | F_SYS | F_USER | F_USER_TRACE | F_DEBUG),
  log_type_(LOG_MISC),
  log_sub_type_(0),
  msg_time_(2000)
{
	Time_Value now = Time_Value::gettimeofday();
	get_zero_time(now, show_time_);
	show_time_ += Time_Value(3600 + random()%120);
}

Log::~Log(void) { }

Log *Log::instance_;

int Log::msg_buf_size = 4096;

std::string Log::msg_head[] = {
		"[MSG_SYS_ABORT] ",			/// M_SYS_ABORT 	= 0,
		"[MSG_ABORT] ",					/// M_ABORT 			= 1,
		"[MSG_EXIT] ",					/// M_EXIT 			= 2,
		"[MSG_SYS] ",						/// M_SYS 				= 3,
		"[MSG_USER] ",					/// M_USER				= 4,
		"[MSG_USER_TRACE] ",		/// M_USER_TRACE	= 5,
		"[MSG_DEBUG] ",					/// M_DEBUG			= 6,
		"[LOGIN] ",						/// LOG_LOGIN			= 7,
		"[GATE] ",							/// LOG_GATE			= 8,
		"[GAME]",							/// LOG_GAME			= 9,
		"[MASTER]",						/// LOG_MASTER		= 10,
		"[DB]"									/// LOG_DB				= 11,
		"[MISC]"								/// LOG_MISC			= 12
};

Log *Log::instance(void) {
	if (! instance_)
		instance_ = new Log;
	return instance_;
}

int Log::backtrace_size = 512;

void Log::msg_abort(const char *fmt, ...) {
	va_list	ap;

	va_start(ap, fmt);
	assembly_msg(M_ABORT, fmt, ap);
	va_end(ap);
}

void Log::msg_sys_abort(const char *fmt, ...) {
	if (switcher_ & F_SYS_ABORT) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_SYS_ABORT, fmt, ap);
		va_end(ap);
	}
}

void Log::msg_exit(const char *fmt, ...) {
	if (switcher_ & F_EXIT) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_EXIT, fmt, ap);
		va_end(ap);
	}
}

void Log::msg_sys(const char *fmt, ...) {
	if (switcher_ & F_SYS) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_SYS, fmt, ap);
		va_end(ap);
	}
}

void Log::msg_user(const char *fmt, ...) {
	if (switcher_ & F_USER) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_USER, fmt, ap);
		va_end(ap);
	}
}

void Log::msg_user_trace(const char *fmt, ...) {
	if (switcher_ & F_USER_TRACE) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_USER_TRACE, fmt, ap);
		va_end(ap);
	}
}

void Log::msg_debug(const char *fmt, ...) {
	if (switcher_ & F_DEBUG) {
		va_list	ap;

		va_start(ap, fmt);
		assembly_msg(M_DEBUG, fmt, ap);
		va_end(ap);
	}
}

void Log::assembly_msg(int log_flag, const char *fmt, va_list ap) {
	std::ostringstream msg_stream;

	struct tm tm_v;
	time_t time_v = time(NULL);

	localtime_r(&time_v, &tm_v);

#ifndef LOCAL_DEBUG
	msg_stream << "<pid=" << (int)getpid() << "|tid=" << pthread_self()
			<< ">(" << (tm_v.tm_hour) << ":" << (tm_v.tm_min) << ":" << (tm_v.tm_sec) << ")";
#endif

	msg_stream << msg_head[log_flag];

	char line_buf[msg_buf_size];
	memset(line_buf, 0, sizeof(line_buf));
	vsnprintf(line_buf, sizeof(line_buf), fmt, ap);

	msg_stream << line_buf;

	switch (log_flag) {
	case M_SYS_ABORT: {
		msg_stream << "errno = " << errno;

		memset(line_buf, 0, sizeof(line_buf));
		strerror_r(errno, line_buf, sizeof(line_buf));
		msg_stream << ", errstr=[" << line_buf << "]" << std::endl;
		logging_file(msg_stream);
		abort();

		break;
	}
	case M_ABORT: {
		msg_stream << std::endl;
		logging_file(msg_stream);
		abort();

		break;
	}
	case M_EXIT: {
		msg_stream << std::endl;
		logging_file(msg_stream);
		exit(1);

		break;
	}
	case M_SYS: {
		msg_stream << ", errno = " << errno;

		memset(line_buf, 0, sizeof(line_buf));
		strerror_r(errno, line_buf, sizeof(line_buf));
		msg_stream << ", errstr=[" << line_buf << "]" << std::endl;

		logging_file(msg_stream);

		break;
	}
	case M_USER: {
		msg_stream << std::endl;
		logging_file(msg_stream);

		break;
	}
	case M_USER_TRACE: {
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
	case M_DEBUG: {
		msg_stream << std::endl;
		logging_file(msg_stream);

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

	MSG_DEBUG("msg process time start --------------------------------------");
	for (Msg_Process_Time_Vec::iterator it = msg_time_vec.begin(); it != msg_time_vec.end(); ++it) {
		double avg = (*it).times ? (*it).tv.sec()/(*it).times : 0.0;
		MSG_DEBUG("msg process time msg_id = %d, times = %d sec = %ld usec = %ld avg:%ld", (*it).msg_id, (*it).times, (*it).tv.sec(), (*it).tv.usec(), avg);
	}
	MSG_DEBUG("msg process time end --------------------------------------");
}
