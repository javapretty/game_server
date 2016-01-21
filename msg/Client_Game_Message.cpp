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
	if (r.read_int32(agent_num) || r.read_int32(server_num) || r.read_int64(role_id) || r.read_string(account) || r.read_string(timestamp) || r.read_string(ip)){
		return -1;
	}
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
	if (role_info.deserialize(r)){
		return -1;
	}
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
	if (r.read_int32(agent_num) || r.read_int32(server_num) || r.read_uint8(gender) || r.read_string(account) || r.read_string(role_name)){
		return -1;
	}
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
	if (r.read_int64(role_id)){
		return -1;
	}
	return 0;
}

void MSG_520002::reset(){
	role_id = 0;
}

MSG_120100::MSG_120100(void){
	reset();
}

void MSG_120100::serialize(Block_Buffer & w) const {
	w.write_uint32(pack_type);
}

int MSG_120100::deserialize(Block_Buffer & r) {
	if (r.read_uint32(pack_type)){
		return -1;
	}
	return 0;
}

void MSG_120100::reset(){
	pack_type = 0;
}

MSG_120101::MSG_120101(void){
	reset();
}

void MSG_120101::serialize(Block_Buffer & w) const {
	w.write_uint8(pay_type);
	w.write_uint16(add_capacity);
	w.write_uint32(pack_type);
}

int MSG_120101::deserialize(Block_Buffer & r) {
	if (r.read_uint8(pay_type) || r.read_uint16(add_capacity) || r.read_uint32(pack_type)){
		return -1;
	}
	return 0;
}

void MSG_120101::reset(){
	pay_type = 0;
	add_capacity = 0;
	pack_type = 0;
}

MSG_120102::MSG_120102(void){
	reset();
}

void MSG_120102::serialize(Block_Buffer & w) const {
	uint16_t __index_vec_size = index.size();
	w.write_uint16(__index_vec_size);
	for(uint16_t i = 0; i < __index_vec_size; ++i) {
		w.write_int32(index[i]);
	}

}

int MSG_120102::deserialize(Block_Buffer & r) {
	uint16_t __index_vec_size;
	if(r.read_uint16(__index_vec_size) )
		return -1;
	index.reserve(__index_vec_size);
	for(uint16_t i = 0; i < __index_vec_size; ++i) {
		int32_t v;
		if(r.read_int32(v) )
			return -1;
		index.push_back(v);
	}

	return 0;
}

void MSG_120102::reset(){
	index.clear();
}

MSG_120103::MSG_120103(void){
	reset();
}

void MSG_120103::serialize(Block_Buffer & w) const {
	w.write_uint32(index_from);
	w.write_uint32(index_to);
}

int MSG_120103::deserialize(Block_Buffer & r) {
	if (r.read_uint32(index_from) || r.read_uint32(index_to)){
		return -1;
	}
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
	if (r.read_int16(split_num) || r.read_uint32(index)){
		return -1;
	}
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
	w.write_uint32(pack_type);
}

int MSG_120105::deserialize(Block_Buffer & r) {
	if (r.read_uint32(pack_type)){
		return -1;
	}
	return 0;
}

void MSG_120105::reset(){
	pack_type = 0;
}

MSG_120106::MSG_120106(void){
	reset();
}

void MSG_120106::serialize(Block_Buffer & w) const {
	w.write_int32(amount);
	w.write_int32(e_value);
	w.write_uint32(index);
}

int MSG_120106::deserialize(Block_Buffer & r) {
	if (r.read_int32(amount) || r.read_int32(e_value) || r.read_uint32(index)){
		return -1;
	}
	return 0;
}

void MSG_120106::reset(){
	amount = 0;
	e_value = 0;
	index = 0;
}

MSG_120107::MSG_120107(void){
	reset();
}

void MSG_120107::serialize(Block_Buffer & w) const {
	w.write_int32(item_index);
	uint16_t __select_index_vector_vec_size = select_index_vector.size();
	w.write_uint16(__select_index_vector_vec_size);
	for(uint16_t i = 0; i < __select_index_vector_vec_size; ++i) {
		w.write_int32(select_index_vector[i]);
	}

}

int MSG_120107::deserialize(Block_Buffer & r) {
	uint16_t __select_index_vector_vec_size;
	if(r.read_uint16(__select_index_vector_vec_size) )
		return -1;
	select_index_vector.reserve(__select_index_vector_vec_size);
	for(uint16_t i = 0; i < __select_index_vector_vec_size; ++i) {
		int32_t v;
		if(r.read_int32(v) )
			return -1;
		select_index_vector.push_back(v);
	}

	if (r.read_int32(item_index)){
		return -1;
	}
	return 0;
}

void MSG_120107::reset(){
	item_index = 0;
	select_index_vector.clear();
}

MSG_120108::MSG_120108(void){
	reset();
}

void MSG_120108::serialize(Block_Buffer & w) const {
	w.write_uint32(pack_type);
}

int MSG_120108::deserialize(Block_Buffer & r) {
	if (r.read_uint32(pack_type)){
		return -1;
	}
	return 0;
}

void MSG_120108::reset(){
	pack_type = 0;
}

MSG_120109::MSG_120109(void){
	reset();
}

void MSG_120109::serialize(Block_Buffer & w) const {
	uint16_t __index_vec_size = index.size();
	w.write_uint16(__index_vec_size);
	for(uint16_t i = 0; i < __index_vec_size; ++i) {
		w.write_int32(index[i]);
	}

}

int MSG_120109::deserialize(Block_Buffer & r) {
	uint16_t __index_vec_size;
	if(r.read_uint16(__index_vec_size) )
		return -1;
	index.reserve(__index_vec_size);
	for(uint16_t i = 0; i < __index_vec_size; ++i) {
		int32_t v;
		if(r.read_int32(v) )
			return -1;
		index.push_back(v);
	}

	return 0;
}

void MSG_120109::reset(){
	index.clear();
}

MSG_520100::MSG_520100(void){
	reset();
}

void MSG_520100::serialize(Block_Buffer & w) const {
	w.write_int16(capacity);
	w.write_uint32(pack_type);
	uint16_t __item_info_vec_size = item_info.size();
	w.write_uint16(__item_info_vec_size);
	for(uint16_t i = 0; i < __item_info_vec_size; ++i) {
		item_info[i].serialize(w);
	}

}

int MSG_520100::deserialize(Block_Buffer & r) {
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

	if (r.read_int16(capacity) || r.read_uint32(pack_type)){
		return -1;
	}
	return 0;
}

void MSG_520100::reset(){
	capacity = 0;
	pack_type = 0;
	item_info.clear();
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
	w.write_uint32(id);
}

int MSG_520106::deserialize(Block_Buffer & r) {
	if (r.read_uint32(id)){
		return -1;
	}
	return 0;
}

void MSG_520106::reset(){
	id = 0;
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

MSG_520108::MSG_520108(void){
	reset();
}

void MSG_520108::serialize(Block_Buffer & w) const {
}

int MSG_520108::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520108::reset(){
}

MSG_520109::MSG_520109(void){
	reset();
}

void MSG_520109::serialize(Block_Buffer & w) const {
}

int MSG_520109::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_520109::reset(){
}
