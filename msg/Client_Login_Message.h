/**
 * struct Client_Login_Message description
 *
 * [This file was auto-generated. PLEASE DONT EDIT]
 *
 * @copyright urentech.com
 *
 */



#ifndef CLIENT_LOGIN_MESSAGE_H_
#define CLIENT_LOGIN_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"
#include "Msg_Struct.h"


/*

	注册请求
*/
struct MSG_110000 {

	std::string account;/*账号*/

	std::string password;/*密码*/

	MSG_110000(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	注册回复
*/
struct MSG_510000 {

	std::string ip;/*gate ip*/

	int32_t port;/*gate port*/

	std::string session;

	MSG_510000(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	登录请求
*/
struct MSG_110001 {

	std::string account;/*账号*/

	std::string password;/*密码*/

	MSG_110001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};
/*

	登录回复
*/
struct MSG_510001 {

	std::string ip;/*gate ip*/

	int32_t port;/*gate port*/

	std::string session;

	MSG_510001(void);

	void serialize(Block_Buffer & w) const;

	int deserialize(Block_Buffer & r);

	void reset(void);
};


#endif
