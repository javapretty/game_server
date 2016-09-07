/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef GATE_CLIENT_MESSAGER_H_
#define GATE_CLIENT_MESSAGER_H_

class Block_Buffer;
class Gate_Client_Messager {
public:
	static Gate_Client_Messager *instance(void);

	int process_block(Block_Buffer &buf);
	int process_gate_block(int cid, int msg_id, Block_Buffer &buf);

	int connect_gate(int cid, Block_Buffer &buf);
	int send_heartbeat(int cid, Block_Buffer &buf);

private:
	Gate_Client_Messager(void);
	virtual ~Gate_Client_Messager(void);
	Gate_Client_Messager(const Gate_Client_Messager &);
	const Gate_Client_Messager &operator=(const Gate_Client_Messager &);

private:
	static Gate_Client_Messager *instance_;
};

#define GATE_CLIENT_MESSAGER Gate_Client_Messager::instance()

#endif /* GATE_CLIENT_MESSAGER_H_ */
