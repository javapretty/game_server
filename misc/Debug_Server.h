/*
 * Debug_Server.h
 *
 *  Created on: Dec 18, 2015
 *      Author: zhangyalei
 */

#ifndef DEBUG_SERVER_H_
#define DEBUG_SERVER_H_

#include "Event_Handler.h"
#include "Public_Struct.h"

class Epoll_Watcher;
class Debug_Server: public Event_Handler {
public:
	static Debug_Server *instance(void);
	static void destroy(void);

	int init(int argc, char *argv[]);
	int start(int argc, char *argv[]);
	virtual int handle_timeout(const Time_Value &tv) { return 0; }

	Server_Conf &server_conf(void) { return server_conf_; }

private:
	Debug_Server(void);
	~Debug_Server(void);
	Debug_Server(const Debug_Server &);
	const Debug_Server &operator=(const Debug_Server &);

private:
   static Debug_Server *instance_;
   Epoll_Watcher *wait_watcher_;
   Server_Conf server_conf_;
};

#define DEBUG_SERVER Debug_Server::instance()

#endif /* DEBUG_SERVER_H_ */
