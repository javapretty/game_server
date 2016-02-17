/*
 * Send.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef SENDER_H_
#define SENDER_H_

#include "Thread.h"
#include "Epoll_Watcher.h"
#include "Event_Handler.h"
#include "List.h"
#include "Block_List.h"
#include "boost/unordered_map.hpp"

class Svc;
class Server;
class Connector;

class Send: public Thread, public Event_Handler {
	typedef Thread_Mutex Svc_Map_Lock;
	typedef boost::unordered_map<int, Svc *> Svc_Map;
	typedef List<int, Thread_Mutex> Drop_List;
	typedef Block_List<Thread_Mutex> Data_Block_List;

public:
	Send(void);

	virtual ~Send(void);

	void set(Server *server, Connector *connector, Time_Value &interval);

	int init(void);

	int fini(void);

	virtual void run_handler(void);

	/// 获取、释放一个buf
	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *buf);

	int push_data_block_with_len(int cid, Block_Buffer &rbuf);

	int append_send_block(void);

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
	Data_Block_List append_list_;
	Drop_List drop_list_;

	Svc_Map_Lock svc_map_lock_;
	Svc_Map svc_map_;

	Time_Value interval_;
	bool is_register_self_;
};


#endif /* SENDER_H_ */
