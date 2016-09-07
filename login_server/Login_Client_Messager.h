/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef LOGIN_CLIENT_MESSAGER_H_
#define LOGIN_CLIENT_MESSAGER_H_

#include "Block_Buffer.h"

class Login_Client_Messager {
public:
	static Login_Client_Messager *instance(void);

	int process_block(Block_Buffer &buf);
	int connect_login(int cid, int msg_id, Block_Buffer &buf);

private:
	Login_Client_Messager(void);
	virtual ~Login_Client_Messager(void);
	Login_Client_Messager(const Login_Client_Messager &);
	const Login_Client_Messager &operator=(const Login_Client_Messager &);

private:
	static Login_Client_Messager *instance_;
};

#define LOGIN_CLIENT_MESSAGER Login_Client_Messager::instance()

#endif /* LOGIN_CLIENT_MESSAGER_H_ */
