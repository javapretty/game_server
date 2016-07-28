/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.4]
*/

#ifndef __inner_msg__H
#define __inner_msg__H

#include "Block_Buffer.h"
#include "boost/unordered_map.hpp"
#include "Game_Struct.h"

struct MSG_150001;
struct MSG_550001;
struct MSG_150002;
struct MSG_550002;
struct MSG_150003;
struct MSG_550003;
struct MSG_150101;
struct MSG_150102;
struct MSG_150103;
struct MSG_550103;
struct MSG_150104;
struct MSG_550104;
struct MSG_150105;
struct MSG_550105;
struct MSG_180001;
struct MSG_160000;
struct MSG_160001;
struct MSG_160100;
struct MSG_165000;
struct MSG_140000;
struct MSG_140001;
struct MSG_140100;
struct MSG_140200;

struct MSG_150001 : public MSG {
	std::string account;	

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
	Game_Player_Info player_info;	

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

struct MSG_150103 : public MSG {
	std::vector<Guild_Info> guild_list;	

	MSG_150103(void);
	~MSG_150103();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550103 : public MSG {
	std::vector<Guild_Info> guild_list;	

	MSG_550103(void);
	~MSG_550103();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150104 : public MSG {
	std::vector<Offline_Info> offline_list;	

	MSG_150104(void);
	~MSG_150104();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550104 : public MSG {
	std::vector<Offline_Info> offline_list;	

	MSG_550104(void);
	~MSG_550104();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_150105 : public MSG {
	std::vector<Rank_Info> rank_list;	

	MSG_150105(void);
	~MSG_150105();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_550105 : public MSG {
	std::vector<Rank_Info> rank_list;	

	MSG_550105(void);
	~MSG_550105();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_180001 : public MSG {
	int64_t role_id;	
	std::string role_name;	
	std::string account;	
	int32_t level;	
	std::string client_ip;	
	int32_t login_time;	
	int32_t logout_time;	

	MSG_180001(void);
	~MSG_180001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_160000 : public MSG {
	Master_Player_Info player_info;	

	MSG_160000(void);
	~MSG_160000();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_160001 : public MSG {
	int64_t role_id;	

	MSG_160001(void);
	~MSG_160001();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_160100 : public MSG {
	int64_t guild_id;	
	std::string guild_name;	

	MSG_160100(void);
	~MSG_160100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_165000 : public MSG {
	int32_t level;	

	MSG_165000(void);
	~MSG_165000();
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

struct MSG_140100 : public MSG {

	MSG_140100(void);
	~MSG_140100();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

struct MSG_140200 : public MSG {
	Master_Player_Info player_info;	

	MSG_140200(void);
	~MSG_140200();
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
	void print(void);
};

#endif
