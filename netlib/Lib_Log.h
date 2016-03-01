/*
 * Lib_Log.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 *
 *  记录底层组件日志写本地文件
 *
 */

#ifndef LIB_LOG_H_
#define LIB_LOG_H_

#include <cstdarg>
#include <string>
#include "Thread_Mutex.h"
#include "Time_Value.h"

struct Lib_Log_File {
	Lib_Log_File(void): tv(Time_Value::zero), fp(0)	{ }

	Time_Value tv;
	std::string filepath;
	FILE *fp;
};

class Lib_Log {
public:
	typedef Thread_Mutex Lib_Log_File_Lock;
	enum {
		LOG_TRACE = 0,		//打印程序运行堆栈，跟踪记录数据信息，与DEBUG相比更细致化的记录信息
		LIB_LOG_DEBUG = 1,		//细粒度信息事件对调试应用程序是非常有帮助的
		LOG_INFO = 2,			//消息在粗粒度级别上突出强调应用程序的运行过程
		LOG_WARN = 3,			//会出现潜在错误的情形
		LOG_ERROR = 4,		//虽然发生错误事件，但仍然不影响系统的继续运行
		LOG_FATAL = 5,		//严重的错误事件，将会导致应用程序的退出
		NULL_STUB = 6,
	};
	static int msg_buf_size;
	static int backtrace_size;
	static std::string msg_head[];
	static std::string lib_log_dir;

	static Lib_Log *instance(void);
	static void destroy(void);

	void log_trace(const char *fmt, ...);
	void log_debug(const char *fmt, ...);
	void log_info(const char *fmt, ...);
	void log_warn(const char *fmt, ...);
	void log_error(const char *fmt, ...);
	void log_fatal(const char *fmt, ...);

private:
	Lib_Log(void);
	virtual ~Lib_Log(void);

	void assembly_msg(int log_flag, const char *fmt, va_list ap);
	void logging_file(std::ostringstream &msg_stream);

	void make_lib_log_dir(void);
	void make_lib_log_filepath(std::string &path);

private:
	static Lib_Log *instance_;

	Lib_Log_File_Lock log_lock_;
	Lib_Log_File log_file_;
};

////////////////////////////////////////////////////////////////////////////////

//打印程序运行堆栈,跟踪记录数据信息
#define LIB_LOG_TRACE(FMT, ...) do {						\
		Lib_Log::instance()->log_trace("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//调试信息
#define LIB_LOG_DEBUG(FMT, ...) do {					\
		Lib_Log::instance()->log_debug("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//突出强调应用程序的运行过程
#define LIB_LOG_INFO(FMT, ...) do {						\
		Lib_Log::instance()->log_info("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//出现潜在错误的情形
#define LIB_LOG_WRAN(FMT, ...) do {						\
		Lib_Log::instance()->log_warn("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//虽然发生错误事件，但仍然不影响系统的继续运行
#define LIB_LOG_ERROR(FMT, ...) do {						\
		Lib_Log::instance()->log_error("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

//严重的错误事件，将会导致应用程序的退出
#define LIB_LOG_FATAL(FMT, ...) do {					\
		Lib_Log::instance()->log_fatal("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

#endif /* LIB_LOG_H_ */
