/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef GATE_INNER_MESSAGER_H_
#define GATE_INNER_MESSAGER_H_

#include "Public_Struct.h"

class Gate_Inner_Messager {
public:
	static Gate_Inner_Messager *instance(void);

	//login消息处理
	int process_login_block(Block_Buffer &buf);

	//game消息处理
	int process_game_block(Block_Buffer &buf);

	//master消息处理
	int process_master_block(Block_Buffer &buf);

	//玩家成功登录
	int process_success_login(int player_cid, std::string &account);

private:
	Gate_Inner_Messager(void);
	virtual ~Gate_Inner_Messager(void);
	Gate_Inner_Messager(const Gate_Inner_Messager &);
	const Gate_Inner_Messager &operator=(const Gate_Inner_Messager &);

private:
	static Gate_Inner_Messager *instance_;
};

#define GATE_INNER_MESSAGER Gate_Inner_Messager::instance()

#endif /* GATE_INNER_MESSAGER_H_ */
