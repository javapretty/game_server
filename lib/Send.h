/*
 * Send.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef SEND_H_
#define SEND_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "List.h"
#include "Block_List.h"

class Svc;
class Server;
class Connector;
class Send: public Thread, public Event_Handler {
	typedef Thread_Mutex Svc_Map_Lock;
	typedef boost::unordered_map<int, Svc *> Svc_Map;
	typedef List<int, Thread_Mutex> Drop_List;

public:
	Send(void);
	virtual ~Send(void);

	void set(Server *server, Connector *connector, Time_Value &interval);
	int init(void);
	int fini(void);

	virtual void run_handler(void);

	virtual Block_Buffer *pop_block(int cid);
	virtual int push_block(int cid, Block_Buffer *buffer);

	//将要发送的数据包放到svc的send_block_list里面
	int push_svc_block(int cid, Block_Buffer &buffer);

	int push_drop(int cid);
	int process_drop(void);

	int register_svc(Svc *svc);
	int unregister_svc(Svc *svc);

	int register_self_timer(void);

	virtual int drop_handler(int cid);
	virtual Svc *find_svc(int cid);

	Epoll_Watcher *reactor(void);

	Time_Value &get_interval(void);
	void set_interval(Time_Value &tv);

	virtual int handle_timeout(const Time_Value &tv);

protected:
	Server *server_;
	Connector *connector_;

private:
	Epoll_Watcher *reactor_;
	Drop_List drop_list_;

	Svc_Map_Lock svc_map_lock_;
	Svc_Map svc_map_;

	Time_Value interval_;
	bool is_register_self_;
};

#endif /* SEND_H_ */
