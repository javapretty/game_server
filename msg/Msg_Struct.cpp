#include "Msg_Struct.h"


Role_Info::Role_Info(void){
	reset();
}

void Role_Info::serialize(Block_Buffer & w) const {
	w.write_int64(role_id);
	w.write_uint8(career);
	w.write_uint8(gender);
	w.write_uint16(level);
	w.write_string(account);
	w.write_string(role_name);
}

int Role_Info::deserialize(Block_Buffer & r) {
	if (r.read_int64(role_id) || r.read_uint8(career) || r.read_uint8(gender) || r.read_uint16(level) || r.read_string(account) || r.read_string(role_name)){
		return -1;
	}
	return 0;
}

void Role_Info::reset(){
	role_id = 0;
	career = 0;
	gender = 0;
	level = 0;
	account.clear();
	role_name.clear();
}

Item_Basic_Info::Item_Basic_Info(void){
	reset();
}

void Item_Basic_Info::serialize(Block_Buffer & w) const {
	w.write_int32(amount);
	w.write_uint8(bind);
	w.write_uint32(index);
	w.write_uint32(id);
}

int Item_Basic_Info::deserialize(Block_Buffer & r) {
	if (r.read_int32(amount) || r.read_uint8(bind) || r.read_uint32(index) || r.read_uint32(id)){
		return -1;
	}
	return 0;
}

void Item_Basic_Info::reset(){
	amount = 0;
	bind = 0;
	index = 0;
	id = 0;
}

Money_Info::Money_Info(void){
	reset();
}

void Money_Info::serialize(Block_Buffer & w) const {
	w.write_int32(bind_copper);
	w.write_int32(copper);
	w.write_int32(coupon);
	w.write_int32(gold);
}

int Money_Info::deserialize(Block_Buffer & r) {
	if (r.read_int32(bind_copper) || r.read_int32(copper) || r.read_int32(coupon) || r.read_int32(gold)){
		return -1;
	}
	return 0;
}

void Money_Info::reset(){
	bind_copper = 0;
	copper = 0;
	coupon = 0;
	gold = 0;
}

Mail_Detail::Mail_Detail(void){
	reset();
}

void Mail_Detail::serialize(Block_Buffer & w) const {
	w.write_int8(pickup);
	w.write_int32(mail_id);
	w.write_int32(send_time);
	w.write_int32(sender_type);
	w.write_int64(sender_id);
	w.write_string(sender_name);
	w.write_string(mail_title);
	w.write_string(mail_content);
	money_info.serialize(w);
	uint16_t __item_vector_vec_size = item_vector.size();
	w.write_uint16(__item_vector_vec_size);
	for(uint16_t i = 0; i < __item_vector_vec_size; ++i) {
		item_vector[i].serialize(w);
	}

}

int Mail_Detail::deserialize(Block_Buffer & r) {
	uint16_t __item_vector_vec_size;
	if(r.read_uint16(__item_vector_vec_size)  )
		return -1;
	item_vector.reserve(__item_vector_vec_size);
	for(uint16_t i = 0; i < __item_vector_vec_size; ++i) {
		Item_Basic_Info v;
		if(v.deserialize(r))
			return -1;
		item_vector.push_back(v);
	}

	if (r.read_int8(pickup) || r.read_int32(mail_id) || r.read_int32(send_time) || r.read_int32(sender_type) || r.read_int64(sender_id) || r.read_string(sender_name) || r.read_string(mail_title) || r.read_string(mail_content) || money_info.deserialize(r)){
		return -1;
	}
	return 0;
}

void Mail_Detail::reset(){
	pickup = 0;
	mail_id = 0;
	send_time = 0;
	sender_type = 0;
	sender_id = 0;
	sender_name.clear();
	mail_title.clear();
	mail_content.clear();
	money_info.reset();
	item_vector.clear();
}
