#include "Active_Message.h"


MSG_300000::MSG_300000(void){
	reset();
}

void MSG_300000::serialize(Block_Buffer & w) const {
}

int MSG_300000::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_300000::reset(){
}

MSG_300100::MSG_300100(void){
	reset();
}

void MSG_300100::serialize(Block_Buffer & w) const {
	uint16_t __item_info_vec_size = item_info.size();
	w.write_uint16(__item_info_vec_size);
	for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
		item_info[i].serialize(w);
	}

}

int MSG_300100::deserialize(Block_Buffer & r) {
	uint16_t __item_info_vec_size;
	if(r.read_uint16(__item_info_vec_size)  )
		return -1;
	item_info.reserve(__item_info_vec_size);
	for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
		Item_Basic_Info v;
		if(v.deserialize(r))
			return -1;
		item_info.push_back(v);
	}

	return 0;
}

void MSG_300100::reset(){
	item_info.clear();
}

MSG_300101::MSG_300101(void){
	reset();
}

void MSG_300101::serialize(Block_Buffer & w) const {
	w.write_int8(type);
}

int MSG_300101::deserialize(Block_Buffer & r) {
	if (r.read_int8(type)){
		return -1;
	}
	return 0;
}

void MSG_300101::reset(){
	type = 0;
}

MSG_300102::MSG_300102(void){
	reset();
}

void MSG_300102::serialize(Block_Buffer & w) const {
	w.write_int16(capacity);
	w.write_int32(type);
}

int MSG_300102::deserialize(Block_Buffer & r) {
	if (r.read_int16(capacity) || r.read_int32(type)){
		return -1;
	}
	return 0;
}

void MSG_300102::reset(){
	capacity = 0;
	type = 0;
}

MSG_300103::MSG_300103(void){
	reset();
}

void MSG_300103::serialize(Block_Buffer & w) const {
	money.serialize(w);
}

int MSG_300103::deserialize(Block_Buffer & r) {
	if (money.deserialize(r)){
		return -1;
	}
	return 0;
}

void MSG_300103::reset(){
	money.reset();
}

MSG_300104::MSG_300104(void){
	reset();
}

void MSG_300104::serialize(Block_Buffer & w) const {
	uint16_t __item_erased_vec_size = item_erased.size();
	w.write_uint16(__item_erased_vec_size);
	for(uint16_t i = 0; i < __item_erased_vec_size; ++i) {
		w.write_int32(item_erased[i]);
	}

}

int MSG_300104::deserialize(Block_Buffer & r) {
	uint16_t __item_erased_vec_size;
	if(r.read_uint16(__item_erased_vec_size) )
		return -1;
	item_erased.reserve(__item_erased_vec_size);
	for(uint16_t i = 0; i < __item_erased_vec_size; ++i) {
		int32_t v;
		if(r.read_int32(v) )
			return -1;
		item_erased.push_back(v);
	}

	return 0;
}

void MSG_300104::reset(){
	item_erased.clear();
}
