/*
 * Svc.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef SVC_H_
#define SVC_H_

#include "Event_Handler.h"
#include "Block_List.h"
#include "Thread_Mutex.h"

class Block_Buffer;
class Server;
class Connector;

class Svc: public Event_Handler {
public:
	typedef Block_List<Thread_Mutex> Data_Block_List;
	typedef std::vector<Block_Buffer *> Block_Vector;

	const static int MAX_LIST_SIZE = 1000;
	const static int MAX_PACK_SIZE = 60 * 1024;

	Svc(void);

	virtual ~Svc(void);

	void reset(void);

	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *block);

	virtual int register_recv_handler(void);

	virtual int unregister_recv_handler(void);

	virtual int register_send_handler(void);

	virtual int unregister_send_handler(void);

	virtual int recv_handler(int cid);

	virtual int close_handler(int cid);

	virtual int handle_input(void);

	virtual int handle_close(void);

	int close_fd(void);

	int recv_data(void);

	int send_data(void);

	int pack_recv_data(Block_Vector &block_vec);

	int push_recv_block(Block_Buffer *buf);

	int push_send_block(Block_Buffer *buf);

	void set_cid(int cid);

	int get_cid(void);

	bool get_reg_recv(void);

	void set_reg_recv(bool val);

	bool get_reg_send(void);

	void set_reg_send(bool val);

	bool is_closed(void);

	bool get_closed(void);

	void set_closed(bool v);

	int get_peer_addr(std::string &ip, int &port);

	int get_local_addr(std::string &ip, int &port);

	void set_max_list_size(size_t max_size);

	void set_max_pack_size(size_t max_size);

	void set_peer_addr(void);

	void set_role_id(int64_t role_id);

	void set_server(Server *server);

	void set_connector(Connector *connector);

protected:
	Server *server_;
	Connector *connector_;

private:
	int cid_;
	Data_Block_List recv_block_list_;
	Data_Block_List send_block_list_;

	size_t max_list_size_;
	size_t max_pack_size_;

	bool is_closed_;
	bool is_reg_recv_, is_reg_send_;

	std::string peer_ip_;
	int peer_port_;

	int64_t role_id_;
};

////////////////////////////////////////////////////////////////////////////////

inline void Svc::reset(void) {
	Data_Block_List::BList blist;

	blist.clear();
	recv_block_list_.swap(blist);
	for (Data_Block_List::BList::iterator it = blist.begin(); it != blist.end(); ++it) {
		push_block(cid_, *it);
	}

	blist.clear();
	send_block_list_.swap(blist);
	for (Data_Block_List::BList::iterator it = blist.begin(); it != blist.end(); ++it) {
		push_block(cid_, *it);
	}

	cid_ = 0;
	is_closed_ = false;
	is_reg_recv_ = false;
	is_reg_send_ = false;

	max_list_size_ = MAX_LIST_SIZE;
	max_pack_size_ = MAX_PACK_SIZE;

	peer_ip_.clear();
	peer_port_ = 0;

	role_id_ = 0;
	server_ = 0;
	Event_Handler::reset();
}

inline int Svc::push_recv_block(Block_Buffer *buf) {
	if (is_closed_)
		return -1;

	if (recv_block_list_.size() >= max_list_size_) {
		LIB_LOG_INFO("recv_block_list_ has full.");
		return -1;
	}
	recv_block_list_.push_back(buf);
	return 0;
}

inline int Svc::push_send_block(Block_Buffer *buf) {
	if (is_closed_)
		return -1;

	if (send_block_list_.size() >= max_list_size_) {
		LIB_LOG_INFO("role_id:%ld send_block_list_ has full send_block_list_.size() = %d, max_list_size_ = %d", role_id_, send_block_list_.size(), max_list_size_);
		handle_close();
		return -1;
	}
	send_block_list_.push_back(buf);
	return 0;
}

inline void Svc::set_cid(int cid) {
	cid_ = cid;
}

inline int Svc::get_cid(void) {
	return cid_;
}

inline bool Svc::get_reg_recv(void) {
	return is_reg_recv_;
}

inline void Svc::set_reg_recv(bool val) {
	is_reg_recv_ = val;
}

inline bool Svc::get_reg_send(void) {
	return is_reg_send_;
}

inline void Svc::set_reg_send(bool val) {
	is_reg_send_ = val;
}

inline bool Svc::is_closed(void) {
	return is_closed_;
}

inline bool Svc::get_closed(void) {
	return is_closed_;
}

inline void Svc::set_closed(bool v) {
	is_closed_ = v;
}

inline void Svc::set_max_list_size(size_t max_size) {
	max_list_size_ = max_size;
}

inline void Svc::set_max_pack_size(size_t max_size) {
	max_pack_size_ = max_size;
}

inline void Svc::set_peer_addr(void) {
	get_peer_addr(peer_ip_, peer_port_);
}

inline void Svc::set_role_id(int64_t role_id) {
	role_id_ = role_id;
}

#endif /* SVC_H_ */
