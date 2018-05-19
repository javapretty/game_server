/*
 * Receive.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef RECEIVE_H_
#define RECEIVE_H_

#include "Thread.h"
#include "Svc.h"
#include "Epoll_Watcher.h"
#include "List.h"

class Receive;
class Server;
class Connector;
class Receive_Watcher: public Epoll_Watcher {
public:
	Receive_Watcher(Receive *r, int type = 0, int timeout = 0);
	virtual ~Receive_Watcher(void);

	virtual int inner_cmd(void);

private:
	Receive *receiver_;
};

class Receive: public Thread {
public:
	typedef List<int, Thread_Mutex> Drop_List;

	Receive(void);
	virtual ~Receive(void);

	int set(Server *server, Connector *connector, Time_Value *timeout = 0);
	int init(void);
	int fini(void);

	virtual void run_handler(void);

	Epoll_Watcher *reactor(void);

	int push_drop(int cid);
	int process_drop(void);

	int register_svc(Svc *svc);
	int unregister_svc(Svc *svc);

	virtual int drop_handler(int cid);
	virtual Svc *find_svc(int cid);

protected:
	Server *server_;
	Connector *connector_;

private:
	Drop_List drop_list_;
	Time_Value heartbeat_timeout_;
	Receive_Watcher *reactor_;
};

#endif /* RECEIVE_H_ */
