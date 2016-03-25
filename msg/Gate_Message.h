/** 
* struct Gate_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#ifndef __Gate_Message__H
#define __Gate_Message__H

#include "Block_Buffer.h"
#include "Public_Struct.h"

struct MSG_140000 {
	std::string account;	
	std::string session;	
	std::string gate_ip;	
	int32_t gate_port;	

	MSG_140000(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_140001 {
	std::string account;	

	MSG_140001(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_140100 {

	MSG_140100(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct MSG_140200 {
	int64_t role_id;	

	MSG_140200(void);
	void serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

#endif
