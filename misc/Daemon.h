/*
 * Daemon.h
 *
 *  Created on: Mar 26, 2016
 *      Author: zhangyalei
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#include "Event_Handler.h"
#include "Public_Struct.h"

class Epoll_Watcher;
class Daemon: public Event_Handler {
public:
	Daemon(int type);
	virtual ~Daemon(void);

	void loop(void);

	virtual int handle_timeout(const Time_Value &tv);
	virtual int start_log_client(void);

protected:
	int type_;
	Epoll_Watcher *watcher_;
};

/////////////////////////////////////////////////////////////////////
class Daemon_Log: public Daemon {
public:
	static Daemon_Log *instance(void);
	static void destroy(void);

	void start_server(int server_id);
	void start_client(void);

private:
	Daemon_Log(void);
	virtual ~Daemon_Log(void);
	Daemon_Log(const Daemon_Log &);
	const Daemon_Log &operator=(const Daemon_Log &);

private:
	static Daemon_Log *instance_;
};

#define DAEMON_LOG Daemon_Log::instance()

/////////////////////////////////////////////////////////////////////
class Daemon_DB: public Daemon {
public:
	static Daemon_DB *instance(void);
	static void destroy(void);

	void start_server(int server_id);
	void start_client(void);

private:
	Daemon_DB(void);
	virtual ~Daemon_DB(void);
	Daemon_DB(const Daemon_DB &);
	const Daemon_DB &operator=(const Daemon_DB &);

private:
	static Daemon_DB *instance_;
};

#define DAEMON_DB Daemon_DB::instance()

/////////////////////////////////////////////////////////////////////
class Daemon_Login: public Daemon {
public:
	static Daemon_Login *instance(void);
	static void destroy(void);

	void start_server(int server_id);
	void start_client(void);

private:
	Daemon_Login(void);
	virtual ~Daemon_Login(void);
	Daemon_Login(const Daemon_Login &);
	const Daemon_Login &operator=(const Daemon_Login &);

private:
	static Daemon_Login *instance_;
};

#define DAEMON_LOGIN Daemon_Login::instance()

/////////////////////////////////////////////////////////////////////
class Daemon_Master: public Daemon {
public:
	static Daemon_Master *instance(void);
	static void destroy(void);

	void start_server(int server_id);
	void start_client(void);

private:
	Daemon_Master(void);
	virtual ~Daemon_Master(void);
	Daemon_Master(const Daemon_Master &);
	const Daemon_Master &operator=(const Daemon_Master &);

private:
	static Daemon_Master *instance_;
};

#define DAEMON_MASTER Daemon_Master::instance()

/////////////////////////////////////////////////////////////////////
class Daemon_Game: public Daemon {
public:
	static Daemon_Game *instance(void);
	static void destroy(void);

	void start_server(int server_id);
	void start_client(void);

private:
	Daemon_Game(void);
	virtual ~Daemon_Game(void);
	Daemon_Game(const Daemon_Game &);
	const Daemon_Game &operator=(const Daemon_Game &);

private:
	static Daemon_Game *instance_;
};

#define DAEMON_GAME Daemon_Game::instance()

/////////////////////////////////////////////////////////////////////
class Daemon_Gate: public Daemon {
public:
	static Daemon_Gate *instance(void);
	static void destroy(void);

	void start_server(int server_id);
	void start_client(void);

private:
	Daemon_Gate(void);
	virtual ~Daemon_Gate(void);
	Daemon_Gate(const Daemon_Gate &);
	const Daemon_Gate &operator=(const Daemon_Gate &);

private:
	static Daemon_Gate *instance_;
};

#define DAEMON_GATE Daemon_Gate::instance()

#endif /* DAEMON_H_ */
