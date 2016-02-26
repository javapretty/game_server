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


struct MSG_300000 {

	MSG_300000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	背包物品更新
*/
struct MSG_300100 {

	std::vector<Item_Basic_Info> item_info_vec;

	MSG_300100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	物品附加信息更新
*/
struct MSG_300101 {

	

	MSG_300101(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	金钱更新
*/
struct MSG_300102 {

	int32_t copper;

	int32_t gold;

	MSG_300102(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};
/*

	收到邮件通知
*/
struct MSG_300200 {

	std::vector<Mail_Detail> mail_detail_vec;

	MSG_300200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};


#endif
