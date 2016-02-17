/*
 * Server.h
 *
 *  Created on: Jan 16,2016
 *      Author: zhangyalei
 */

#ifndef LIB_SERVER_H_
#define LIB_SERVER_H_

#include "Accept.h"
#include "Pack.h"
#include "Receive.h"
#include "Send.h"
#include "Svc.h"
#include "Svc_Static_List.h"
#include "Thread_Mutex.h"
#include "Object_Pool.h"
#include "Block_Pool_Group.h"

class Server_Accept: public Accept {
public:
	virtual int accept_svc(int connfd);
};

////////////////////////////////////////////////////////////////////////////////

class Server_Svc: public Svc {
public:
	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *block);

	virtual int register_recv_handler(void);

	virtual int unregister_recv_handler(void);

	virtual int register_send_handler(void);

	virtual int unregister_send_handler(void);

	virtual int recv_handler(int cid);

	virtual int close_handler(int cid);
};

////////////////////////////////////////////////////////////////////////////////

class Server_Receive: public Receive {
public:
	virtual int drop_handler(int cid);

	virtual Svc *find_svc(int cid);
};

////////////////////////////////////////////////////////////////////////////////

class Server_Send: public Send {
public:
	/// 获取、释放一个buf
	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *buf);

	virtual int drop_handler(int cid);

	virtual Svc *find_svc(int cid);
};

////////////////////////////////////////////////////////////////////////////////

class Server_Pack: public Pack {
public:
	virtual Svc *find_svc(int cid);

	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *block);

	virtual int packed_data_handler(Block_Vector &block_vec);

	virtual int drop_handler(int cid);
};

////////////////////////////////////////////////////////////////////////////////

class Server: public Thread {
public:
	typedef Object_Pool<Server_Svc, Spin_Lock> Server_Svc_Pool;
	typedef Svc_Static_List<Server_Svc *, Spin_Lock> Svc_List;
	typedef Block_List<Thread_Mutex> Data_List;
	typedef List<int, Thread_Mutex> Int_List;

	static const int svc_max_list_size = 400; /// 200个包
	static const int svc_max_pack_size = 10 * 1024; /// 接收单个包最大32k

	Server(void);
	virtual ~Server(void);

	void run_handler(void);
	virtual void process_list(void);

	void set(int port, Time_Value &recv_timeout, Time_Value &send_interval);
	int init(void);
	int start(void);

	inline Svc_List &svc_list(void) { return svc_static_list_; }
	inline Server_Svc_Pool &svc_pool(void) { return svc_pool_; }
	inline Data_List &block_list(void) { return block_list_;}
	inline Int_List &drop_cid_list(void) { return drop_cid_list_; }
	inline Server_Accept &accept(void) { return accept_; }
	inline Server_Receive &receive(void) { return receive_; }
	inline Server_Send &send(void) { return send_; }
	inline Server_Pack &pack(void) { return pack_; }

	Block_Buffer *pop_block(int cid);
	int push_block(int cid, Block_Buffer *buf);
	int send_block(int cid, Block_Buffer &buf);

	Svc *find_svc(int cid);
	int recycle_svc(int cid);

	int get_server_info(Server_Info &info);
	void free_cache(void);

protected:
	Data_List block_list_;
	Int_List drop_cid_list_;

private:
	Svc_List svc_static_list_;
	Server_Svc_Pool svc_pool_;
	Block_Pool_Group block_pool_group_;

	Server_Accept accept_;
	Server_Receive receive_;
	Server_Send send_;
	Server_Pack pack_;
};

#endif /* LIB_SERVER_H_ */
