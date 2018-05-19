/*
 * Lib_Log.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 *
 */

#ifndef LIB_LOG_H_
#define LIB_LOG_H_

#include <cstdarg>
#include <string>
#include "Thread_Mutex.h"
#include "Time_Value.h"
#include "Public_Define.h"

struct Lib_Log_File {
	Lib_Log_File(void): tv(Time_Value::zero), fp(0)	{ }

	Time_Value tv;
	std::string filepath;
	FILE *fp;
};

class Lib_Log {
public:
	typedef Thread_Mutex Lib_Log_File_Lock;

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

	void set_file_switcher(bool file_switcher);

private:
	Lib_Log(void);
	virtual ~Lib_Log(void);

	void assembly_msg(int log_flag, const char *fmt, va_list ap);
	void logging_file(std::ostringstream &msg_stream);

	void make_lib_log_dir(void);
	void make_lib_log_filepath(std::string &path);

private:
	static Lib_Log *instance_;
	bool file_switcher_;			//是否写入文件
	Lib_Log_File_Lock log_lock_;
	Lib_Log_File log_file_;
};

////////////////////////////////////////////////////////////////////////////////

inline void Lib_Log::set_file_switcher(bool file_switcher) {
	file_switcher_ = file_switcher;
}

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
#define LIB_LOG_WARN(FMT, ...) do {						\
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
