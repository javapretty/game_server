/** 
* struct Active_Message description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.0]
*
*/

#include "Active_Message.h"

MSG_300000::MSG_300000(void) {
	reset();
}

void MSG_300000::serialize(Block_Buffer &buffer) const {
}

int MSG_300000::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_300000::reset(void) {
}

MSG_300001::MSG_300001(void) {
	reset();
}

void MSG_300001::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(player_level);
	buffer.write_int32(player_exp);
}

int MSG_300001::deserialize(Block_Buffer &buffer) {
	player_level = buffer.read_int32();
	player_exp = buffer.read_int32();
	return 0;
}

void MSG_300001::reset(void) {
	player_level = 0;
	player_exp = 0;
}

MSG_300002::MSG_300002(void) {
	reset();
}

void MSG_300002::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(vip_level);
	buffer.write_int32(vip_exp);
}

int MSG_300002::deserialize(Block_Buffer &buffer) {
	vip_level = buffer.read_int32();
	vip_exp = buffer.read_int32();
	return 0;
}

void MSG_300002::reset(void) {
	vip_level = 0;
	vip_exp = 0;
}

MSG_300100::MSG_300100(void) {
	reset();
}

void MSG_300100::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(copper);
	buffer.write_int32(gold);
}

int MSG_300100::deserialize(Block_Buffer &buffer) {
	copper = buffer.read_int32();
	gold = buffer.read_int32();
	return 0;
}

void MSG_300100::reset(void) {
	copper = 0;
	gold = 0;
}

MSG_300101::MSG_300101(void) {
	reset();
}

void MSG_300101::serialize(Block_Buffer &buffer) const {
	uint16_t item_info_size = item_info.size();
	buffer.write_uint16(item_info_size);
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info[i].serialize(buffer);
	}
}

int MSG_300101::deserialize(Block_Buffer &buffer) {
	uint16_t item_info_size = buffer.read_uint16();
	Item_Info item_info_v;
	for(uint16_t i = 0; i < item_info_size; ++i) {
		item_info_v.deserialize(buffer);
		item_info.push_back(item_info_v);
	}
	return 0;
}

void MSG_300101::reset(void) {
	item_info.clear();
}

MSG_300200::MSG_300200(void) {
	reset();
}

void MSG_300200::serialize(Block_Buffer &buffer) const {
	uint16_t mail_info_size = mail_info.size();
	buffer.write_uint16(mail_info_size);
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info[i].serialize(buffer);
	}
}

int MSG_300200::deserialize(Block_Buffer &buffer) {
	uint16_t mail_info_size = buffer.read_uint16();
	Mail_Detail mail_info_v;
	for(uint16_t i = 0; i < mail_info_size; ++i) {
		mail_info_v.deserialize(buffer);
		mail_info.push_back(mail_info_v);
	}
	return 0;
}

void MSG_300200::reset(void) {
	mail_info.clear();
}

MSG_300300::MSG_300300(void) {
	reset();
}

void MSG_300300::serialize(Block_Buffer &buffer) const {
	uint16_t property_info_size = property_info.size();
	buffer.write_uint16(property_info_size);
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info[i].serialize(buffer);
	}
}

int MSG_300300::deserialize(Block_Buffer &buffer) {
	uint16_t property_info_size = buffer.read_uint16();
	Property_Detail property_info_v;
	for(uint16_t i = 0; i < property_info_size; ++i) {
		property_info_v.deserialize(buffer);
		property_info.push_back(property_info_v);
	}
	return 0;
}

void MSG_300300::reset(void) {
	property_info.clear();
}
