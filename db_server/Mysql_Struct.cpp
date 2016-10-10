/*
 * Mysql_Struct.cpp
 *
 *  Created on: Jul 20, 2016
 *      Author: zhangyalei
 */

#include "Mysql_Operator.h"
#include "Mysql_Struct.h"
#include "DB_Manager.h"
#include "Common_Func.h"

Mysql_Struct::Mysql_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {}

Mysql_Struct::~Mysql_Struct() {}

void Mysql_Struct::create_data(int64_t key_index, Block_Buffer &buffer) {
	char str_sql[512] = {0};
	std::string str_name;
	std::string str_value;
	std::stringstream stream;
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		stream.str("");
		stream << iter->field_name << ",";
		str_name += stream.str();

		stream.str("");
		stream << "?,";
		str_value += stream.str();
	}
	str_name = str_name.substr(0, str_name.length()-1);
	str_value = str_value.substr(0, str_value.length()-1);
	sprintf(str_sql, "INSERT INTO %s (%s) VALUES (%s)", table_name().c_str(), str_name.c_str(), str_value.c_str());
	sql::PreparedStatement* pstmt = MYSQL_CONNECTION->create_pstmt(str_sql);
	if (!pstmt) {
		LOG_ERROR("create_pstmt error, sql:%s", str_sql);
		return;
	}

	int param_index = 0;
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		param_index++;

		if(iter->field_label == "arg") {
			if(iter->field_type == "int8") {
				buffer.write_int8(0);
				pstmt->setInt(param_index, 0);
			}
			else if(iter->field_type == "int16") {
				buffer.write_int16(0);
				pstmt->setInt(param_index, 0);
			}
			else if(iter->field_type == "int32") {
				buffer.write_int32(0);
				pstmt->setInt(param_index, 0);
			}
			else if(iter->field_type == "int64") {
				int64_t value = 0;
				if(iter->field_name == index_name()) {
					value = key_index;
				}
				buffer.write_int64(value);
				pstmt->setInt64(param_index, value);
			}
			else if(iter->field_type == "double") {
				buffer.write_double(0);
				pstmt->setDouble(param_index, 0);
			}
			else if(iter->field_type == "bool") {
				buffer.write_bool(false);
				pstmt->setBoolean(param_index, false);
			}
			else if(iter->field_type == "string") {
				buffer.write_string("");
				pstmt->setString(param_index, "");
			}
			else {
				LOG_ERROR("Can not find the field_type:%s, struct_name:%s", iter->field_type.c_str(), struct_name().c_str());
			}
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			//blob_buffer只提供对该vector类型的初始化数据
			Block_Buffer blob_buffer;
			int field_len = create_data_vector(*iter, buffer, blob_buffer);

			std::string blob_data = base64_encode((unsigned char *)blob_buffer.get_read_ptr(), field_len);
			pstmt->setString(param_index, blob_data);
		}
		else if(iter->field_label == "struct") {
			//blob_buffer只提供对该struct类型的初始化数据
			Block_Buffer blob_buffer;
			int field_len = create_data_struct(*iter, buffer, blob_buffer);

			std::string blob_data = base64_encode((unsigned char *)blob_buffer.get_read_ptr(), field_len);
			pstmt->setString(param_index, blob_data);

			LOG_INFO("struct_name:%s, field_type:%s, field_name:%s, param_index:%d, field_len:%d", struct_name().c_str(),
					iter->field_type.c_str(), iter->field_name.c_str(), param_index, field_len);
		}
	}
	pstmt->execute();
	delete pstmt;
}

void Mysql_Struct::load_data(int64_t key_index, Block_Buffer &buffer) {
	char str_sql[128] = {0};
	if(key_index == 0) {
		//加载master公共数据，返回消息是数据列表，需要写入长度
		sprintf(str_sql, "select * from %s", table_name().c_str());
	} else {
		//加载玩家数据，不需要写长度
		sprintf(str_sql, "select * from %s where %s=%ld", table_name().c_str(), index_name().c_str(), key_index);
	}
	sql::ResultSet *result = MYSQL_CONNECTION->execute_query(str_sql);
	if (result) {
		if (key_index == 0) {
			buffer.write_uint16(result->rowsCount());
		}

		while (result->next()) {
			for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
					iter != field_vec().end(); iter++) {
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
	int64_t key_index = 0;
	buffer.peek_int64(key_index);
	LOG_INFO("table %s save key_index:%ld", table_name().c_str(), key_index);
	if (key_index <= 0) {
		return;
	}

	char update_sql[512] = {};
	std::string update_value;
	std::stringstream update_stream;
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		update_stream.str("");
		update_stream << iter->field_name << "=?,";
		update_value += update_stream.str();
	}
	update_value = update_value.substr(0, update_value.length()-1);
	sprintf(update_sql, "UPDATE Set %s ", update_value.c_str());

	char insert_sql[1048] = {};
	std::string insert_name;
	std::string insert_value;
	std::stringstream insert_stream;
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		insert_stream.str("");
		insert_stream << iter->field_name << ",";
		insert_name += insert_stream.str();

		insert_stream.str("");
		insert_stream << "?,";
		insert_value += insert_stream.str();
	}
	insert_name = insert_name.substr(0, insert_name.length()-1);
	insert_value = insert_value.substr(0, insert_value.length()-1);
	sprintf(insert_sql, "INSERT INTO %s (%s) VALUES (%s)",
			table_name().c_str(), insert_name.c_str(), insert_value.c_str());
	sprintf(insert_sql + strlen(insert_sql), " ON DUPLICATE KEY %s", update_sql);
	sql::PreparedStatement* pstmt = MYSQL_CONNECTION->create_pstmt(insert_sql);

	int param_index = 0;
	int param_len = field_vec().size();
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		param_index++;

		if(iter->field_label == "arg") {
			if(iter->field_type == "int8") {
				int8_t value = 0;
				buffer.read_int8(value);
				pstmt->setInt(param_index, value);
				pstmt->setInt(param_index + param_len, value);
			}
			else if(iter->field_type == "int16") {
				int16_t value = 0;
				buffer.read_int16(value);
				pstmt->setInt(param_index, value);
				pstmt->setInt(param_index + param_len, value);
			}
			else if(iter->field_type == "int32") {
				int32_t value = 0;
				buffer.read_int32(value);
				pstmt->setInt(param_index, value);
				pstmt->setInt(param_index + param_len, value);
			}
			else if(iter->field_type == "int64") {
				int64_t value = 0;
				buffer.read_int64(value);
				pstmt->setInt64(param_index, value);
				pstmt->setInt64(param_index + param_len, value);
			}
			else if(iter->field_type == "double") {
				double value = 0;
				buffer.read_double(value);
				pstmt->setDouble(param_index, value);
				pstmt->setDouble(param_index + param_len, value);
			}
			else if(iter->field_type == "bool") {
				bool value = false;
				buffer.read_bool(value);
				pstmt->setBoolean(param_index, value);
				pstmt->setBoolean(param_index + param_len, value);
			}
			else if(iter->field_type == "string") {
				std::string value = "";
				buffer.read_string(value);
				pstmt->setString(param_index, value);
				pstmt->setString(param_index + param_len, value);
			}
			else {
				LOG_ERROR("Can not find the field_type:%s, struct_name:%s", iter->field_type.c_str(), struct_name().c_str());
			}
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			int read_idx = buffer.get_read_idx();
			int field_len = build_len_vector(*iter, buffer);
			buffer.set_read_idx(read_idx);

			std::string blob_data = base64_encode((unsigned char *)buffer.get_read_ptr(), field_len);
			buffer.set_read_idx(read_idx + field_len);
			pstmt->setString(param_index, blob_data);
			pstmt->setString(param_index + param_len, blob_data);

			LOG_INFO("struct_name:%s, fileld_label:%s, field_type:%s, field_name:%s, param_index:%d, field_len:%d, read_idx:%d", struct_name().c_str(),
					iter->field_label.c_str(), iter->field_type.c_str(), iter->field_name.c_str(), param_index, field_len, buffer.get_read_idx());
		}
		else if(iter->field_label == "struct") {
			int read_idx = buffer.get_read_idx();
			int field_len = build_len_struct(*iter, buffer);
			buffer.set_read_idx(read_idx);

			std::string blob_data = base64_encode((unsigned char *)buffer.get_read_ptr(), field_len);
			buffer.set_read_idx(read_idx + field_len);
			pstmt->setString(param_index, blob_data);
			pstmt->setString(param_index + param_len, blob_data);

			LOG_INFO("struct_name:%s, fileld_label:%s, field_type:%s, field_name:%s, param_index:%d, field_len:%d, read_idx:%d", struct_name().c_str(),
				iter->field_label.c_str(), iter->field_type.c_str(), iter->field_name.c_str(), param_index, field_len, buffer.get_read_idx());
		}
	}
	pstmt->execute();
	delete pstmt;
}

void Mysql_Struct::save_data_vector(Block_Buffer &buffer) {
	uint16_t count = 0;
	buffer.read_uint16(count);
	LOG_INFO("table %s save size:%d", table_name().c_str(), count);
	for(int i = 0; i < count; i++) {
		save_data(buffer);
	}
}

void Mysql_Struct::delete_data(Block_Buffer &buffer) {
	char sql_str[128] = {0};
	uint16_t count = 0;
	buffer.read_uint16(count);
	for(int i = 0; i < count; i++){
		int64_t key_index = 0;
		buffer.read_int64(key_index);
		sprintf(sql_str, "delete from %s where %s=%ld", table_name().c_str(), index_name().c_str(), key_index);
		MYSQL_CONNECTION->execute(sql_str);
	}
}

int Mysql_Struct::create_data_arg(const Field_Info &field_info, Block_Buffer &buffer, Block_Buffer &blob_buffer) {
	int field_len = 0;
	if(field_info.field_type == "int8") {
		buffer.write_int8(0);
		blob_buffer.write_int8(0);
		field_len = sizeof(int8_t);
	}
	else if(field_info.field_type == "int16") {
		buffer.write_int16(0);
		blob_buffer.write_int16(0);
		field_len = sizeof(int16_t);
	}
	else if(field_info.field_type == "int32") {
		buffer.write_int32(0);
		blob_buffer.write_int32(0);
		field_len = sizeof(int32_t);
	}
	else if(field_info.field_type == "int64") {
		buffer.write_int64(0);
		blob_buffer.write_int64(0);
		field_len = sizeof(int64_t);
	}
	else if(field_info.field_type == "double") {
		buffer.write_double(0);
		blob_buffer.write_double(0);
		field_len = sizeof(double);
	}
	else if(field_info.field_type == "bool") {
		buffer.write_bool(false);
		blob_buffer.write_bool(false);
		field_len = sizeof(bool);
	}
	else if(field_info.field_type == "string") {
		buffer.write_string("");
		blob_buffer.write_string("");
		field_len = sizeof(uint16_t);
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
	return field_len;
}

int Mysql_Struct::create_data_vector(const Field_Info &field_info, Block_Buffer &blob_buffer, Block_Buffer &buffer) {
	buffer.write_uint16(0);
	blob_buffer.write_uint16(0);
	return sizeof(uint16_t);
}

int Mysql_Struct::create_data_struct(const Field_Info &field_info, Block_Buffer &blob_buffer, Block_Buffer &buffer) {
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return 0;
	}

	int field_len = 0;
	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg") {
			field_len += create_data_arg(field_info, buffer, blob_buffer);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			field_len += create_data_vector(field_info, blob_buffer, buffer);
		}
		else if(iter->field_label == "struct") {
			field_len += create_data_struct(field_info, blob_buffer, buffer);
		}
	}
	return field_len;
}

void Mysql_Struct::build_buffer_arg(const Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result) {
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

void Mysql_Struct::build_buffer_vector(const Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result) {
	std::string blob_str = result->getString(field_info.field_name);
	std::string decode = base64_decode(blob_str);
	buffer.copy(decode);
}

void Mysql_Struct::build_buffer_struct(const Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result) {
	std::string blob_str = result->getString(field_info.field_name);
	std::string decode = base64_decode(blob_str);
	buffer.copy(decode);
}

int Mysql_Struct::build_len_arg(const Field_Info &field_info, Block_Buffer &buffer) {
	int field_len = 0;
	if(field_info.field_type == "int8") {
		field_len = sizeof(int8_t);
	}
	else if(field_info.field_type == "int16") {
		field_len = sizeof(int16_t);
	}
	else if(field_info.field_type == "int32") {
		field_len = sizeof(int32_t);
	}
	else if(field_info.field_type == "int64") {
		field_len = sizeof(int64_t);
	}
	else if(field_info.field_type == "double") {
		field_len = sizeof(double);
	}
	else if(field_info.field_type == "bool") {
		field_len = sizeof(bool);
	}
	else if(field_info.field_type == "string") {
		//std::string value = buffer.read_string();
		//注意：string类型长度要用length计算,还要加上uint16_t类型的长度变量,不用read_string,减少memcpy调用次数
		uint16_t str_len = 0;
		buffer.peek_uint16(str_len);
		field_len = sizeof(uint16_t) + str_len;
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
	//设置buffer读指针，为了下次正确读取数据
	int read_idx = buffer.get_read_idx();
	buffer.set_read_idx(read_idx + field_len);
	return field_len;
}

int Mysql_Struct::build_len_vector(const Field_Info &field_info, Block_Buffer &buffer) {
	int field_len = sizeof(uint16_t);
	uint16_t vec_size = 0;
	buffer.read_uint16(vec_size);
	if(is_struct(field_info.field_type)) {
		for(uint16_t i = 0; i < vec_size; ++i) {
			field_len += build_len_struct(field_info, buffer);
		}
	}
	else{
		for(uint16_t i = 0; i < vec_size; ++i) {
			field_len += build_len_arg(field_info, buffer);
		}
	}
	return field_len;
}

int Mysql_Struct::build_len_struct(const Field_Info &field_info, Block_Buffer &buffer) {
	int field_len = 0;
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return field_len;
	}

	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg") {
			field_len += build_len_arg(*iter, buffer);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			field_len += build_len_vector(*iter, buffer);
		}
		else if(iter->field_label == "struct") {
			field_len += build_len_struct(*iter, buffer);
		}
	}
	return field_len;
}
