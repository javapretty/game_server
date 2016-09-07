/*
 * Log_Struct.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Log_DB.h"
#include "Log_Struct.h"

Log_Struct::Log_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {}

Log_Struct::~Log_Struct() {}

void Log_Struct::save_data(Block_Buffer &buffer) {
	std::string str_name;
	std::string str_value;
	int64_t key_index = 0;
	buffer.peek_int64(key_index);
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		if(iter->field_label == "arg") {
			build_sql_arg(*iter, buffer, str_name, str_value);
		}
		else {
			LOG_ERROR("Can not find the field_label: %s", iter->field_label.c_str());
			return;
		}
	}

	str_name = str_name.substr(0, str_name.length()-1);
	str_value = str_value.substr(0, str_value.length()-1);
	char str_sql[512] = {0};
	sprintf(str_sql, "INSERT INTO %s (%s) VALUES (%s)", table_name().c_str(), str_name.c_str(), str_value.c_str());
	LOG_DB->connection()->execute(str_sql);

	LOG_INFO("table %s save key_index:%ld", table_name().c_str(), key_index);
}

void Log_Struct::build_sql_arg(const Field_Info &field_info, Block_Buffer &buffer, std::string &str_name, std::string &str_value) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	if(field_info.field_type == "int8") {
		int8_t value = 0;
		buffer.read_int8(value);
		stream << value << "," ;
	}
	else if(field_info.field_type == "int16") {
		int16_t value = 0;
		buffer.read_int16(value);
		stream << value << "," ;
	}
	else if(field_info.field_type == "int32") {
		int32_t value = 0;
		buffer.read_int32(value);
		stream << value << "," ;
	}
	else if(field_info.field_type == "int64") {
		int64_t value = 0;
		buffer.read_int64(value);
		stream << value << "," ;
	}
	else if(field_info.field_type == "double") {
		double value = 0;
		buffer.read_double(value);
		stream << value << "," ;
	}
	else if(field_info.field_type == "bool") {
		bool value = false;
		buffer.read_bool(value);
		stream << value << "," ;
	}
	else if(field_info.field_type == "string") {
		std::string value = "";
		buffer.read_string(value);
		stream << "\'" << value << "\'" << "," ;
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
	str_value += stream.str();
}
