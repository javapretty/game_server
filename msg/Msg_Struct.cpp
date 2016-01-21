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
	w.write_uint32(seq);
	w.write_uint32(id);
}

int Item_Basic_Info::deserialize(Block_Buffer & r) {
	if (r.read_int32(amount) || r.read_uint8(bind) || r.read_uint32(index) || r.read_uint32(seq) || r.read_uint32(id)){
		return -1;
	}
	return 0;
}

void Item_Basic_Info::reset(){
	amount = 0;
	bind = 0;
	index = 0;
	seq = 0;
	id = 0;
}

Money::Money(void){
	reset();
}

void Money::serialize(Block_Buffer & w) const {
	w.write_int32(bind_copper);
	w.write_int32(copper);
	w.write_int32(coupon);
	w.write_int32(gold);
}

int Money::deserialize(Block_Buffer & r) {
	if (r.read_int32(bind_copper) || r.read_int32(copper) || r.read_int32(coupon) || r.read_int32(gold)){
		return -1;
	}
	return 0;
}

void Money::reset(){
	bind_copper = 0;
	copper = 0;
	coupon = 0;
	gold = 0;
}
