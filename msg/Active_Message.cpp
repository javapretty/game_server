#include "Active_Message.h"


MSG_300000::MSG_300000(void){
	reset();
}

void MSG_300000::serialize(Block_Buffer &buffer) const {
}

int MSG_300000::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_300000::reset(){
}

MSG_300100::MSG_300100(void){
	reset();
}

void MSG_300100::serialize(Block_Buffer &buffer) const {
	uint16_t __item_info_vec_vec_size = item_info_vec.size();
	buffer.write_uint16(__item_info_vec_vec_size);
	for(uint16_t i = 0; i < __item_info_vec_vec_size; ++i) {
		item_info_vec[i].serialize(buffer);
	}

}

int MSG_300100::deserialize(Block_Buffer &buffer) {
	uint16_t __item_info_vec_vec_size = buffer.read_uint16();
	Item_Basic_Info v;
	for(uint16_t i = 0; i < __item_info_vec_vec_size; ++i) {
		if(v.deserialize(buffer))
			return -1;
		item_info_vec.push_back(v);
	}

	return 0;
}

void MSG_300100::reset(){
	item_info_vec.clear();
}

MSG_300101::MSG_300101(void){
	reset();
}

void MSG_300101::serialize(Block_Buffer &buffer) const {
}

int MSG_300101::deserialize(Block_Buffer &buffer) {
	return 0;
}

void MSG_300101::reset(){
}

MSG_300102::MSG_300102(void){
	reset();
}

void MSG_300102::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(copper);
	buffer.write_int32(gold);
}

int MSG_300102::deserialize(Block_Buffer &buffer) {
	copper = buffer.read_int32();
	gold = buffer.read_int32();
	return 0;
}

void MSG_300102::reset(){
	copper = 0;
	gold = 0;
}

MSG_300200::MSG_300200(void){
	reset();
}

void MSG_300200::serialize(Block_Buffer &buffer) const {
	uint16_t __mail_detail_vec_vec_size = mail_detail_vec.size();
	buffer.write_uint16(__mail_detail_vec_vec_size);
	for(uint16_t i = 0; i < __mail_detail_vec_vec_size; ++i) {
		mail_detail_vec[i].serialize(buffer);
	}

}

int MSG_300200::deserialize(Block_Buffer &buffer) {
	uint16_t __mail_detail_vec_vec_size = buffer.read_uint16();
	Mail_Detail v;
	for(uint16_t i = 0; i < __mail_detail_vec_vec_size; ++i) {
		if(v.deserialize(buffer))
			return -1;
		mail_detail_vec.push_back(v);
	}

	return 0;
}

void MSG_300200::reset(){
	mail_detail_vec.clear();
}
