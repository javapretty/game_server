/** 
* struct Client_Login_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#ifndef __Client_Login_Message__H
#define __Client_Login_Message__H

#include "Block_Buffer.h"
#include "Msg_Struct.h"

struct MSG_100000 {
	std::string account;	
	std::string password;	

	MSG_100000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_500000 {
	std::string ip;	//gate_ip
	int32_t port;	//gate_port
	std::string session;	

	MSG_500000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_100001 {
	std::string account;	
	std::string password;	

	MSG_100001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_500001 {
	std::string ip;	//gate_ip
	int32_t port;	//gate_port
	std::string session;	

	MSG_500001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_100002 {
	std::string account;	
	std::string session;	//登录验证的seesion

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
