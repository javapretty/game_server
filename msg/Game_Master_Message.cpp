/** 
* struct Game_Master_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

#include "Game_Master_Message.h"

MSG_160001::MSG_160001(void) {
	reset();
}

void MSG_160001::serialize(Block_Buffer &buffer) const {
	player_info.serialize(buffer);
}

int MSG_160001::deserialize(Block_Buffer &buffer) {
	player_info.deserialize(buffer);
	return 0;
}

void MSG_160001::reset(void) {
	player_info.reset();
}

MSG_160002::MSG_160002(void) {
	reset();
}

void MSG_160002::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
}

int MSG_160002::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	return 0;
}

void MSG_160002::reset(void) {
	role_id = 0;
}
