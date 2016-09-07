/*
 * Mongo_Struct.cpp
 *
 *  Created on: Jul 20, 2016
 *      Author: zhangyalei
 */

#include "Mongo_Operator.h"
#include "Mongo_Struct.h"
#include "DB_Manager.h"

Mongo_Struct::Mongo_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {
	std::string tb_name = "game." + table_name();
	set_table_name(tb_name);
}

Mongo_Struct::~Mongo_Struct() {}

void Mongo_Struct::create_data(int64_t key_index, Block_Buffer &buffer) {
	BSONObjBuilder builder ;
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		if(iter->field_label == "arg") {
			create_data_arg(*iter, buffer, builder, key_index);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			create_data_vector(*iter, buffer, builder);
		}
		else if(iter->field_label == "struct") {
			create_data_struct(*iter, buffer, builder);
		}
	}
	MONGO_CONNECTION.update(table_name(), MONGO_QUERY(index_name() << (long long int)key_index),
				BSON("$set" << builder.obj() ), true);
}

void Mongo_Struct::load_data(int64_t key_index, Block_Buffer &buffer) {
	std::vector<BSONObj> total_record;
	if(key_index == 0) {
		//加载master公共数据，返回消息是数据列表，需要写入长度
		int total_count = MONGO_CONNECTION.count(table_name());
		if(total_count > 0) {
			MONGO_CONNECTION.findN(total_record, table_name(), Query(), total_count);
		}
		buffer.write_uint16(total_record.size());
	} else {
		//加载玩家数据，不需要写长度
		BSONObj obj = MONGO_CONNECTION.findOne(table_name(), MONGO_QUERY(index_name() << (long long int)key_index));
		total_record.push_back(obj);
	}

	for (std::vector<BSONObj>::iterator iter = total_record.begin();
			iter != total_record.end(); ++iter) {
		for(std::vector<Field_Info>::const_iterator it = field_vec().begin();
				it != field_vec().end(); it++) {
			if(it->field_label == "arg") {
				build_buffer_arg(*it, buffer, *iter);
			}
			else if(it->field_label == "vector" || it->field_label == "map") {
				build_buffer_vector(*it, buffer, *iter);
			}
			else if(it->field_label == "struct") {
				build_buffer_struct(*it, buffer, *iter);
			}
		}
	}
}

void Mongo_Struct::save_data(Block_Buffer &buffer) {
	BSONObjBuilder set_builder;
	int64_t key_index = 0;
	buffer.peek_int64(key_index);
	LOG_INFO("table %s save key_index:%ld", table_name().c_str(), key_index);
	if (key_index <= 0) {
		return;
	}

	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		if(iter->field_label == "arg") {
			build_bson_arg(*iter, buffer, set_builder);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			build_bson_vector(*iter, buffer, set_builder);
		}
		else if(iter->field_label == "struct") {
			build_bson_struct(*iter, buffer, set_builder);
		}
	}
	MONGO_CONNECTION.update(table_name(), MONGO_QUERY(index_name() << (long long int)key_index),
			BSON("$set" << set_builder.obj() ), true);
}

void Mongo_Struct::save_data_vector(Block_Buffer &buffer) {
	uint16_t count = 0;
	buffer.read_uint16(count);
	LOG_INFO("table %s save size:%d", table_name().c_str(), count);
	for(int i = 0; i < count; i++) {
		save_data(buffer);
	}
}

void Mongo_Struct::delete_data(Block_Buffer &buffer) {
	uint16_t count = 0;
	buffer.read_uint16(count);
	for(int i = 0; i < count; i++) {
		int64_t key_index = 0;
		buffer.read_int64(key_index);
		MONGO_CONNECTION.remove(table_name(), MONGO_QUERY(index_name() << (long long int)(key_index)));
	}
}

void Mongo_Struct::create_data_arg(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder, int64_t key_index) {
	if(field_info.field_type == "int8") {
		builder << field_info.field_name << 0;
		buffer.write_int8(0);
	}
	else if(field_info.field_type == "int16") {
		builder << field_info.field_name << 0;
		buffer.write_int16(0);
	}
	else if(field_info.field_type == "int32") {
		builder << field_info.field_name << 0;
		buffer.write_int32(0);
	}
	else if(field_info.field_type == "int64") {
		int64_t value = 0;
		if(field_info.field_name == index_name())
			value = key_index;
		builder << field_info.field_name << (long long int)value;
		buffer.write_int64(value);
	}
	else if(field_info.field_type == "double") {
		builder << field_info.field_name << 0;
		buffer.write_double(0);
	}
	else if(field_info.field_type == "bool") {
		builder << field_info.field_name << false;
		buffer.write_bool(false);
	}
	else if(field_info.field_type == "string") {
		builder << field_info.field_name << "";
		buffer.write_string("");
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
}

void Mongo_Struct::create_data_vector(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder){
	std::vector<BSONObj> builder_vec;
	builder << field_info.field_name << builder_vec;
	buffer.write_uint16(0);
}

void Mongo_Struct::create_data_struct(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder) {
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	std::vector<Field_Info> field_vec = it->second->field_vec();
	BSONObjBuilder obj_builder;
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg"){
			create_data_arg(*iter, buffer, obj_builder, 0);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			create_data_vector(*iter, buffer, obj_builder);
		}
		else if(iter->field_label == "struct") {
			create_data_struct(*iter, buffer, obj_builder);
		}
	}
	builder << field_info.field_type << obj_builder.obj();
}

void Mongo_Struct::build_buffer_arg(const Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj) {
	if(field_info.field_type == "int8") {
		int8_t value = bsonobj[field_info.field_name].numberInt();
		buffer.write_int8(value);
	}
	else if(field_info.field_type == "int16") {
		int16_t value = bsonobj[field_info.field_name].numberInt();
		buffer.write_int16(value);
	}
	else if(field_info.field_type == "int32") {
		int32_t value = bsonobj[field_info.field_name].numberInt();
		buffer.write_int32(value);
	}
	else if(field_info.field_type == "int64") {
		int64_t value = bsonobj[field_info.field_name].numberLong();
		buffer.write_int64(value);
	}
	else if(field_info.field_type == "double") {
		double value = bsonobj[field_info.field_name].numberDouble();
		buffer.write_double(value);
	}
	else if(field_info.field_type == "bool") {
		bool value = bsonobj[field_info.field_name].numberInt();
		buffer.write_bool(value);
	}
	else if(field_info.field_type == "string") {
		std::string value = bsonobj[field_info.field_name].valuestrsafe();
		buffer.write_string(value);
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
}

void Mongo_Struct::build_buffer_vector(const Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj) {
	BSONObj fieldobj = bsonobj.getObjectField(field_info.field_name);
	uint16_t count = fieldobj.nFields();
	buffer.write_uint16(count);

	BSONObjIterator field_iter(fieldobj);
	BSONObj obj;
	if(is_struct(field_info.field_type)) {
		while (field_iter.more()) {
			obj = field_iter.next().embeddedObject();
			build_buffer_struct(field_info, buffer, obj);
		}
	}
	else{
		while (field_iter.more()) {
			obj = field_iter.next().embeddedObject();
			build_buffer_arg(field_info, buffer, obj);
		}
	}
}

void Mongo_Struct::build_buffer_struct(const Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj) {
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	BSONObj fieldobj = bsonobj.getObjectField(field_info.field_type);
	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg") {
			build_buffer_arg(*iter, buffer, fieldobj);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			build_buffer_vector(*iter, buffer, fieldobj);
		}
		else if(iter->field_label == "struct") {
			build_buffer_struct(*iter, buffer, fieldobj);
		}
	}
}

void Mongo_Struct::build_bson_arg(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder){
	if(field_info.field_type == "int8") {
		int8_t value = 0;
		buffer.read_int8(value);
		builder << field_info.field_name << (int)value;
	}
	else if(field_info.field_type == "int16") {
		int16_t value = 0;
		buffer.read_int16(value);
		builder << field_info.field_name << (int)value;
	}
	else if(field_info.field_type == "int32") {
		int32_t value = 0;
		buffer.read_int32(value);
		builder << field_info.field_name << (int)value;
	}
	else if(field_info.field_type == "int64") {
		int64_t value = 0;
		buffer.read_int64(value);
		builder << field_info.field_name << (long long int)value;
	}
	else if(field_info.field_type == "double") {
		double value = 0;
		buffer.read_double(value);
		builder << field_info.field_name << value;
	}
	else if(field_info.field_type == "bool") {
		bool value = false;
		buffer.read_bool(value);
		builder << field_info.field_name << value;
	}
	else if(field_info.field_type == "string") {
		std::string value = "";
		buffer.read_string(value);
		builder << field_info.field_name << value;
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
}

void Mongo_Struct::build_bson_vector(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder) {
	std::vector<BSONObj> bson_vec;
	uint16_t vec_size = 0;
	buffer.read_uint16(vec_size);
	if(is_struct(field_info.field_type)) {
		for(uint16_t i = 0; i < vec_size; ++i) {
			BSONObjBuilder obj_builder;
			build_bson_struct(field_info, buffer, obj_builder);
			bson_vec.push_back(obj_builder.obj());
		}
	}
	else{
		for(uint16_t i = 0; i < vec_size; ++i) {
			BSONObjBuilder obj_builder;
			build_bson_arg(field_info, buffer, obj_builder);
			bson_vec.push_back(obj_builder.obj());
		}
	}

	builder << field_info.field_name << bson_vec;
}

void Mongo_Struct::build_bson_struct(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder){
	Struct_Name_Map::iterator it = DB_MANAGER->db_struct_name_map().find(field_info.field_type);
	if(it == DB_MANAGER->db_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	BSONObjBuilder obj_builder;
	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg"){
			build_bson_arg(*iter, buffer, obj_builder);
		}
		else if(iter->field_label == "vector" || iter->field_label == "map") {
			build_bson_vector(*iter, buffer, obj_builder);
		}
		else if(iter->field_label == "struct") {
			build_bson_struct(*iter, buffer, obj_builder);
		}
	}
	builder << field_info.field_type << obj_builder.obj();
}
