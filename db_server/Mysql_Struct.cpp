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

void Mysql_Struct::create_data(int64_t key_index, Block_Buffer &buffer) {
	std::string str_name;
	std::string str_value;
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++) {
		if(iter->field_label == "arg") {
			create_data_arg(*iter, buffer, str_name, str_value, key_index);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			create_data_vector(*iter, buffer, str_name, str_value);
		}
		else if(iter->field_label == "struct") {
			create_data_struct(*iter, buffer, str_name, str_value);
		}
	}

	str_name = str_name.substr(0, str_name.length()-1);
	str_value = str_value.substr(0, str_value.length()-1);
	char str_sql[512] = {0};
	sprintf(str_sql, "INSERT INTO %s (%s) VALUES (%s)", table_name_.c_str(), str_name.c_str(), str_value.c_str());
	MYSQL_CONNECTION->execute(str_sql);
}

void Mysql_Struct::load_data(int64_t key_index, Block_Buffer &buffer) {
	char str_sql[64] = {0};
	if(key_index == 0) {
		//加载master公共数据，返回消息是数据列表，需要写入长度
		sprintf(str_sql, "select * from %s", table_name_.c_str());
	} else {
		//加载玩家数据，不需要写长度
		sprintf(str_sql, "select * from %s where %s=%ld", table_name_.c_str(), key_index_.c_str(), key_index);
	}
	sql::ResultSet *result = MYSQL_CONNECTION->execute_query(str_sql);
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
	int64_t key_index = buffer.peek_int64();
	LOG_INFO("table %s save key_index:%ld", table_name_.c_str(), key_index);
	if (key_index <= 0) {
		return;
	}

	std::string str_insert_name;
	std::string str_insert_value;
	std::string str_update;
	std::stringstream stream;
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++) {
		stream.str("");
		stream << iter->field_name << ",";
		str_insert_name += stream.str();

		stream.str("");
		stream << "?,";
		str_insert_value += stream.str();

		stream.str("");
		stream << iter->field_name << "=?,";
		str_update += stream.str();
	}
	str_insert_name = str_insert_name.substr(0, str_insert_name.length()-1);
	str_insert_value = str_insert_value.substr(0, str_insert_value.length()-1);
	str_update = str_update.substr(0, str_update.length()-1);

	//插入更新语句，如果记录不存在则插入，存在则更新
	char str_sql[512] = {0};
	sprintf(str_sql, "INSERT INTO %s(%s) VALUES(%s) ON DUPLICATE KEY UPDATE %s",
			table_name_.c_str(), str_insert_name.c_str(), str_insert_value.c_str(), str_update.c_str());
	sql::PreparedStatement* pstmt = MYSQL_CONNECTION->create_pstmt(str_sql);
	if (!pstmt) {
		LOG_ERROR("create_pstmt error, sql:%s", str_sql);
		return;
	}

	int param_index1 = 0;
	int param_index2 = 0;
	int param_size = field_vec_.size();
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++) {
		param_index1++;
		param_index2 = param_index1 + param_size;

		if(iter->field_label == "arg") {
			if(iter->field_type == "int8") {
				int8_t value = buffer.read_int8();
				pstmt->setInt(param_index1, value);
				pstmt->setInt(param_index2, value);
			}
			else if(iter->field_type == "int16") {
				int16_t value = buffer.read_int16();
				pstmt->setInt(param_index1, value);
				pstmt->setInt(param_index2, value);
			}
			else if(iter->field_type == "int32") {
				int32_t value = buffer.read_int32();
				pstmt->setInt(param_index1, value);
				pstmt->setInt(param_index2, value);
			}
			else if(iter->field_type == "int64") {
				int64_t value = buffer.read_int64();
				pstmt->setInt64(param_index1, value);
				pstmt->setInt64(param_index2, value);
			}
			else if(iter->field_type == "double") {
				double value = buffer.read_double();
				pstmt->setDouble(param_index1, value);
				pstmt->setDouble(param_index2, value);
			}
			else if(iter->field_type == "bool") {
				bool value = buffer.read_bool();
				pstmt->setBoolean(param_index1, value);
				pstmt->setBoolean(param_index2, value);
			}
			else if(iter->field_type == "string") {
				std::string value = buffer.read_string();
				pstmt->setString(param_index1, value);
				pstmt->setString(param_index2, value);
			}
			else {
				LOG_ERROR("Can not find the field_type:%s, struct_name:%s", iter->field_type.c_str(), struct_name().c_str());
			}
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			//int read_idx = buffer.get_read_idx();
			int field_len = build_field_len_vector(*iter, buffer);
			LOG_INFO("struct_name:%s, filed_type:%s, field_name:%s, field_len:%d",
								struct_name().c_str(), iter->field_type.c_str(), iter->field_name.c_str(), field_len);
			//buffer.set_read_idx(read_idx);

			char buf[64] = "I am a good boy!";
			DataBuf data_buf(buf, strlen(buf));
			std::istream s(&data_buf);
			pstmt->setBlob(param_index1, &s);
			pstmt->setBlob(param_index2, &s);
		}
		else if(iter->field_label == "struct") {
			//int read_idx = buffer.get_read_idx();
			int field_len = build_field_len_struct(*iter, buffer);
			LOG_INFO("struct_name:%s, filed_type:%s, field_name:%s, field_len:%d",
								struct_name().c_str(), iter->field_type.c_str(), iter->field_name.c_str(), field_len);
			//buffer.set_read_idx(read_idx);

			char buf[64] = "";
			DataBuf data_buf(buf, strlen(buf));
			std::istream s(&data_buf);
			pstmt->setBlob(param_index1, &s);
			pstmt->setBlob(param_index2 + param_size, &s);
		}
	}
	pstmt->execute();
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

void Mysql_Struct::create_data_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &str_name, std::string &str_value, int64_t key_index) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	if(field_info.field_type == "int8") {
		stream << 0 << ",";
		buffer.write_int8(0);
	}
	else if(field_info.field_type == "int16") {
		stream << 0 << ",";
		buffer.write_int16(0);
	}
	else if(field_info.field_type == "int32") {
		stream << 0 << ",";
		buffer.write_int32(0);
	}
	else if(field_info.field_type == "int64") {
		int64_t value = 0;
		if(field_info.field_name == key_index_) {
			value = key_index;
		}
		stream << value << ",";
		buffer.write_int64(value);
	}
	else if(field_info.field_type == "double") {
		stream << 0 << ",";
		buffer.write_double(0);
	}
	else if(field_info.field_type == "bool") {
		stream << false << ",";
		buffer.write_bool(false);
	}
	else if(field_info.field_type == "string") {
		stream << "\'\'" << ",";
		buffer.write_string("");
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
	str_value += stream.str();
}

void Mysql_Struct::create_data_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &str_name, std::string &str_value) {
	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	stream << "\'\'" << "," ;
	str_value += stream.str();
	buffer.write_uint16(0);
}

void Mysql_Struct::create_data_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &str_name, std::string &str_value) {
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	std::stringstream stream;
	stream.str("");
	stream << field_info.field_name << "," ;
	str_name += stream.str();

	stream.str("");
	stream << "\'\'" << "," ;
	str_value += stream.str();

	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg") {
			create_data_arg(field_info, buffer, str_name, str_value, 0);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			create_data_vector(field_info, buffer, str_name, str_value);
		}
		else if(iter->field_label == "struct") {
			create_data_struct(field_info, buffer, str_name, str_value);
		}
	}
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
	std::string blob_str = result->getString(field_info.field_name);
	buffer.copy(blob_str);
	if (blob_str == "") {
		buffer.write_uint16(0);
	}
}

void Mysql_Struct::build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result) {
	std::string blob_str = result->getString(field_info.field_name);
	buffer.copy(blob_str);
}

int Mysql_Struct::build_field_len_arg(Field_Info &field_info, Block_Buffer &buffer) {
	int field_len = 0;
	if(field_info.field_type == "int8") {
		int8_t value = buffer.read_int8();
		field_len = sizeof(value);
	}
	else if(field_info.field_type == "int16") {
		int16_t value = buffer.read_int16();
		field_len = sizeof(value);
	}
	else if(field_info.field_type == "int32") {
		int32_t value = buffer.read_int32();
		field_len = sizeof(value);
	}
	else if(field_info.field_type == "int64") {
		int64_t value = buffer.read_int64();
		field_len = sizeof(value);
	}
	else if(field_info.field_type == "double") {
		double value = buffer.read_double();
		field_len = sizeof(value);
	}
	else if(field_info.field_type == "bool") {
		bool value = buffer.read_int8();
		field_len = sizeof(value);
	}
	else if(field_info.field_type == "string") {
		std::string value = buffer.read_string();
		//string类型变量要先写uint16_t类型的长度
		field_len = sizeof(value) + sizeof(uint16_t);
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
	return field_len;
}

int Mysql_Struct::build_field_len_vector(Field_Info &field_info, Block_Buffer &buffer) {
	int field_len = 2;
	uint16_t vec_size = buffer.read_uint16();
	if(is_struct(field_info.field_type)) {
		for(uint16_t i = 0; i < vec_size; ++i) {
			field_len += build_field_len_struct(field_info, buffer);
		}
	}
	else{
		for(uint16_t i = 0; i < vec_size; ++i) {
			field_len += build_field_len_arg(field_info, buffer);
		}
	}
	return field_len;
}

int Mysql_Struct::build_field_len_struct(Field_Info &field_info, Block_Buffer &buffer) {
	int field_len = 0;
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return field_len;
	}

	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg") {
			field_len += build_field_len_arg(*iter, buffer);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			field_len += build_field_len_vector(*iter, buffer);
		}
		else if(iter->field_label == "struct") {
			field_len += build_field_len_struct(*iter, buffer);
		}
	}
	return field_len;
}
