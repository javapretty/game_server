/*
 * Master_Server.h
 *
 *  Created on: 2016年1月13日
 *      Author: zhangyalei
 */

#ifndef MASTER_SERVER_H_
#define MASTER_SERVER_H_

#include "Server.h"

class Master_Gate_Server: public Server {
public:
	static Master_Gate_Server *instance(void);

	virtual void process_list(void);

private:
	Master_Gate_Server(void);
	virtual ~Master_Gate_Server(void);
	Master_Gate_Server(const Master_Gate_Server &);
	const Master_Gate_Server &operator=(const Master_Gate_Server &);

private:
	static Master_Gate_Server *instance_;
};

#define MASTER_GATE_SERVER Master_Gate_Server::instance()

////////////////////////////////////////////////////////////////////////

class Master_Game_Server: public Server {
public:
	static Master_Game_Server *instance(void);

	virtual void process_list(void);

private:
	Master_Game_Server(void);
	virtual ~Master_Game_Server(void);
	Master_Game_Server(const Master_Game_Server &);
	const Master_Game_Server &operator=(const Master_Game_Server &);

private:
	static Master_Game_Server *instance_;
};

#define MASTER_GAME_SERVER Master_Game_Server::instance()

#endif /* MASTER_SERVER_H_ */
