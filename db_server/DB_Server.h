/*
 * DB_Server.h
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#ifndef DB_SERVER_H_
#define DB_SERVER_H_

#include "Server.h"

class DB_Server: public Server {
public:
	static DB_Server *instance(void);

	virtual void process_list(void);

private:
	DB_Server(void);
	virtual ~DB_Server(void);
	DB_Server(const DB_Server &);
	const DB_Server &operator=(const DB_Server &);

private:
	static DB_Server *instance_;
};

#define DB_SERVER DB_Server::instance()

#endif /* DB_SERVER_H_ */
