/*
 * Log_Server.h
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#ifndef LOG_SERVER_H_
#define LOG_SERVER_H_

#include "Server.h"

class Log_Server: public Server {
public:
	static Log_Server *instance(void);

	virtual void process_list(void);

private:
	Log_Server(void);
	virtual ~Log_Server(void);
	Log_Server(const Log_Server &);
	const Log_Server &operator=(const Log_Server &);

private:
	static Log_Server *instance_;
};

#define LOG_SERVER Log_Server::instance()

#endif /* LOG_SERVER_H_ */
