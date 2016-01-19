// -*- C++ -*-
/*
 * Debug_Starter.h
 *
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#ifndef DEBUG_STARTER_H_
#define DEBUG_STARTER_H_

#include "Event_Handler.h"

class Epoll_Watcher;

class Debug_Starter: public Event_Handler {
public:
	static Debug_Starter *instance(void);
	static void destroy(void);

	int init(int argc, char *argv[]);
	int fini(void);

	int start(int argc, char *argv[]);
	virtual int handle_timeout(const Time_Value &tv);

private:
	Debug_Starter(void);
	~Debug_Starter(void);
	Debug_Starter(const Debug_Starter &);
	const Debug_Starter &operator=(const Debug_Starter &);

private:
   static Debug_Starter *instance_;
   Epoll_Watcher *server_reactor_;
   bool is_register_;
};

#endif /* DEBUG_STARTER_H_ */
