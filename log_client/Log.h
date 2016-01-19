/*
 * Log.h
 *
 *  Created on: May 10, 2012
 *      Author: zhangyalei
 */

#ifndef LOG_H_
#define LOG_H_

#include <cstdarg>
#include <string>
#include <vector>
#include "boost/unordered_map.hpp"
#include "Time_Value.h"

class Block_Buffer;

struct Msg_Process_Time {
	int msg_id;
	int times;
	Time_Value tv;
	Msg_Process_Time(void) : msg_id(0), times(0) {}
	void add_time(Time_Value &process_tv) {;
		tv += process_tv;
		times++;
	}
};
typedef boost::unordered_map<long, Msg_Process_Time> Msg_Process_Time_Map;
typedef std::vector<Msg_Process_Time> Msg_Process_Time_Vec;

class Log {
public:
	enum {
		F_SYS_ABORT 		= 0x40,
		F_ABORT 				= 0x20,
		F_EXIT 				= 0x10,
		F_SYS 					= 0x8,
		F_USER					= 0x4,
		F_USER_TRACE		= 0x2,
		F_DEBUG				= 0x1,
	};

	enum {
		M_SYS_ABORT 		= 0,
		M_ABORT 				= 1,
		M_EXIT 				= 2,
		M_SYS 					= 3,
		M_USER					= 4,
		M_USER_TRACE		= 5,
		M_DEBUG				= 6,

		LOG_LOGIN			= 7,
		LOG_GATE				= 8,
		LOG_GAME				= 9,
		LOG_MASTER			= 10,
		LOG_DB 				= 11,
		LOG_MISC 			= 12,
	};

	static int msg_buf_size;
	static int backtrace_size;
	static std::string msg_head[];

	static Log *instance(void);

	void msg_sys_abort(const char *fmt, ...);
	void msg_abort(const char *fmt, ...);
	void msg_exit(const char *fmt, ...);
	void msg_sys(const char *fmt, ...);
	void msg_user(const char *fmt, ...);
	void msg_user_trace(const char *fmt, ...);
	void msg_debug(const char *fmt, ...);

	void set_log_type(int type, int sub_type = 0);
	int get_log_type(void);
	void set_switcher(int switcher);

	void assembly_msg(int log_flag, const char *fmt, va_list ap);
	void logging_file(std::ostringstream &msg_stream);
	int logging_db(Block_Buffer &buf);

	void free_cache(void);

	void msg_time(int msg_id, Time_Value &time);
	void show_msg_time(Time_Value &now);
	void show_msg_time(void);

private:
	Log(void);
	virtual ~Log(void);

private:
	static Log *instance_;
	int switcher_;
	int log_type_;
	int log_sub_type_;
	Time_Value show_time_;
	Msg_Process_Time_Map msg_time_;
};

#define LOG Log::instance()

////////////////////////////////////////////////////////////////////////////////

inline void Log::set_log_type(int type, int sub_type) {
	log_type_ = type;
	log_sub_type_ = sub_type;
}

inline int Log::get_log_type(void) {
	return log_type_;
}

inline void Log::set_switcher(int switcher) {
	switcher_ = switcher;
}

////////////////////////////////////////////////////////////////////////////////
/// 调用abort产生core文件, 结束程序
#define MSG_ABORT(FMT, ...) do {					\
		Log::instance()->msg_abort("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 消息内包含errno和对应的错误描述, 调用abort产生core文件结束程序
#define MSG_SYS_ABORT(FMT, ...) do {					\
		Log::instance()->msg_sys_abort("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 调用exit结束程序
#define MSG_EXIT(FMT, ...) do {						\
		Log::instance()->msg_exit("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 消息内包含errno和对应的错误描述
#define MSG_SYS(FMT, ...) do {						\
		Log::instance()->msg_sys("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用户层代码错误信息
#define MSG_USER(FMT, ...) do {						\
		Log::instance()->msg_user("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 用户层代码错误信息
#define MSG_USER_TRACE(FMT, ...) do {						\
		Log::instance()->msg_user_trace("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)

/// 调试信息
#define MSG_DEBUG(FMT, ...) do {					\
		Log::instance()->msg_debug("in %s:%d function %s: "#FMT, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	} while (0)


class Perf_Mon {
public:
	Perf_Mon(int msg_id)
		: msg_id_(msg_id), tv_(Time_Value::gettimeofday()) { }

	~Perf_Mon(void) {
		Time_Value res_tv = Time_Value::gettimeofday() - tv_;
		LOG->msg_time(msg_id_, res_tv);
	}

private:
	int msg_id_;
	Time_Value tv_;
};

#endif /* LOG_H_ */
