/** 
* struct Client_Master_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#ifndef __Client_Master_Message__H
#define __Client_Master_Message__H

#include "Block_Buffer.h"

struct MSG_110001 {
	int8_t chat_type;	//1
	std::string chat_content;	
	std::string role_name;	//私聊时候发送对方名字

	MSG_110001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_510001 {
	int8_t chat_type;	
	std::string chat_content;	
	std::string role_name;	
	int32_t career;	
	int32_t gender;	

	MSG_510001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
