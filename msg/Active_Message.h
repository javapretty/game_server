/**
 * struct Active_Message description
 *
 * [This file was auto-generated. Please Do not edit]
 *
 */

#ifndef ACTIVE_MESSAGE_H_
#define ACTIVE_MESSAGE_H_

#include "Block_Buffer.h"
#include "Msg_Struct.h"



	//服务器主动断开连接
struct MSG_300000 {

	MSG_300000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

	//更新vip信息
struct MSG_300001 {

	int32_t vip_level;

	int32_t vip_exp;

	MSG_300001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

	//物品信息更新
struct MSG_300100 {

	std::vector<Item_Info> item_info;

	MSG_300100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

	//物品附加信息更新
struct MSG_300101 {

	

	MSG_300101(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

	//金钱更新
struct MSG_300102 {

	int32_t copper;

	int32_t gold;

	MSG_300102(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

	//邮件信息更新
struct MSG_300200 {

	std::vector<Mail_Detail> mail_info;

	MSG_300200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
