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
	char str_name[2048] = {0};
	char str_value[2048] = {0};
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++){
		if((*iter).field_label == "arg"){
			create_data_arg(*iter, str_name, str_value, index);
		}
		else if((*iter).field_label == "vector"){
			create_data_vector(*iter, str_name, str_value);
		}
		else if((*iter).field_label == "struct"){
			create_data_struct(*iter, str_name, str_value);
		}
	}

	char str_sql[2048] = {0};
	sprintf(str_sql, "INSERT INTO %s (%s) VALUES (%s)", db_name_.c_str(), str_name, str_value);
	LOG_DEBUG("execute SQL=[%s]", str_sql);
	MYSQL_CONNECTION->Execute(str_sql);
}

void Mysql_Struct::load_data(int64_t index, Block_Buffer &buffer){

}

void Mysql_Struct::save_data(Block_Buffer &buffer){
	std::string sql_str;
	int64_t index_value = buffer.peek_int64();
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++){
		if((*iter).field_label == "arg"){
			build_bson_arg(*iter, buffer, sql_str);
		}
		else if((*iter).field_label == "vector"){
			build_bson_vector(*iter, buffer, sql_str);
		}
		else if((*iter).field_label == "struct"){
			build_bson_struct(*iter, buffer, sql_str);
		}
	}
	MYSQL_CONNECTION->Execute(sql_str.c_str());

	LOG_DEBUG("table %s save index:%ld", db_name_.c_str(), index_value);
}

void Mysql_Struct::save_data_vector(Block_Buffer &buffer){
	uint16_t count = buffer.read_uint16();
	LOG_DEBUG("table %s save size:%d", db_name_.c_str(), count);
	for(int i = 0; i < count; i++){
		save_data(buffer);
	}
}

void Mysql_Struct::delete_data(Block_Buffer &buffer) {
	char sql_str[256] = {0};
	uint16_t count = buffer.read_uint16();
	for(int i = 0; i < count; i++){
		int64_t index = buffer.read_int64();
		sprintf(sql_str, "delete from %s where %s=%ld", db_name_.c_str(), index_.c_str(), index);
		MYSQL_CONNECTION->Execute(sql_str);
	}
}

void Mysql_Struct::create_data_arg(Field_Info &field_info, char *str_name, char *str_value, int64_t index){
	sprintf(str_name, "%s %s,", str_name, field_info.field_name.c_str());

	if (field_info.field_type == "int8" || field_info.field_type == "int16" || field_info.field_type == "int32"
			|| field_info.field_type == "double" || field_info.field_type == "bool" ) {
		sprintf(str_value, "%s 0, ", str_value);
	} else if (field_info.field_type == "int64") {
		int64_t value = 0;
		if(field_info.field_name == index_) {
			value = index;
		}
		sprintf(str_value, "%s %ld, ", str_value, value);
	} else if (field_info.field_type == "string") {
		sprintf(str_value, "%s %s, ", str_value, "");
	} else {
		LOG_ERROR("Can not find the type %s", field_info.field_type.c_str());
	}
}

void Mysql_Struct::create_data_vector(Field_Info &field_info, char *str_name, char *str_value){
	sprintf(str_name, "%s %s,", str_name, field_info.field_name.c_str());
	sprintf(str_value, "%s %s,", str_value, "");
}

void Mysql_Struct::create_data_struct(Field_Info &field_info, char *str_name, char *str_value){
	DB_Manager::DB_Struct_Name_Map::iterator iter = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(iter == DB_MANAGER->db_struct_name_map().end()){
		LOG_ERROR("Can not find the module %s", field_info.field_type.c_str());
		return;
	}

	DB_Struct *db_struct  = iter->second;
	std::vector<Field_Info> field_vec = db_struct->field_vec();
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++){
		if((*iter).field_label == "arg"){
			create_data_arg(*iter, str_name, str_value, 0);
		}
		else if((*iter).field_label == "vector"){
			create_data_vector(*iter, str_name, str_value);
		}
		else if((*iter).field_label == "struct"){
			create_data_struct(*iter, str_name, str_value);
		}
	}
}

void Mysql_Struct::build_bson_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str){

}

void Mysql_Struct::build_bson_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str){

}

void Mysql_Struct::build_bson_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str){

}

void Mysql_Struct::build_buffer_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str) {

}

void Mysql_Struct::build_buffer_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str) {

}

void Mysql_Struct::build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str) {

}
