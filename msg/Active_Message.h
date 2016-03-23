/** 
* struct Active_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#ifndef __Active_Message__H
#define __Active_Message__H

#include "Block_Buffer.h"
#include "Msg_Struct.h"

struct MSG_300000 {

	MSG_300000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_300001 {
	int32_t player_level;	
	int32_t player_exp;	

	MSG_300001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_300002 {
	int32_t vip_level;	
	int32_t vip_exp;	

	MSG_300002(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_300100 {
	int32_t copper;	
	int32_t gold;	

	MSG_300100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_300101 {
	std::vector<Item_Info> item_info;	

	MSG_300101(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_300200 {
	std::vector<Mail_Detail> mail_info;	

	MSG_300200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_300300 {
	std::vector<Property_Detail> property_info;	

	MSG_300300(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
