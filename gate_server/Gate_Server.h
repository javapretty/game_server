/*
 * Gate_Server.h
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#ifndef GATE_SERVER_H_
#define GATE_SERVER_H_

#include "Server.h"

class Gate_Client_Server: public Server {
public:
	static Gate_Client_Server *instance(void);

	virtual void process_list(void);

private:
	Gate_Client_Server(void);
	virtual ~Gate_Client_Server(void);
	Gate_Client_Server(const Gate_Client_Server &);
	const Gate_Client_Server &operator=(const Gate_Client_Server &);

private:
	static Gate_Client_Server *instance_;
};

#define GATE_CLIENT_SERVER Gate_Client_Server::instance()

#endif /* GATE_SERVER_H_ */
