/*
 * Log_Struct.cpp
 *
 *  Created on: 2016年7月28日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Log_DB.h"
#include "Log_Struct.h"

Log_Struct::Log_Struct(Xml &xml, TiXmlNode *node) : DB_Struct(xml, node){}

Log_Struct::~Log_Struct(){}

void Log_Struct::save_data(Block_Buffer &buffer){
	std::string str_name;
	std::string str_value;
	int64_t key_index = buffer.peek_int64();
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++){
		if((*iter).field_label == "arg"){
			build_sql_arg(*iter, buffer, str_name, str_value);
		}
		else {
			LOG_ERROR("Can not find the field_label: %s", (*iter).field_label.c_str());
			return;
		}
	}

	str_name = str_name.substr(0, str_name.length()-1);
	str_value = str_value.substr(0, str_value.length()-1);
	char str_sql[512] = {0};
	sprintf(str_sql, "INSERT INTO %s (%s) VALUES (%s)", table_name_.c_str(), str_name.c_str(), str_value.c_str());
	LOG_DB_CONNECTION->execute(str_sql);

	LOG_DEBUG("table %s save key_index:%ld", table_name_.c_str(), key_index);
}

void Log_Struct::build_sql_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &str_name, std::string &str_value){
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	if(field_info.field_type == "int8"){
		int8_t value = buffer.read_int8();
		stream << value << "," ;
	}
	else if(field_info.field_type == "int16"){
		int16_t value = buffer.read_int16();
		stream << value << "," ;
	}
	else if(field_info.field_type == "int32"){
		int32_t value = buffer.read_int32();
		stream << value << "," ;
	}
	else if(field_info.field_type == "int64"){
		int64_t value = buffer.read_int64();
		stream << value << "," ;
	}
	else if(field_info.field_type == "string"){
		std::string value = buffer.read_string();
		stream << "\'" << value << "\'" << "," ;
	}
	else if(field_info.field_type == "bool"){
		bool value = buffer.read_bool();
		stream << value << "," ;
	}
	else if(field_info.field_type == "double"){
		double value = buffer.read_double();
		stream << value << "," ;
	}
	else {
		LOG_ERROR("Can not find the type %s", field_info.field_type.c_str());
	}
	str_value += stream.str();
}
