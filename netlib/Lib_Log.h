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
		F_SYS_ABORT 	= 0x40,
		F_ABORT 		= 0x20,
		F_EXIT 			= 0x10,
		F_SYS 			= 0x8,
		F_USER			= 0x4,
		F_USER_TRACE	= 0x2,
		F_DEBUG			= 0x1,

		M_SYS_ABORT 	= 0,
		M_ABORT 		= 1,
		M_EXIT 			= 2,
		M_SYS 			= 3,
		M_USER			= 4,
		M_USER_TRACE	= 5,
		M_DEBUG			= 6,


		NULL_STUB		= 9,
	};
	static int msg_buf_size;
	static int backtrace_size;
	static std::string msg_head[];
	static std::string lib_log_dir;

	static Lib_Log *instance(void);
	static void destroy(void);

	void on_flag(int v);
	void off_flag(int v);

	void msg_sys_abort(const char *fmt, ...);
	void msg_abort(const char *fmt, ...);
	void msg_exit(const char *fmt, ...);
	void msg_sys(const char *fmt, ...);
	void msg_user(const char *fmt, ...);
	void msg_debug(const char *fmt, ...);

	void msg_user_trace(const char *fmt, ...);

	void set_switcher(int switcher);

private:
	Lib_Log(void);
	virtual ~Lib_Log(void);

	void assembly_msg(int log_flag, const char *fmt, va_list ap);
	void logging(std::ostringstream &msg_stream);

	void make_lib_log_dir(void);
	void make_lib_log_filepath(std::string &path);


private:
	static Lib_Log *instance_;
	int switcher_;

	Lib_Log_File_Lock log_lock_;
	Lib_Log_File log_file_;
};

////////////////////////////////////////////////////////////////////////////////

inline void Lib_Log::set_switcher(int switcher) {
	switcher_ = switcher;
}

////////////////////////////////////////////////////////////////////////////////

/// 调用abort产生core文件, 结束程序
#define LOG_ABORT(FMT, ...) do {					\
		Lib_Log::instance()->msg_abort("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 消息内包含errno和对应的错误描述, 调用abort产生core文件结束程序
#define LOG_SYS_ABORT(FMT, ...) do {					\
		Lib_Log::instance()->msg_sys_abort("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 调用exit结束程序
#define LOG_EXIT(FMT, ...) do {						\
		Lib_Log::instance()->msg_exit("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 消息内包含errno和对应的错误描述
#define LOG_SYS(FMT, ...) do {						\
		Lib_Log::instance()->msg_sys("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用户层代码错误信息
#define LOG_USER(FMT, ...) do {						\
		Lib_Log::instance()->msg_user("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用户层代码错误跟踪信息
#define LOG_USER_TRACE(FMT, ...) do {						\
		Lib_Log::instance()->msg_user_trace("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 调试信息
#define LOG_DEBUG(FMT, ...) do {					\
		Lib_Log::instance()->msg_debug("in %s:%d %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

#endif /* LIB_LOG_H_ */
