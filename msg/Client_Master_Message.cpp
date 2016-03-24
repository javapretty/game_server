/** 
* struct Client_Master_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#include "Client_Master_Message.h"

MSG_110001::MSG_110001(void) {
	reset();
}

void MSG_110001::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(chat_type);
	buffer.write_string(chat_content);
	buffer.write_string(role_name);
}

int MSG_110001::deserialize(Block_Buffer &buffer) {
	chat_type = buffer.read_int8();
	chat_content = buffer.read_string();
	role_name = buffer.read_string();
	return 0;
}

void MSG_110001::reset(void) {
	chat_type = 0;
	chat_content.clear();
	role_name.clear();
}

MSG_510001::MSG_510001(void) {
	reset();
}

void MSG_510001::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(chat_type);
	buffer.write_string(chat_content);
	buffer.write_string(role_name);
	buffer.write_int32(career);
	buffer.write_int32(gender);
}

int MSG_510001::deserialize(Block_Buffer &buffer) {
	chat_type = buffer.read_int8();
	chat_content = buffer.read_string();
	role_name = buffer.read_string();
	career = buffer.read_int32();
	gender = buffer.read_int32();
	return 0;
}

void MSG_510001::reset(void) {
	chat_type = 0;
	chat_content.clear();
	role_name.clear();
	career = 0;
	gender = 0;
}
