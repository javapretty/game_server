/*
 * Game_Server.h
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#ifndef GAME_SERVER_H_
#define GAME_SERVER_H_

#include "Server.h"

class Game_Gate_Server: public Server {
public:
	static Game_Gate_Server *instance(void);

	virtual void process_list(void);

private:
	Game_Gate_Server(void);
	virtual ~Game_Gate_Server(void);
	Game_Gate_Server(const Game_Gate_Server &);
	const Game_Gate_Server &operator=(const Game_Gate_Server &);

private:
	static Game_Gate_Server *instance_;
};

#define GAME_GATE_SERVER Game_Gate_Server::instance()

#endif /* GAME_SERVER_H_ */
