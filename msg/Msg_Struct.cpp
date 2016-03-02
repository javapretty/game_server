#include "Msg_Struct.h"


Role_Info::Role_Info(void){
	reset();
}

void Role_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(level);
	buffer.write_int32(exp);
	buffer.write_int32(career);
	buffer.write_int32(gender);
	buffer.write_int32(vitality);
	buffer.write_int32(vip);
	buffer.write_int32(charge_gold);
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
}

int Role_Info::deserialize(Block_Buffer &buffer) {
	level = buffer.read_int32();
	exp = buffer.read_int32();
	career = buffer.read_int32();
	gender = buffer.read_int32();
	vitality = buffer.read_int32();
	vip = buffer.read_int32();
	charge_gold = buffer.read_int32();
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	return 0;
}

void Role_Info::reset(){
	level = 0;
	exp = 0;
	career = 0;
	gender = 0;
	vitality = 0;
	vip = 0;
	charge_gold = 0;
	role_id = 0;
	account.clear();
	role_name.clear();
}

Hero_Detail::Hero_Detail(void){
	reset();
}

void Hero_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(hero_id);
	buffer.write_int32(level);
	buffer.write_int32(exp);
	buffer.write_int32(star);
	buffer.write_int32(quality);
	buffer.write_int32(power);
	buffer.write_int32(brains);
	buffer.write_int32(agile);
}

int Hero_Detail::deserialize(Block_Buffer &buffer) {
	hero_id = buffer.read_int32();
	level = buffer.read_int32();
	exp = buffer.read_int32();
	star = buffer.read_int32();
	quality = buffer.read_int32();
	power = buffer.read_int32();
	brains = buffer.read_int32();
	agile = buffer.read_int32();
	return 0;
}

void Hero_Detail::reset(){
	hero_id = 0;
	level = 0;
	exp = 0;
	star = 0;
	quality = 0;
	power = 0;
	brains = 0;
	agile = 0;
}

Item_Basic_Info::Item_Basic_Info(void){
	reset();
}

void Item_Basic_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(id);
	buffer.write_int32(amount);
}

int Item_Basic_Info::deserialize(Block_Buffer &buffer) {
	id = buffer.read_int32();
	amount = buffer.read_int32();
	return 0;
}

void Item_Basic_Info::reset(){
	id = 0;
	amount = 0;
}

Mail_Detail::Mail_Detail(void){
	reset();
}

void Mail_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(mail_id);
	buffer.write_int32(send_time);
	buffer.write_int32(sender_type);
	buffer.write_int32(copper);
	buffer.write_int32(gold);
	buffer.write_int64(sender_id);
	buffer.write_bool(pickup);
	buffer.write_string(sender_name);
	buffer.write_string(mail_title);
	buffer.write_string(mail_content);
}

int Mail_Detail::deserialize(Block_Buffer &buffer) {
	mail_id = buffer.read_int32();
	send_time = buffer.read_int32();
	sender_type = buffer.read_int32();
	copper = buffer.read_int32();
	gold = buffer.read_int32();
	sender_id = buffer.read_int64();
	pickup = buffer.read_bool();
	sender_name = buffer.read_string();
	mail_title = buffer.read_string();
	mail_content = buffer.read_string();
	return 0;
}

void Mail_Detail::reset(){
	mail_id = 0;
	send_time = 0;
	sender_type = 0;
	copper = 0;
	gold = 0;
	sender_id = 0;
	pickup = false;
	sender_name.clear();
	mail_title.clear();
	mail_content.clear();
}
