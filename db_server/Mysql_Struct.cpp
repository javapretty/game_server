/*
 * Mysql_Struct.cpp
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#include "Mysql_Operator.h"
#include "Mysql_Struct.h"
#include "DB_Manager.h"

Mysql_Struct::Mysql_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {}

Mysql_Struct::~Mysql_Struct() {}

void Mysql_Struct::create_data(int64_t key_index) {
	std::string str_name;
	std::string str_value;
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++) {
		if(iter->field_label == "arg") {
			create_data_arg(*iter, str_name, str_value, key_index);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			create_data_vector(*iter, str_name, str_value);
		}
		else if(iter->field_label == "struct") {
			create_data_struct(*iter, str_name, str_value);
		}
	}

	str_name = str_name.substr(0, str_name.length()-1);
	str_value = str_value.substr(0, str_value.length()-1);
	char sql[512] = {0};
	sprintf(sql, "INSERT INTO %s (%s) VALUES (%s)", table_name_.c_str(), str_name.c_str(), str_value.c_str());
	MYSQL_CONNECTION->execute(sql);
}

void Mysql_Struct::load_data(int64_t key_index, Block_Buffer &buffer) {
	char sql[256] = {0};
	if(key_index == 0) {
		//加载master公共数据，返回消息是数据列表，需要写入长度
		sprintf(sql, "select * from %s", table_name_.c_str());
	} else {
		//加载玩家数据，不需要写长度
		sprintf(sql, "select * from %s where %s=%ld", table_name_.c_str(), key_index_.c_str(), key_index);
	}
	sql::ResultSet *result = MYSQL_CONNECTION->execute_query(sql);
	if (result) {
		if (key_index == 0) {
			buffer.write_uint16(result->rowsCount());
		}

		while (result->next()) {
			for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
					iter != field_vec_.end(); iter++) {
				if(iter->field_label == "arg") {
					build_buffer_arg(*iter, buffer, result);
				}
				else if(iter->field_label == "vector" || iter->field_label == "map") {
					build_buffer_vector(*iter, buffer, result);
				}
				else if(iter->field_label == "struct") {
					build_buffer_struct(*iter, buffer, result);
				}
			}
		}
	}
}

void Mysql_Struct::save_data(Block_Buffer &buffer) {
	std::string str_sql;
	int64_t key_index = buffer.peek_int64();
	LOG_INFO("table %s save key_index:%ld", table_name_.c_str(), key_index);
	if (key_index <= 0) {
		return;
	}

	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++) {
		if(iter->field_label == "arg") {
			build_sql_arg(*iter, buffer, str_sql);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			build_sql_vector(*iter, buffer, str_sql);
		}
		else if(iter->field_label == "struct") {
			build_sql_struct(*iter, buffer, str_sql);
		}
	}

	str_sql = str_sql.substr(0, str_sql.length()-1);
	char sql[512] = {0};
	sprintf(sql, "UPDATE %s SET %s where %s=%ld", table_name_.c_str(), str_sql.c_str(), key_index_.c_str(), key_index);
	MYSQL_CONNECTION->execute(sql);
}

void Mysql_Struct::save_data_vector(Block_Buffer &buffer) {
	uint16_t count = buffer.read_uint16();
	LOG_INFO("table %s save size:%d", table_name_.c_str(), count);
	for(int i = 0; i < count; i++) {
		save_data(buffer);
	}
}

void Mysql_Struct::delete_data(Block_Buffer &buffer) {
	char sql_str[256] = {0};
	uint16_t count = buffer.read_uint16();
	for(int i = 0; i < count; i++){
		int64_t index = buffer.read_int64();
		sprintf(sql_str, "delete from %s where %s=%ld", table_name_.c_str(), key_index_.c_str(), index);
		MYSQL_CONNECTION->execute(sql_str);
	}
}

void Mysql_Struct::create_data_arg(Field_Info &field_info, std::string &str_name, std::string &str_value, int64_t key_index) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	if(field_info.field_type == "int8") {
		stream << 0 << ",";
	}
	else if(field_info.field_type == "int16") {
		stream << 0 << ",";
	}
	else if(field_info.field_type == "int32") {
		stream << 0 << ",";
	}
	else if(field_info.field_type == "int64") {
		int64_t value = 0;
		if(field_info.field_name == key_index_) {
			value = key_index;
		}
		stream << value << ",";
	}
	else if(field_info.field_type == "double") {
		stream << 0 << ",";
	}
	else if(field_info.field_type == "bool") {
		stream << 0 << ",";
	}
	else if(field_info.field_type == "string") {
		stream << "\'\'" << ",";
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
	str_value += stream.str();
}

void Mysql_Struct::create_data_vector(Field_Info &field_info, std::string &str_name, std::string &str_value) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	stream << "\'\'" << "," ;
	str_value += stream.str();
}

void Mysql_Struct::create_data_struct(Field_Info &field_info, std::string &str_name, std::string &str_value) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	stream << "\'\'" << "," ;
	str_value += stream.str();
}

void Mysql_Struct::build_buffer_arg(Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result) {
	if(field_info.field_type == "int8") {
		int8_t value = result->getInt(field_info.field_name);
		buffer.write_int8(value);
	}
	else if(field_info.field_type == "int16") {
		int16_t value = result->getInt(field_info.field_name);
		buffer.write_int16(value);
	}
	else if(field_info.field_type == "int32") {
		int32_t value = result->getInt(field_info.field_name);
		buffer.write_int32(value);
	}
	else if(field_info.field_type == "int64") {
		int64_t value = result->getInt64(field_info.field_name);
		buffer.write_int64(value);
	}
	else if(field_info.field_type == "double") {
		double value = result->getDouble(field_info.field_name);
		buffer.write_double(value);
	}
	else if(field_info.field_type == "bool") {
		bool value = result->getBoolean(field_info.field_name);
		buffer.write_bool(value);
	}
	else if(field_info.field_type == "string") {
		std::string value = result->getString(field_info.field_name);
		buffer.write_string(value);
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
}

void Mysql_Struct::build_buffer_vector(Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result) {
	buffer.write_uint16(0);
}

void Mysql_Struct::build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result) {
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg") {
			build_buffer_arg(*iter, buffer, result);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			build_buffer_vector(*iter, buffer, result);
		}
		else if(iter->field_label == "struct") {
			build_buffer_struct(*iter, buffer, result);
		}
	}
}

void Mysql_Struct::build_sql_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &str_sql) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "=";

	if(field_info.field_type == "int8") {
		int32_t value = buffer.read_int8();
		stream << value << "," ;
	}
	else if(field_info.field_type == "int16") {
		int32_t value = buffer.read_int16();
		stream << value << "," ;
	}
	else if(field_info.field_type == "int32") {
		int32_t value = buffer.read_int32();
		stream << value << "," ;
	}
	else if(field_info.field_type == "int64") {
		int64_t value = buffer.read_int64();
		stream << value << "," ;
	}
	else if(field_info.field_type == "double") {
		double value = buffer.read_double();
		stream << value << "," ;
	}
	else if(field_info.field_type == "bool") {
		bool value = buffer.read_bool();
		stream << value << "," ;
	}
	else if(field_info.field_type == "string") {
		std::string value = buffer.read_string();
		stream << "\'" << value << "\'" << "," ;
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
	str_sql += stream.str();
}

void Mysql_Struct::build_sql_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &str_sql) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "=\'\'," ;
	str_sql += stream.str();
}

void Mysql_Struct::build_sql_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &str_sql) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "=\'\'," ;
	str_sql += stream.str();
}
