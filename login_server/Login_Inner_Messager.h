/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef LOGIN_INNER_MESSAGER_H_
#define LOGIN_INNER_MESSAGER_H_

#include "Public_Struct.h"

class Login_Inner_Messager {
public:
	static Login_Inner_Messager *instance(void);

	/// gate消息处理
	int process_gate_block(Block_Buffer &buf);

	int gate_login_player_account(int gate_cid, int32_t player_cid, MSG_140000& msg);

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
