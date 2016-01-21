#include "Game_DB_Message.h"


MSG_150000::MSG_150000(void){
	reset();
}

void MSG_150000::serialize(Block_Buffer & w) const {
}

int MSG_150000::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_150000::reset(){
}

MSG_550000::MSG_550000(void){
	reset();
}

void MSG_550000::serialize(Block_Buffer & w) const {
	uint16_t __cache_vector_vec_size = cache_vector.size();
	w.write_uint16(__cache_vector_vec_size);
	for(uint16_t i = 0; i < __cache_vector_vec_size; ++i) {
		cache_vector[i].serialize(w);
	}

}

int MSG_550000::deserialize(Block_Buffer & r) {
	uint16_t __cache_vector_vec_size;
	if(r.read_uint16(__cache_vector_vec_size)  )
		return -1;
	cache_vector.reserve(__cache_vector_vec_size);
	for(uint16_t i = 0; i < __cache_vector_vec_size; ++i) {
		Player_DB_Cache v;
		if(v.deserialize(r))
			return -1;
		cache_vector.push_back(v);
	}

	return 0;
}

void MSG_550000::reset(){
	cache_vector.clear();
}

MSG_150001::MSG_150001(void){
	reset();
}

void MSG_150001::serialize(Block_Buffer & w) const {
	account_info.serialize(w);
}

int MSG_150001::deserialize(Block_Buffer & r) {
	if (account_info.deserialize(r)){
		return -1;
	}
	return 0;
}

void MSG_150001::reset(){
	account_info.reset();
}

MSG_550001::MSG_550001(void){
	reset();
}

void MSG_550001::serialize(Block_Buffer & w) const {
	player_data.serialize(w);
}

int MSG_550001::deserialize(Block_Buffer & r) {
	if (player_data.deserialize(r)){
		return -1;
	}
	return 0;
}

void MSG_550001::reset(){
	player_data.reset();
}

MSG_150002::MSG_150002(void){
	reset();
}

void MSG_150002::serialize(Block_Buffer & w) const {
	player_info.serialize(w);
}

int MSG_150002::deserialize(Block_Buffer & r) {
	if (player_info.deserialize(r)){
		return -1;
	}
	return 0;
}

void MSG_150002::reset(){
	player_info.reset();
}

MSG_550002::MSG_550002(void){
	reset();
}

void MSG_550002::serialize(Block_Buffer & w) const {
	player_data.serialize(w);
}

int MSG_550002::deserialize(Block_Buffer & r) {
	if (player_data.deserialize(r)){
		return -1;
	}
	return 0;
}

void MSG_550002::reset(){
	player_data.reset();
}

MSG_150003::MSG_150003(void){
	reset();
}

void MSG_150003::serialize(Block_Buffer & w) const {
	player_data.serialize(w);
}

int MSG_150003::deserialize(Block_Buffer & r) {
	if (player_data.deserialize(r)){
		return -1;
	}
	return 0;
}

void MSG_150003::reset(){
	player_data.reset();
}

MSG_550003::MSG_550003(void){
	reset();
}

void MSG_550003::serialize(Block_Buffer & w) const {
	w.write_int64(role_id);
}

int MSG_550003::deserialize(Block_Buffer & r) {
	if (r.read_int64(role_id)){
		return -1;
	}
	return 0;
}

void MSG_550003::reset(){
	role_id = 0;
}
