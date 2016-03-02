/**
 * struct Client_Login_Message description
 *
 * [This file was auto-generated. Please Do not edit]
 *
 */

#ifndef CLIENT_LOGIN_MESSAGE_H_
#define CLIENT_LOGIN_MESSAGE_H_

#include "Block_Buffer.h"
#include "Msg_Struct.h"


/*

	注册
*/
struct MSG_100000 {

	std::string account;/*账号*/

	std::string password;/*密码*/

	MSG_100000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_500000 {

	std::string ip; /*gate ip*/

	int32_t port; /*gate port*/

	std::string session;

	MSG_500000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	登录
*/
struct MSG_100001 {

	std::string account;/*账号*/

	std::string password;/*密码*/

	MSG_100001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_500001 {

	std::string ip; /*gate ip*/

	int32_t port; /*gate port*/

	std::string session;

	MSG_500001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	客户端登录gate
*/
struct MSG_100002 {

	std::string account;

	std::string session; //登录验证的seesion

	MSG_100002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_500002 {

	std::string account;

	MSG_500002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	心跳
*/
struct MSG_100003 {

	int32_t client_time;

	MSG_100003(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
struct MSG_500003 {

	int32_t client_time;

	int32_t server_time;

	MSG_500003(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
