/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef MASTER_CLIENT_MESSAGER_H_
#define MASTER_CLIENT_MESSAGER_H_

#include "Msg_Define.h"

class Block_Buffer;
class Master_Client_Messager {
public:
	static Master_Client_Messager *instance(void);

	int process_block(Block_Buffer &buf);

	//gate通知master玩家上线
	int gate_master_player_signin(int gate_cid, int player_cid, Block_Buffer &buf);

private:
	Master_Client_Messager(void);
	virtual ~Master_Client_Messager(void);
	Master_Client_Messager(const Master_Client_Messager &);
	const Master_Client_Messager &operator=(const Master_Client_Messager &);

private:
	static Master_Client_Messager *instance_;
};

#define MASTER_CLIENT_MESSAGER Master_Client_Messager::instance()

#endif /* MASTER_CLIENT_MESSAGER_H_ */
