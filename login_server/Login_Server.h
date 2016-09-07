/*
 * Login_Server.h
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#ifndef LOGIN_SERVER_H_
#define LOGIN_SERVER_H_

#include "Server.h"

class Login_Client_Server: public Server {
public:
	static Login_Client_Server *instance(void);

	virtual void process_list(void);

private:
	Login_Client_Server(void);
	virtual ~Login_Client_Server(void);
	Login_Client_Server(const Login_Client_Server &);
	const Login_Client_Server &operator=(const Login_Client_Server &);

private:
	static Login_Client_Server *instance_;
};

#define LOGIN_CLIENT_SERVER Login_Client_Server::instance()

///////////////////////////////////////////////////////////////////////////

class Login_Gate_Server: public Server {
public:
	static Login_Gate_Server *instance(void);

	virtual void process_list(void);

private:
	Login_Gate_Server(void);
	virtual ~Login_Gate_Server(void);
	Login_Gate_Server(const Login_Gate_Server &);
	const Login_Gate_Server &operator=(const Login_Gate_Server &);

private:
	static Login_Gate_Server *instance_;
};

#define LOGIN_GATE_SERVER Login_Gate_Server::instance()

#endif /* LOGIN_SERVER_H_ */
