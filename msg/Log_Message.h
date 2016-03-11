/** 
* struct Log_Message description
* 
* [This file was auto-generated. Please Do not edit]
*
*/

#ifndef __Log_Message__H
#define __Log_Message__H

#include "Block_Buffer.h"
#include "boost/unordered_map.hpp"
#include "Time_Value.h"
#include "Msg_Struct.h"
#include "Public_Struct.h"

struct MSG_180000 {
	int32_t id;	
	std::string name;	

	MSG_180000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_180001 {
	int64_t role_id;	
	std::string role_name;	
	std::string account;	
	uint16_t level;	
	std::string client_ip;	
	uint32_t login_time;	
	uint32_t logout_time;	
	uint32_t online_time;	

	MSG_180001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
