/*
 * Game_Connector.h
 *
 *  Created on: 2016年1月13日
 *      Author: zhangyalei
 */

#ifndef GAME_CONNECTOR_H_
#define GAME_CONNECTOR_H_

#include "Connector.h"

class Game_DB_Connector: public Connector {
public:
	static Game_DB_Connector *instance(void);

	virtual void process_list(void);

private:
	Game_DB_Connector(void);
	virtual ~Game_DB_Connector(void);
	Game_DB_Connector(const Game_DB_Connector &);
	const Game_DB_Connector &operator=(const Game_DB_Connector &);

private:
	static Game_DB_Connector *instance_;
};

#define GAME_DB_CONNECTOR Game_DB_Connector::instance()

///////////////////////////////////////////////////////////////////////////

class Game_Master_Connector: public Connector {
public:
	static Game_Master_Connector *instance(void);

	virtual void process_list(void);

private:
	Game_Master_Connector(void);
	virtual ~Game_Master_Connector(void);
	Game_Master_Connector(const Game_Master_Connector &);
	const Game_Master_Connector &operator=(const Game_Master_Connector &);

private:
	static Game_Master_Connector *instance_;
};

#define GAME_MASTER_CONNECTOR Game_Master_Connector::instance()

#endif /* GAME_CONNECTOR_H_ */
