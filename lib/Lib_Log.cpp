/*
 * Lib_Log.cpp
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#include <stdarg.h>
#include <execinfo.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Lib_Log.h"
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"
#include "Common_Func.h"

Lib_Log::Lib_Log(void) : file_switcher_(false) { }

Lib_Log::~Lib_Log(void) { }

int Lib_Log::msg_buf_size = 4096;
int Lib_Log::backtrace_size = 512;
std::string Lib_Log::lib_log_dir = "./lib_log";

std::string Lib_Log::msg_head[] = {
		"[LIB_LOG_TRACE] ",
		"[LIB_LOG_DEBUG] ",
		"[LIB_LOG_INFO] ",
		"[LIB_LOG_WARN] ",
		"[LIB_LOG_ERROR] ",
		"[LIB_LOG_FATAL] "
};

Lib_Log *Lib_Log::instance_ = 0;
Lib_Log *Lib_Log::instance(void) {
	if (!instance_)
		instance_ = new Lib_Log;
	return instance_;
}

void Lib_Log::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Lib_Log::log_trace(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_TRACE, fmt, ap);
	va_end(ap);
}

void Lib_Log::log_debug(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_DEBUG, fmt, ap);
	va_end(ap);
}

void Lib_Log::log_info(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_INFO, fmt, ap);
	va_end(ap);
}

void Lib_Log::log_warn(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_WARN, fmt, ap);
	va_end(ap);
}

void Lib_Log::log_error(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_ERROR, fmt, ap);
	va_end(ap);
}

void Lib_Log::log_fatal(const char *fmt, ...) {
	va_list	ap;
	va_start(ap, fmt);
	assembly_msg(LOG_FATAL, fmt, ap);
	va_end(ap);
}

void Lib_Log::assembly_msg(int log_flag, const char *fmt, va_list ap) {
	std::ostringstream msg_stream;

	struct tm tm_v;
	time_t time_v = time(NULL);
	localtime_r(&time_v, &tm_v);
	msg_stream << "<pid=" << (int)getpid() << "|tid=" << pthread_self()
			<< ">(" << (tm_v.tm_hour) << ":" << (tm_v.tm_min) << ":" << (tm_v.tm_sec) << ")";

	msg_stream << msg_head[log_flag];

	char line_buf[msg_buf_size];
	memset(line_buf, 0, sizeof(line_buf));
	vsnprintf(line_buf, sizeof(line_buf), fmt, ap);
	msg_stream << line_buf;

	switch (log_flag) {
	case LOG_TRACE: {
		set_color(STDERR_FILENO, MAGENTA);
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
		break;
	}
	case LOG_DEBUG: {
		set_color(STDERR_FILENO, WHITE);
		msg_stream << std::endl;
		break;
	}
	case LOG_INFO: {
		set_color(STDERR_FILENO, LGREEN);
		msg_stream << std::endl;
		break;
	}
	case LOG_WARN: {
		set_color(STDERR_FILENO, LBLUE);
		msg_stream << std::endl;
		break;
	}
	case LOG_ERROR: {
		set_color(STDERR_FILENO, LRED);
		msg_stream << ", errno = " << errno;
		memset(line_buf, 0, sizeof(line_buf));
		strerror_r(errno, line_buf, sizeof(line_buf));
		msg_stream << ", errstr=[" << line_buf << "]" << std::endl;
		break;
	}
	case LOG_FATAL: {
		set_color(STDERR_FILENO, YELLOW);
		msg_stream << "errno = " << errno;
		memset(line_buf, 0, sizeof(line_buf));
		strerror_r(errno, line_buf, sizeof(line_buf));
		msg_stream << ", errstr=[" << line_buf << "]" << std::endl;

		if (file_switcher_) {
			logging_file(msg_stream);
		}
		else {
			std::cerr << msg_stream.str();
			reset_color(STDERR_FILENO);
		}
		abort();
		break;
	}
	default: {
		break;
	}
	}

	if (file_switcher_) {
		logging_file(msg_stream);
	}
	else {
		std::cerr << msg_stream.str();
		reset_color(STDERR_FILENO);
	}
	return ;
}

void Lib_Log::make_lib_log_dir(void) {
	int ret = mkdir(lib_log_dir.c_str(), 0775);
	if (ret == -1 && errno != EEXIST) {
		perror("mkdir error");
	}
}

void Lib_Log::make_lib_log_filepath(std::string &path) {
	time_t time_v = time(NULL);
	struct tm tm_v;
	localtime_r(&time_v, &tm_v);

	std::stringstream stream;
	stream 	<< lib_log_dir << "/" << (tm_v.tm_year + 1900) << "-" << (tm_v.tm_mon + 1) << "-" << (tm_v.tm_mday)
			<< "-" << (tm_v.tm_hour) << ".log";

	path = stream.str().c_str();
}

void Lib_Log::logging_file(std::ostringstream &msg_stream) {
	GUARD(Lib_Log_File_Lock, mon, log_lock_);

	if (! log_file_.fp) {
		make_lib_log_dir();
		make_lib_log_filepath(log_file_.filepath);

		if ((log_file_.fp = fopen(log_file_.filepath.c_str(), "a")) == NULL) {
			printf("filepath=[%s]\n", log_file_.filepath.c_str());
			perror("fopen error");
			return;
		}
		log_file_.tv = Time_Value::gettimeofday();
	} else {
		if (! is_same_hour(log_file_.tv, Time_Value::gettimeofday())) {
			fclose(log_file_.fp);
			log_file_.tv = Time_Value::gettimeofday();
			make_lib_log_dir();
			make_lib_log_filepath(log_file_.filepath);

			if ((log_file_.fp = fopen(log_file_.filepath.c_str(), "a")) == NULL) {
				printf("filepath=[%s]", log_file_.filepath.c_str());
				perror("fopen error");
				return;
			}
		}
	}

	fputs(msg_stream.str().c_str(), log_file_.fp);
	fflush(log_file_.fp);
}
