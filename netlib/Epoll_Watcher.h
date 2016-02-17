/*
 * Epoll_Watcher.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 *
 *  Reactor
 *  底层使用epoll实现，主要功能为io事件回调、心跳超时机制、毫秒级定时器
 *
 */

#ifndef EPOLL_WATCHER_H_
#define EPOLL_WATCHER_H_

#include <queue>
#include <vector>
#include "Event_Handler.h"
#include "Object_Pool.h"
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"
#include "boost/unordered_map.hpp" /// notice: 如果用std::unordered_map, 需添加-std=c++0x参数 -D__GXX_EXPERIMENTAL_CXX0X__

class Time_Value;

class Epoll_Watcher : public Event_Handler {
	/// Timer
	typedef std::priority_queue<Event_Handler *, std::vector<Event_Handler *>, Event_Handler::greater> Timer_Queue;
	typedef boost::unordered_set<Event_Handler *> Event_Timer_Set;

	/// IO
	typedef std::vector<Event_Handler *> Event_Map;

	/// 心跳
	typedef boost::unordered_map<int, Event_Handler *> Heart_Map;

	typedef RE_Mutex Mutex;

public:

	enum {
		EVENT_INPUT 		= 0x1,
		EVENT_OUTPUT 		= 0x2,
		EVENT_TIMEOUT 		= 0x4,
		EVENT_ONCE_IO_IN 	= 0x8,		/// 一次性IO输入事件
		EVENT_ONCE_IO_OUT 	= 0x10,		/// 一次性IO输出事件
		EVENT_ONCE_TIMER 	= 0x20,		/// 一次性定时器事件
		WITH_IO_HEARTBEAT 	= 0x40,		/// IO附带心跳机制
	};

	/**
	 * type可以指定IO附带心跳机制: WITH_IO_HEARTBEAT
	 * heart_second制定IO心跳超时时间
	 */
	Epoll_Watcher(int type = 0, int heart_second = 30);

	virtual ~Epoll_Watcher(void);

	int init(void);

	int fini(void);

	/**
	 * 添加监听事件
	 *	op可以为EVENT_INPUT, EVENT_OUTPUT, EVENT_TIMEOUT等位集合
	 *	EVENT_INPUT, EVENT_OUTPUT, EVENT_ONCE_IO_IN, EVENT_ONCE_IO_OUT为监听IO事件
	 *	EVENT_TIMEOUT, EVENT_ONCE_TIMER为监听定时器事件
	 *	tv为定时器事件指定的间隔时间
	 */
	virtual int add(Event_Handler *evh, int op, Time_Value *tv = NULL);

	/// 移除eh的IO/定时器事件监听
	virtual int remove(Event_Handler *eh);

	/// 开始事件监听循环
	virtual int loop(void);

	/// 结束事件监听循环
	virtual int end_loop(void);

	/// 内部接口
	virtual int inner_cmd(void);

	/// 使阻塞的epoll_wait返回, 执行新的inner_cmd/定时器
	virtual int notify(void);

private:
	int open(void);

	int io_open(void);

	int timer_open(void);

	int add_io(Event_Handler *evh, int op);

	int add_timer(Event_Handler *evh, int op, Time_Value *tv);

	int remove_io(Event_Handler *evh);

	int remove_timer(Event_Handler *evh);

	/// 计算epoll_wait的超时时间
	int calculate_timeout(void);

	/// 处理已到期定时器事件
	void process_timer_event(void);

	void watcher_loop(void);

	int handle_input(void);

	int handle_timeout(const Time_Value &tv);

	int next_heart_idx(void);

private:
	int type_;
	bool end_flag_;
	int epfd_;
	int max_events_;
	struct epoll_event *events_;

	Event_Map pending_io_map_;
	int io_heart_idx_, heart_second_;
	Heart_Map io_heart_map_[2];
	Mutex io_lock_;

	Timer_Queue tq_; /// 时间优先队列(最小堆)
	Event_Timer_Set timer_set_;
	Mutex tq_lock_;

	int pipe_fd_[2]; /// notify管道
};

#endif /* EPOLL_WATCHER_H_ */
