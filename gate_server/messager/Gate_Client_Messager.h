/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GATE_CLIENT_MESSAGER_H_
#define GATE_CLIENT_MESSAGER_H_

#include "Msg_Define.h"

class Block_Buffer;
class Gate_Player;

class Gate_Client_Messager {
public:
	static Gate_Client_Messager *instance(void);

	int process_block(Block_Buffer &buf);
	int process_gate_block(int cid, int msg_id, Block_Buffer &buf);
	int process_game_block(int msg_id, Block_Buffer &buf);

	int refresh_heartbeat(int cid, MSG_111000 &msg);
	int connect_gate(int cid, MSG_111001 &msg);

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
