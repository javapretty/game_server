/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef LOGIN_INNER_MESSAGER_H_
#define LOGIN_INNER_MESSAGER_H_

#include "Msg_Define.h"
#include "Block_Buffer.h"

class Login_Inner_Messager {
public:
	static Login_Inner_Messager *instance(void);

	/// gate消息处理
	int process_gate_block(Block_Buffer &buf);

	/// login自身回环消息处理
	int process_self_loop_block(Block_Buffer &buf);

	int process_112000(int gate_cid, int32_t player_cid, MSG_112000& msg);

private:
	Login_Inner_Messager(void);
	virtual ~Login_Inner_Messager(void);
	Login_Inner_Messager(const Login_Inner_Messager &);
	const Login_Inner_Messager &operator=(const Login_Inner_Messager &);

private:
	static Login_Inner_Messager *instance_;
};

#define LOGIN_INNER_MESSAGER Login_Inner_Messager::instance()


#endif /* LOGIN_INNER_MESSAGER_H_ */
