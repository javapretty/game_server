/*
 * Pack.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef STREAM_PACKER_H_
#define STREAM_PACKER_H_

#include "Thread.h"
#include "List.h"
#include "Svc.h"

class Server;
class Connector;

class Pack: public Thread {
public:
	typedef List<int, Thread_Mutex> Cid_List;
	typedef std::vector<Block_Buffer *> Block_Vector;

	Pack(void);
	virtual ~Pack(void);

	int set(Server *server, Connector *connector);

	int push_packing_cid(int cid);

	virtual Svc *find_svc(int cid);

	virtual Block_Buffer *pop_block(int cid);

	virtual int push_block(int cid, Block_Buffer *block);

	virtual int packed_data_handler(Block_Vector &block_vec);

	virtual int drop_handler(int cid);

	int push_drop(int cid);

	int process_drop(void);

	virtual void run_handler(void);

	void dump_info(void);

	int process(void);

	int process_packing_list(void);

protected:
	Server *server_;
	Connector *connector_;

private:
	Cid_List packing_list_;
	Cid_List drop_list_;
};

#endif /* STREAM_PACKER_H_ */
