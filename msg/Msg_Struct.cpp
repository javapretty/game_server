#include "Msg_Struct.h"


Role_Info::Role_Info(void){
	reset();
}

void Role_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_uint8(career);
	buffer.write_uint8(gender);
	buffer.write_uint16(level);
	buffer.write_string(account);
	buffer.write_string(role_name);
}

int Role_Info::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	career = buffer.read_uint8();
	gender = buffer.read_uint8();
	level = buffer.read_uint16();
	account = buffer.read_string();
	role_name = buffer.read_string();
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

void Item_Basic_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(amount);
	buffer.write_uint8(bind);
	buffer.write_uint32(index);
	buffer.write_uint32(id);
}

int Item_Basic_Info::deserialize(Block_Buffer &buffer) {
	amount = buffer.read_int32();
	bind = buffer.read_uint8();
	index = buffer.read_uint32();
	id = buffer.read_uint32();
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

void Money_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(bind_copper);
	buffer.write_int32(copper);
	buffer.write_int32(bind_gold);
	buffer.write_int32(gold);
}

int Money_Info::deserialize(Block_Buffer &buffer) {
	bind_copper = buffer.read_int32();
	copper = buffer.read_int32();
	bind_gold = buffer.read_int32();
	gold = buffer.read_int32();
	return 0;
}

void Money_Info::reset(){
	bind_copper = 0;
	copper = 0;
	bind_gold = 0;
	gold = 0;
}

Mail_Detail::Mail_Detail(void){
	reset();
}

void Mail_Detail::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(pickup);
	buffer.write_int32(mail_id);
	buffer.write_int32(send_time);
	buffer.write_int32(sender_type);
	buffer.write_int64(sender_id);
	buffer.write_string(sender_name);
	buffer.write_string(mail_title);
	buffer.write_string(mail_content);
	money_info.serialize(buffer);
}

int Mail_Detail::deserialize(Block_Buffer &buffer) {
	pickup = buffer.read_int8();
	mail_id = buffer.read_int32();
	send_time = buffer.read_int32();
	sender_type = buffer.read_int32();
	sender_id = buffer.read_int64();
	sender_name = buffer.read_string();
	mail_title = buffer.read_string();
	mail_content = buffer.read_string();
	money_info.deserialize(buffer);
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
}
