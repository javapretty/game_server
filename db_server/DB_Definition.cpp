/*
 * DB_Definition.cpp
 *
 *  Created on: May 30, 2016
 *      Author: lijunliang
 */

#include "DB_Definition.h"
#include "DB_Operator.h"
#include "DB_Manager.h"

DB_Definition::DB_Definition(Xml &xml, TiXmlNode *node):
	defname_(),
	dbname_(),
	index_(),
	msgid_(0)
{
	field_vec_.clear();

	if(node){
		defname_ = xml.get_key(node);
		TiXmlNode* sub = xml.enter_node(node, defname_.c_str());
		std::string label_name;
		XML_LOOP_BEGIN(sub)
			if((label_name = xml.get_key(sub)) == "db"){
				dbname_ = xml.get_attr_str(sub, "dbname");
				msgid_ = xml.get_attr_int(sub, "msgid");
				index_ = xml.get_attr_str(sub, "index");
			}
			else if(set_label(xml.get_key(sub), label_name)){
				Field_Info field_info;
				field_info.field_label = label_name;
				field_info.field_type = xml.get_attr_str(sub, "type");
				field_info.field_name = xml.get_attr_str(sub, "name");
				field_vec_.push_back(field_info);
			}
		XML_LOOP_END(sub)
	}
}

DB_Definition::~DB_Definition(){
	defname_.clear();
	dbname_.clear();
	msgid_ = 0;
	field_vec_.clear();
}

void DB_Definition::init_table(int64_t index){
	BSONObjBuilder set_builder ;
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++){
		if((*iter).field_label == "arg"){
			init_table_arg(*iter, set_builder, index);
		}
		else if((*iter).field_label == "vector"){
			init_table_vector(*iter, set_builder);
		}
		else if((*iter).field_label == "struct"){
			init_table_struct(*iter, set_builder);
		}
	}
	CACHED_CONNECTION.update(dbname_, MONGO_QUERY(index_ << (long long int)index),
				BSON("$set" << set_builder.obj() ), true);
}

void DB_Definition::save_data(Block_Buffer &buffer){
	BSONObjBuilder set_builder;
	int64_t index_value = buffer.peek_int64();
	for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
			iter != field_vec_.end(); iter++){
		if((*iter).field_label == "arg"){
			build_bson_arg(*iter, buffer, set_builder);
		}
		else if((*iter).field_label == "vector"){
			build_bson_vector(*iter, buffer, set_builder);
		}
		else if((*iter).field_label == "struct"){
			build_bson_struct(*iter, buffer, set_builder);
		}
	}
	CACHED_CONNECTION.update(dbname_, MONGO_QUERY(index_ << (long long int)index_value),
			BSON("$set" << set_builder.obj() ), true);
}

void DB_Definition::save_data_vector(Block_Buffer &buffer){
	uint16_t count = buffer.read_uint16();
	LOG_DEBUG("%s save list size is %d", dbname_.c_str(), count);
	for(int i = 0; i < count; i++){
		save_data(buffer);
	}
}

void DB_Definition::load_data(int64_t index, Block_Buffer &buffer){
	std::vector<BSONObj> total_record;
	if(index == 0){
		int total_count = CACHED_CONNECTION.count(dbname_);
		if(total_count > 0)
			CACHED_CONNECTION.findN(total_record, dbname_, Query(), total_count);
	}
	else {
		BSONObj obj = CACHED_CONNECTION.findOne(dbname_, MONGO_QUERY(index_ << (long long int)index));
		total_record.push_back(obj);
	}

	if(index == 0){
		buffer.write_uint16(total_record.size());
	}
	for (std::vector<BSONObj>::iterator iter = total_record.begin();
			iter != total_record.end(); ++iter) {
		for(std::vector<Field_Info>::iterator it = field_vec_.begin();
				it != field_vec_.end(); it++){
			if((*it).field_label == "arg"){
				build_buffer_arg(*it, buffer, *iter);
			}
			else if((*it).field_label == "vector"){
				build_buffer_vector(*it, buffer, *iter);
			}
			else if((*it).field_label == "struct"){
				build_buffer_struct(*it, buffer, *iter);
			}
		}
	}
}

void DB_Definition::delete_data(Block_Buffer &buffer) {
	uint16_t count = buffer.read_uint16();
	int64_t index = 0;
	for(int i = 0; i < count; i++){
		index = buffer.read_int64();
		CACHED_CONNECTION.remove(dbname_, MONGO_QUERY(index_ << (long long int)(index)));
	}
}

void DB_Definition::init_table_arg(Field_Info &field_info, BSONObjBuilder &builder, int64_t index){
	if(field_info.field_type == "int16"){
		int16_t value = 0;
		builder << field_info.field_name << (int)value;
	}
	else if(field_info.field_type == "int32"){
		int32_t value = 0;
		builder << field_info.field_name << (int)value;
	}
	else if(field_info.field_type == "int64"){
		int64_t value = 0;
		if(field_info.field_name == index_)
			value = index;
		builder << field_info.field_name << (long long int)value;
	}
	else if(field_info.field_type == "string"){
		std::string value = "";
		builder << field_info.field_name << value;
	}
	else if(field_info.field_type == "bool"){
		bool value = 0;
		builder << field_info.field_name << value;
	}
	else if(field_info.field_type == "double"){
		double value = 0;
		builder << field_info.field_name << value;
	}
	else {
		LOG_ERROR("Can not find the type %s", field_info.field_type.c_str());
	}
}

void DB_Definition::init_table_vector(Field_Info &field_info, BSONObjBuilder &builder){
	std::vector<BSONObj> builder_vec;
	builder << field_info.field_name << builder_vec;
}

void DB_Definition::init_table_struct(Field_Info &field_info, BSONObjBuilder &builder){
	DB_Manager::DB_Name_Definition_Map::iterator iter = DB_MANAGER->db_name_definition_map().find(field_info.field_type);
	if(iter == DB_MANAGER->db_name_definition_map().end()){
		LOG_ERROR("Can not find the module %s", field_info.field_type.c_str());
		return;
	}
	DB_Definition *def  = iter->second;

	std::vector<Field_Info> field_vec = def->field_vec();
	BSONObjBuilder obj_builder;
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++){
		if((*iter).field_label == "arg"){
			init_table_arg(*iter, obj_builder, 0);
		}
		else if((*iter).field_label == "vector"){
			init_table_vector(*iter, obj_builder);
		}
		else if((*iter).field_label == "struct"){
			init_table_struct(*iter, obj_builder);
		}
	}
	builder << field_info.field_type << obj_builder.obj();
}

void DB_Definition::build_bson_arg(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder){
	if(field_info.field_type == "int16"){
		int16_t value = buffer.read_int16();
		builder << field_info.field_name << (int)value;
	}
	else if(field_info.field_type == "int32"){
		int32_t value = buffer.read_int32();
		builder << field_info.field_name << (int)value;
	}
	else if(field_info.field_type == "int64"){
		int64_t value = buffer.read_int64();
		builder << field_info.field_name << (long long int)value;
	}
	else if(field_info.field_type == "string"){
		std::string value = buffer.read_string();
		builder << field_info.field_name << value;
	}
	else if(field_info.field_type == "bool"){
		bool value = buffer.read_bool();
		builder << field_info.field_name << value;
	}
	else if(field_info.field_type == "double"){
		double value = buffer.read_double();
		builder << field_info.field_name << value;
	}
	else {
		LOG_ERROR("Can not find the type %s", field_info.field_type.c_str());
	}
}

void DB_Definition::build_bson_vector(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder){
	std::vector<BSONObj> bson_vec;
	uint16_t vec_size = buffer.read_uint16();

	if(is_struct(field_info.field_type)){
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

void DB_Definition::build_bson_struct(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder){
	DB_Manager::DB_Name_Definition_Map::iterator iter = DB_MANAGER->db_name_definition_map().find(field_info.field_type);
	if(iter == DB_MANAGER->db_name_definition_map().end()){
		LOG_ERROR("Can not find the module %s", field_info.field_type.c_str());
		return;
	}

	DB_Definition *def  = iter->second;
	std::vector<Field_Info> field_vec = def->field_vec();
	BSONObjBuilder obj_builder;
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++){
		if((*iter).field_label == "arg"){
			build_bson_arg(*iter, buffer, obj_builder);
		}
		else if((*iter).field_label == "vector"){
			build_bson_vector(*iter, buffer, obj_builder);
		}
		else if((*iter).field_label == "struct"){
			build_bson_struct(*iter, buffer, obj_builder);
		}
	}
	builder << field_info.field_type << obj_builder.obj();
}

void DB_Definition::build_buffer_arg(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj) {
	if(field_info.field_type == "int16"){
		int16_t value = bsonobj[field_info.field_name].numberInt();
		buffer.write_int16(value);
	}
	else if(field_info.field_type == "int32"){
		int32_t value = bsonobj[field_info.field_name].numberInt();
		buffer.write_int32(value);
	}
	else if(field_info.field_type == "int64"){
		int64_t value = bsonobj[field_info.field_name].numberLong();
		buffer.write_int64(value);
	}
	else if(field_info.field_type == "string"){
		std::string value = bsonobj[field_info.field_name].valuestrsafe();
		buffer.write_string(value);
	}
	else if(field_info.field_type == "bool"){
		bool value = bsonobj[field_info.field_name].numberInt();
		buffer.write_bool(value);
	}
	else if(field_info.field_type == "double"){
		double value = bsonobj[field_info.field_name].numberDouble();
		buffer.write_double(value);
	}
	else {
		LOG_ERROR("Can not find the type %s", field_info.field_type.c_str());
	}
}

void DB_Definition::build_buffer_vector(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj) {
	BSONObj fieldobj = bsonobj.getObjectField(field_info.field_name);
	uint16_t count = fieldobj.nFields();
	buffer.write_uint16(count);

	BSONObjIterator field_iter(fieldobj);
	BSONObj obj;
	if(is_struct(field_info.field_type)){
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

void DB_Definition::build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj) {
	DB_Manager::DB_Name_Definition_Map::iterator iter = DB_MANAGER->db_name_definition_map().find(field_info.field_type);
	if(iter == DB_MANAGER->db_name_definition_map().end()){
		LOG_ERROR("Can not find the module %s", field_info.field_type.c_str());
		return;
	}

	DB_Definition *def  = iter->second;
	BSONObj fieldobj = bsonobj.getObjectField(field_info.field_type);
	std::vector<Field_Info> field_vec = def->field_vec();
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++){
		if((*iter).field_label == "arg"){
			build_buffer_arg(*iter, buffer, fieldobj);
		}
		else if((*iter).field_label == "vector"){
			build_buffer_vector(*iter, buffer, fieldobj);
		}
		else if((*iter).field_label == "struct"){
			build_buffer_struct(*iter, buffer, fieldobj);
		}
	}
}

bool DB_Definition::is_struct(std::string field_type){
	if(field_type == "int8" || field_type == "int16" || field_type == "int32" || field_type == "int64" ||field_type == "uint8" ||
			field_type == "uint16" || field_type == "uint32" || field_type == "uint64" || field_type == "double" || field_type == "bool" ||
			field_type == "string") return false;
	return true;
}

bool DB_Definition::set_label(std::string src_label, std::string &dst_label){
	if(src_label == "arg" || src_label == "struct"){
		dst_label = src_label;
		return true;
	} else if(src_label == "vector" || src_label == "unordered_map"){
		dst_label = "vector";
		return true;
	}
	return false;
}
