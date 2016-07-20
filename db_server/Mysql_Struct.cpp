/*
 * Mysql_Struct.cpp
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#include "Mysql_Operator.h"
#include "Mysql_Struct.h"
#include "DB_Manager.h"

Mysql_Struct::Mysql_Struct(Xml &xml, TiXmlNode *node) : DB_Struct(xml, node){}

Mysql_Struct::~Mysql_Struct(){}

void Mysql_Struct::create_data(int64_t index){
}

void Mysql_Struct::load_data(int64_t index, Block_Buffer &buffer){
}

void Mysql_Struct::save_data(Block_Buffer &buffer){
}

void Mysql_Struct::save_data_vector(Block_Buffer &buffer){
}

void Mysql_Struct::delete_data(Block_Buffer &buffer) {
}

void Mysql_Struct::create_data_arg(Field_Info &field_info, std::string &sql, int64_t role_id) {

}

void Mysql_Struct::create_data_vector(Field_Info &field_info, std::string &sql) {

}

void Mysql_Struct::create_data_struct(Field_Info &field_info, std::string &sql) {

}

void Mysql_Struct::build_bson_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &sql) {

}

void Mysql_Struct::build_bson_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &sql) {

}

void Mysql_Struct::build_bson_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &sql) {

}

void Mysql_Struct::build_buffer_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &sql) {

}

void Mysql_Struct::build_buffer_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &sql) {

}

void Mysql_Struct::build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &sql) {

}
