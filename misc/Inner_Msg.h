/*
 * Inner_Msg.h
 *
 *  Created on: 2016年8月2日
 *      Author: zhangyalei
 */

#ifndef INNER_MSG_H_
#define INNER_MSG_H_

#include "Block_Buffer.h"
#include "boost/unordered_map.hpp"

struct MSG {
	MSG(){}
	virtual ~MSG(){}
	virtual void serialize(Block_Buffer &buffer) const = 0;
	virtual int deserialize(Block_Buffer &buffer) = 0;
	virtual void reset(void) = 0;
	virtual void print(void) = 0;
};

struct Create_Role_Info : public MSG {
	std::string account;
	std::string role_name;
	std::string client_ip;
	int8_t gender;
	int8_t career;

	Create_Role_Info(void);
	~Create_Role_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct Create_Guild_Info : public MSG {
	int64_t guild_id;
	std::string guild_name;
	int64_t chief_id;

	Create_Guild_Info(void);
	~Create_Guild_Info();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100000 : public MSG {
	std::string account;
	std::string password;

	MSG_100000(void);
	~MSG_100000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500000 : public MSG {
	std::string ip;	//gate_ip
	int32_t port;	//gate_port
	std::string session;

	MSG_500000(void);
	~MSG_500000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100001 : public MSG {
	std::string account;
	std::string password;

	MSG_100001(void);
	~MSG_100001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500001 : public MSG {
	std::string ip;	//gate_ip
	int32_t port;	//gate_port
	std::string session;

	MSG_500001(void);
	~MSG_500001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100002 : public MSG {
	std::string account;
	std::string session;	//登录验证的seesion

	MSG_100002(void);
	~MSG_100002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500002 : public MSG {
	std::string account;

	MSG_500002(void);
	~MSG_500002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_100003 : public MSG {
	int32_t client_time;

	MSG_100003(void);
	~MSG_100003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_500003 : public MSG {
	int32_t client_time;
	int32_t server_time;

	MSG_500003(void);
	~MSG_500003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_140000 : public MSG {
	std::string account;
	std::string session;
	std::string gate_ip;
	int32_t gate_port;

	MSG_140000(void);
	~MSG_140000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_140001 : public MSG {
	std::string account;

	MSG_140001(void);
	~MSG_140001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150001 : public MSG {
	std::string account;
	std::string client_ip;

	MSG_150001(void);
	~MSG_150001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550001 : public MSG {

	MSG_550001(void);
	~MSG_550001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150002 : public MSG {
	Create_Role_Info role_info;

	MSG_150002(void);
	~MSG_150002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550002 : public MSG {

	MSG_550002(void);
	~MSG_550002();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150003 : public MSG {

	MSG_150003(void);
	~MSG_150003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550003 : public MSG {
	int64_t role_id;

	MSG_550003(void);
	~MSG_550003();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150100 : public MSG {
	Create_Guild_Info guild_info;

	MSG_150100(void);
	~MSG_150100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550100 : public MSG {
	Create_Guild_Info guild_info;

	MSG_550100(void);
	~MSG_550100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150101 : public MSG {

	MSG_150101(void);
	~MSG_150101();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150102 : public MSG {
	std::string table_name;	//删除数据表名称
	std::vector<int64_t> delete_list;

	MSG_150102(void);
	~MSG_150102();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

#endif /* INNER_MSG_H_ */
