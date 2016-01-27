#include "Client_Game_Message.h"


MSG_120001::MSG_120001(void){
	reset();
}

void MSG_120001::serialize(Block_Buffer & w) const {
	w.write_int32(agent_num);
	w.write_int32(server_num);
	w.write_int64(role_id);
	w.write_string(account);
	w.write_string(timestamp);
	w.write_string(ip);
}

int MSG_120001::deserialize(Block_Buffer & r) {
	agent_num = r.read_int32();
	server_num = r.read_int32();
	role_id = r.read_int64();
	account = r.read_string();
	timestamp = r.read_string();
	ip = r.read_string();
	return 0;
}

void MSG_120001::reset(){
	agent_num = 0;
	server_num = 0;
	role_id = 0;
	account.clear();
	timestamp.clear();
	ip.clear();
}

MSG_520001::MSG_520001(void){
	reset();
}

void MSG_520001::serialize(Block_Buffer & w) const {
	role_info.serialize(w);
}

int MSG_520001::deserialize(Block_Buffer & r) {
	role_info.deserialize(r);
	return 0;
}

void MSG_520001::reset(){
	role_info.reset();
}

MSG_120002::MSG_120002(void){
	reset();
}

void MSG_120002::serialize(Block_Buffer & w) const {
	w.write_int32(agent_num);
	w.write_int32(server_num);
	w.write_uint8(gender);
	w.write_string(account);
	w.write_string(role_name);
}

int MSG_120002::deserialize(Block_Buffer & r) {
	agent_num = r.read_int32();
	server_num = r.read_int32();
	gender = r.read_uint8();
	account = r.read_string();
	role_name = r.read_string();
	return 0;
}

void MSG_120002::reset(){
	agent_num = 0;
	server_num = 0;
	gender = 0;
	account.clear();
	role_name.clear();
}

MSG_520002::MSG_520002(void){
	reset();
}

void MSG_520002::serialize(Block_Buffer & w) const {
	w.write_int64(role_id);
}

int MSG_520002::deserialize(Block_Buffer & r) {
	role_id = r.read_int64();
	return 0;
}

void MSG_520002::reset(){
	role_id = 0;
}

MSG_120100::MSG_120100(void){
	reset();
}

void MSG_120100::serialize(Block_Buffer & w) const {
	w.write_uint32(bag_type);
}

int MSG_120100::deserialize(Block_Buffer & r) {
	bag_type = r.read_uint32();
	return 0;
}

void MSG_120100::reset(){
	bag_type = 0;
}

MSG_120101::MSG_120101(void){
	reset();
}

void MSG_120101::serialize(Block_Buffer & w) const {
	w.write_uint8(pay_type);
	w.write_uint16(add_capacity);
	w.write_uint32(bag_type);
}

int MSG_120101::deserialize(Block_Buffer & r) {
	pay_type = r.read_uint8();
	add_capacity = r.read_uint16();
	bag_type = r.read_uint32();
	return 0;
}

void MSG_120101::reset(){
	pay_type = 0;
	add_capacity = 0;
	bag_type = 0;
}

MSG_120102::MSG_120102(void){
	reset();
}

void MSG_120102::serialize(Block_Buffer & w) const {
	uint16_t item_index_vec_size = item_index_vec.size();
	w.write_uint16(item_index_vec_size);
	for(uint16_t i = 0; i < item_index_vec_size; ++i) {
		w.write_uint32(item_index_vec[i]);
	}

}

int MSG_120102::deserialize(Block_Buffer & r) {
	uint16_t item_index_vec_size = r.read_uint16();
	for(uint16_t i = 0; i < item_index_vec_size; ++i) {
		uint32_t v = r.read_uint32();
		item_index_vec.push_back(v);
	}
	return 0;
}

void MSG_120102::reset(){
	item_index_vec.clear();
}

MSG_120103::MSG_120103(void){
	reset();
}

void MSG_120103::serialize(Block_Buffer & w) const {
	w.write_uint32(index_from);
	w.write_uint32(index_to);
}

int MSG_120103::deserialize(Block_Buffer & r) {
	index_from = r.read_uint32();
	index_to = r.read_uint32();
	return 0;
}

void MSG_120103::reset(){
	index_from = 0;
	index_to = 0;
}

MSG_120104::MSG_120104(void){
	reset();
}

void MSG_120104::serialize(Block_Buffer & w) const {
	w.write_int16(split_num);
	w.write_uint32(index);
}

int MSG_120104::deserialize(Block_Buffer & r) {
	split_num = r.read_int16();
	index = r.read_uint32();
	return 0;
}

void MSG_120104::reset(){
	split_num = 0;
	index = 0;
}

MSG_120105::MSG_120105(void){
	reset();
}

void MSG_120105::serialize(Block_Buffer & w) const {
	w.write_uint32(bag_type);
}

int MSG_120105::deserialize(Block_Buffer & r) {
	bag_type = r.read_uint32();
	return 0;
}

void MSG_120105::reset(){
	bag_type = 0;
}

MSG_120106::MSG_120106(void){
	reset();
}

void MSG_120106::serialize(Block_Buffer & w) const {
	w.write_uint32(bag_type);
}

int MSG_120106::deserialize(Block_Buffer & r) {
	bag_type = r.read_uint32();
	return 0;
}

void MSG_120106::reset(){
	bag_type = 0;
}

MSG_120107::MSG_120107(void){
	reset();
}

void MSG_120107::serialize(Block_Buffer & w) const {
	uint16_t item_index_vec_size = item_index_vec.size();
	w.write_uint16(item_index_vec_size);
	for(uint16_t i = 0; i < item_index_vec_size; ++i) {
		w.write_uint32(item_index_vec[i]);
	}

}

int MSG_120107::deserialize(Block_Buffer & r) {
	uint16_t item_index_vec_size = r.read_uint16();
	for(uint16_t i = 0; i < item_index_vec_size; ++i) {
		uint32_t v = r.read_uint32();
		item_index_vec.push_back(v);
	}
	return 0;
}

void MSG_120107::reset(){
	item_index_vec.clear();
}

MSG_520100::MSG_520100(void){
	reset();
}

void MSG_520100::serialize(Block_Buffer & w) const {
	w.write_int16(capacity);
	w.write_uint32(bag_type);
	uint16_t __item_info_vec_vec_size = item_info_vec.size();
	w.write_uint16(__item_info_vec_vec_size);
	for(uint16_t i = 0; i < __item_info_vec_vec_size; ++i) {
		item_info_vec[i].serialize(w);
	}

}

int MSG_520100::deserialize(Block_Buffer & r) {
	capacity = r.read_int16();
	bag_type = r.read_uint32();
	uint16_t __item_info_vec_vec_size = r.read_uint16();
	Item_Basic_Info v;
	for(uint16_t i = 0; i < __item_info_vec_vec_size; ++i) {
		if(v.deserialize(r))
			return -1;
		item_info_vec.push_back(v);
	}

	return 0;
}

void MSG_520100::reset(){
	capacity = 0;
	bag_type = 0;
	item_info_vec.clear();
}

MSG_520101::MSG_520101(void){
	reset();
}

void MSG_520101::serialize(Block_Buffer & w) const {
}

int MSG_520101::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520101::reset(){
}

MSG_520102::MSG_520102(void){
	reset();
}

void MSG_520102::serialize(Block_Buffer & w) const {
}

int MSG_520102::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520102::reset(){
}

MSG_520103::MSG_520103(void){
	reset();
}

void MSG_520103::serialize(Block_Buffer & w) const {
}

int MSG_520103::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520103::reset(){
}

MSG_520104::MSG_520104(void){
	reset();
}

void MSG_520104::serialize(Block_Buffer & w) const {
}

int MSG_520104::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520104::reset(){
}

MSG_520106::MSG_520106(void){
	reset();
}

void MSG_520106::serialize(Block_Buffer & w) const {
}

int MSG_520106::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520106::reset(){
}

MSG_520107::MSG_520107(void){
	reset();
}

void MSG_520107::serialize(Block_Buffer & w) const {
}

int MSG_520107::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520107::reset(){
}

MSG_120200::MSG_120200(void){
	reset();
}

void MSG_120200::serialize(Block_Buffer & w) const {
}

int MSG_120200::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_120200::reset(){
}

MSG_120201::MSG_120201(void){
	reset();
}

void MSG_120201::serialize(Block_Buffer & w) const {
	w.write_int32(mail_id);
}

int MSG_120201::deserialize(Block_Buffer & r) {
	mail_id = r.read_int32();
	return 0;
}

void MSG_120201::reset(){
	mail_id = 0;
}

MSG_120202::MSG_120202(void){
	reset();
}

void MSG_120202::serialize(Block_Buffer & w) const {
	w.write_int32(mail_id);
}

int MSG_120202::deserialize(Block_Buffer & r) {
	mail_id = r.read_int32();
	return 0;
}

void MSG_120202::reset(){
	mail_id = 0;
}

MSG_120203::MSG_120203(void){
	reset();
}

void MSG_120203::serialize(Block_Buffer & w) const {
	w.write_string(receiver_name);
	mail_detail.serialize(w);
}

int MSG_120203::deserialize(Block_Buffer & r) {
	receiver_name = r.read_string();
	mail_detail.deserialize(r);
	return 0;
}

void MSG_120203::reset(){
	receiver_name.clear();
	mail_detail.reset();
}

MSG_520200::MSG_520200(void){
	reset();
}

void MSG_520200::serialize(Block_Buffer & w) const {
	uint16_t __mail_detail_vec_vec_size = mail_detail_vec.size();
	w.write_uint16(__mail_detail_vec_vec_size);
	for(uint16_t i = 0; i < __mail_detail_vec_vec_size; ++i) {
		mail_detail_vec[i].serialize(w);
	}

}

int MSG_520200::deserialize(Block_Buffer & r) {
	uint16_t __mail_detail_vec_vec_size = r.read_uint16();
	Mail_Detail v;
	for(uint16_t i = 0; i < __mail_detail_vec_vec_size; ++i) {
		if(v.deserialize(r))
			return -1;
		mail_detail_vec.push_back(v);
	}

	return 0;
}

void MSG_520200::reset(){
	mail_detail_vec.clear();
}

MSG_520201::MSG_520201(void){
	reset();
}

void MSG_520201::serialize(Block_Buffer & w) const {
	uint16_t mail_id_vec_size = mail_id_vec.size();
	w.write_uint16(mail_id_vec_size);
	for(uint16_t i = 0; i < mail_id_vec_size; ++i) {
		w.write_int32(mail_id_vec[i]);
	}

}

int MSG_520201::deserialize(Block_Buffer & r) {
	uint16_t mail_id_vec_size = r.read_uint16();
	for(uint16_t i = 0; i < mail_id_vec_size; ++i) {
		int32_t v = r.read_int32();
		mail_id_vec.push_back(v);
	}
	return 0;
}

void MSG_520201::reset(){
	mail_id_vec.clear();
}

MSG_520202::MSG_520202(void){
	reset();
}

void MSG_520202::serialize(Block_Buffer & w) const {
	uint16_t mail_id_vec_size = mail_id_vec.size();
	w.write_uint16(mail_id_vec_size);
	for(uint16_t i = 0; i < mail_id_vec_size; ++i) {
		w.write_int32(mail_id_vec[i]);
	}

}

int MSG_520202::deserialize(Block_Buffer & r) {
	uint16_t mail_id_vec_size = r.read_uint16();
	for(uint16_t i = 0; i < mail_id_vec_size; ++i) {
		int32_t v = r.read_int32();
		mail_id_vec.push_back(v);
	}
	return 0;
}

void MSG_520202::reset(){
	mail_id_vec.clear();
}

MSG_520203::MSG_520203(void){
	reset();
}

void MSG_520203::serialize(Block_Buffer & w) const {
}

int MSG_520203::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520203::reset(){
}
