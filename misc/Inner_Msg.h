/*
 * Inner_Msg.h
 *
 *  Created on: Aug 2, 2016
 *      Author: zhangyalei
 */

#ifndef INNER_MSG_H_
#define INNER_MSG_H_

#include "Block_Buffer.h"

struct Create_Role_Info {
	std::string account;
	std::string role_name;
	int8_t gender;
	int8_t career;

	Create_Role_Info(void);
	~Create_Role_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Create_Guild_Info {
	int64_t guild_id;
	std::string guild_name;
	int64_t chief_id;

	Create_Guild_Info(void);
	~Create_Guild_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_100001 {
	std::string account;
	std::string password;

	MSG_100001(void);
	~MSG_100001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_500001 {
	std::string gate_ip;
	int32_t gate_port;
	std::string session;

	MSG_500001(void);
	~MSG_500001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_100101 {
	std::string account;
	std::string session;	//登录验证的seesion

	MSG_100101(void);
	~MSG_100101();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_500101 {
	std::string account;

	MSG_500101(void);
	~MSG_500101();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_100102 {
	int32_t client_time;

	MSG_100102(void);
	~MSG_100102();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_500102 {
	int32_t client_time;
	int32_t server_time;

	MSG_500102(void);
	~MSG_500102();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_140000 {
	std::string account;
	std::string session;
	std::string gate_ip;
	int32_t gate_port;

	MSG_140000(void);
	~MSG_140000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_140001 {
	std::string account;

	MSG_140001(void);
	~MSG_140001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif /* INNER_MSG_H_ */
