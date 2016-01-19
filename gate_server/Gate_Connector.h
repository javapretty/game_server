/*
 * Gate_Connector.h
 *
 *  Created on: 2016年1月13日
 *      Author: zhangyalei
 */

#ifndef GATE_CONNECTOR_H_
#define GATE_CONNECTOR_H_

#include "Connector.h"

class Gate_Game_Connector: public Connector {
public:
	static Gate_Game_Connector *instance(void);

	virtual void process_list(void);

private:
	Gate_Game_Connector(void);
	virtual ~Gate_Game_Connector(void);
	Gate_Game_Connector(const Gate_Game_Connector &);
	const Gate_Game_Connector &operator=(const Gate_Game_Connector &);

private:
	static Gate_Game_Connector *instance_;
};

#define GATE_GAME_CONNECTOR Gate_Game_Connector::instance()

////////////////////////////////////////////////////////////////////////

class Gate_Login_Connector: public Connector {
public:
	static Gate_Login_Connector *instance(void);

	virtual void process_list(void);

private:
	Gate_Login_Connector(void);
	virtual ~Gate_Login_Connector(void);
	Gate_Login_Connector(const Gate_Login_Connector &);
	const Gate_Login_Connector &operator=(const Gate_Login_Connector &);

private:
	static Gate_Login_Connector *instance_;
};

#define GATE_LOGIN_CONNECTOR Gate_Login_Connector::instance()

////////////////////////////////////////////////////////////////////////////////

class Gate_Master_Connector: public Connector {
public:
	static Gate_Master_Connector *instance(void);

	virtual void process_list(void);

private:
	Gate_Master_Connector(void);
	virtual ~Gate_Master_Connector(void);
	Gate_Master_Connector(const Gate_Master_Connector &);
	const Gate_Master_Connector &operator=(const Gate_Master_Connector &);

private:
	static Gate_Master_Connector *instance_;
};

#define GATE_MASTER_CONNECTOR Gate_Master_Connector::instance()

#endif /* GATE_SERVER_GATE_CONNECTOR_H_ */
