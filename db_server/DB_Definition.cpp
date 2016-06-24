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
	def_name_(),
	dbname_(),
	index_(),
	cmdid_(0),
	type_vec_()
{
	if(node){
		def_name_ = xml.get_key(node);
		TiXmlNode* sub = xml.enter_node(node, def_name_.c_str());
		std::string label_name;
		XML_LOOP_BEGIN(sub)
			if((label_name = xml.get_key(sub)) == "persistence"){
				dbname_ = xml.get_attr_str(sub, "dbname");
				cmdid_ = xml.get_attr_int(sub, "cmdid");
				index_ = xml.get_attr_str(sub, "index");
			}
			else if(is_arg(label_name, xml.get_key(sub))){
				DB_Type_Description des;
				des.label_name = label_name;
				des.type_name = xml.get_attr_str(sub, "type");
				des.value_name = xml.get_attr_str(sub, "value");
				type_vec_.push_back(des);
			}
		XML_LOOP_END(sub)
	}
}

DB_Definition::~DB_Definition(){
	def_name_.clear();
	dbname_.clear();
	cmdid_ = 0;
	type_vec_.clear();
}

void DB_Definition::init_table(int64_t index){
	BSONObjBuilder set_builder ;
	for(std::vector<DB_Type_Description>::iterator iter = type_vec_.begin();
			iter != type_vec_.end(); iter++){
		if((*iter).label_name == "arg"){
			init_table_arg(*iter, set_builder, index);
		}
		else if((*iter).label_name == "vector"){
			init_table_vector(*iter, set_builder);
		}
		else if((*iter).label_name == "fixed"){
			init_table_fixed(*iter, set_builder);
		}
	}
	CACHED_CONNECTION.update(dbname_, MONGO_QUERY(index_ << (long long int)index),
				BSON("$set" << set_builder.obj() ), true);
}

void DB_Definition::save_data(Block_Buffer &buffer){
	BSONObjBuilder set_builder;
	int64_t index_value = buffer.peek_int64();
	for(std::vector<DB_Type_Description>::iterator iter = type_vec_.begin();
			iter != type_vec_.end(); iter++){
		if((*iter).label_name == "arg"){
			build_bson_obj_arg(*iter, buffer, set_builder);
		}
		else if((*iter).label_name == "vector"){
			build_bson_obj_vector(*iter, buffer, set_builder);
		}
		else if((*iter).label_name == "fixed"){
			build_bson_obj_fixed(*iter, buffer, set_builder);
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
		for(std::vector<DB_Type_Description>::iterator it = type_vec_.begin();
				it != type_vec_.end(); it++){
			if((*it).label_name == "arg"){
				build_struct_arg(*it, buffer, *iter);
			}
			else if((*it).label_name == "vector"){
				build_struct_vector(*it, buffer, *iter);
			}
			else if((*it).label_name == "fixed"){
				build_struct_fixed(*it, buffer, *iter);
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

void DB_Definition::init_table_arg(DB_Type_Description &des, BSONObjBuilder &builder, int64_t index){
	if(des.type_name == "int16"){
		int16_t value = 0;
		builder << des.value_name << (int)value;
	}
	else if(des.type_name == "int32"){
		int32_t value = 0;
		builder << des.value_name << (int)value;
	}
	else if(des.type_name == "int64"){
		int64_t value = 0;
		if(des.value_name == index_)
			value = index;
		builder << des.value_name << (long long int)value;
	}
	else if(des.type_name == "string"){
		std::string value = "";
		builder << des.value_name << value;
	}
	else if(des.type_name == "bool"){
		bool value = 0;
		builder << des.value_name << (int)value;
	}
	else {
		LOG_ERROR("Can not find the type %s", des.type_name.c_str());
	}
}

void DB_Definition::init_table_vector(DB_Type_Description &des, BSONObjBuilder &builder){
	std::vector<BSONObj> builder_vec;
	builder << des.value_name << builder_vec;
}

void DB_Definition::init_table_fixed(DB_Type_Description &des, BSONObjBuilder &builder){
	DB_Manager::DB_Name_Definition_Map::iterator iter = DB_MANAGER->db_name_definition_map().find(des.type_name);
	if(iter == DB_MANAGER->db_name_definition_map().end()){
		LOG_ERROR("Can not find the module %s", des.type_name.c_str());
		return;
	}
	DB_Definition *def  = iter->second;

	std::vector<DB_Type_Description> type_vec = def->type_vec();
	BSONObjBuilder obj_builder;
	for(std::vector<DB_Type_Description>::iterator iter = type_vec.begin();
			iter != type_vec.end(); iter++){
		if((*iter).label_name == "arg"){
			init_table_arg(*iter, obj_builder, 0);
		}
		else if((*iter).label_name == "vector"){
			init_table_vector(*iter, obj_builder);
		}
		else if((*iter).label_name == "fixed"){
			init_table_fixed(*iter, obj_builder);
		}
	}
	builder << des.type_name << obj_builder.obj();
}

void DB_Definition::build_bson_obj_arg(DB_Type_Description &des, Block_Buffer &buffer, BSONObjBuilder &builder){
	if(des.type_name == "int16"){
		int16_t value = buffer.read_int16();
		builder << des.value_name << (int)value;
	}
	else if(des.type_name == "int32"){
		int32_t value = buffer.read_int32();
		builder << des.value_name << (int)value;
	}
	else if(des.type_name == "int64"){
		int64_t value = buffer.read_int64();
		builder << des.value_name << (long long int)value;
	}
	else if(des.type_name == "string"){
		std::string value = buffer.read_string();
		builder << des.value_name << value;
	}
	else if(des.type_name == "bool"){
		bool value = buffer.read_bool();
		builder << des.value_name << (int)value;
	}
	else {
		LOG_ERROR("Can not find the type %s", des.type_name.c_str());
	}
}

void DB_Definition::build_bson_obj_vector(DB_Type_Description &des, Block_Buffer &buffer, BSONObjBuilder &builder){
	std::vector<BSONObj> bson_vec;
	uint16_t vec_size = buffer.read_uint16();

	if(is_fixed_type(des.type_name)){
		for(uint16_t i = 0; i < vec_size; ++i) {
			BSONObjBuilder obj_builder;
			build_bson_obj_fixed(des, buffer, obj_builder);
			bson_vec.push_back(obj_builder.obj());
		}
	}
	else{
		for(uint16_t i = 0; i < vec_size; ++i) {
			BSONObjBuilder obj_builder;
			build_bson_obj_arg(des, buffer, obj_builder);
			bson_vec.push_back(obj_builder.obj());
		}
	}

	builder << des.value_name << bson_vec;
}

void DB_Definition::build_bson_obj_fixed(DB_Type_Description &des, Block_Buffer &buffer, BSONObjBuilder &builder){
	DB_Manager::DB_Name_Definition_Map::iterator iter = DB_MANAGER->db_name_definition_map().find(des.type_name);
	if(iter == DB_MANAGER->db_name_definition_map().end()){
		LOG_ERROR("Can not find the module %s", des.type_name.c_str());
		return;
	}
	DB_Definition *def  = iter->second;

	std::vector<DB_Type_Description> type_vec = def->type_vec();
	BSONObjBuilder obj_builder;
	for(std::vector<DB_Type_Description>::iterator iter = type_vec.begin();
			iter != type_vec.end(); iter++){
		if((*iter).label_name == "arg"){
			build_bson_obj_arg(*iter, buffer, obj_builder);
		}
		else if((*iter).label_name == "vector"){
			build_bson_obj_vector(*iter, buffer, obj_builder);
		}
		else if((*iter).label_name == "fixed"){
			build_bson_obj_fixed(*iter, buffer, obj_builder);
		}
	}
	builder << des.type_name << obj_builder.obj();
}

void DB_Definition::build_struct_arg(DB_Type_Description &des, Block_Buffer &buffer, BSONObj &bsonobj) {
	if(des.type_name == "int16"){
		int16_t value = bsonobj[des.value_name].numberInt();
		buffer.write_int16(value);
	}
	else if(des.type_name == "int32"){
		int32_t value = bsonobj[des.value_name].numberInt();
		buffer.write_int32(value);
	}
	else if(des.type_name == "int64"){
		int64_t value = bsonobj[des.value_name].numberLong();
		buffer.write_int64(value);
	}
	else if(des.type_name == "string"){
		std::string value = bsonobj[des.value_name].valuestrsafe();
		buffer.write_string(value);
	}
	else if(des.type_name == "bool"){
		bool value = bsonobj[des.value_name].numberInt();
		buffer.write_bool(value);
	}
	else {
		LOG_ERROR("Can not find the type %s", des.type_name.c_str());
	}
}

void DB_Definition::build_struct_vector(DB_Type_Description &des, Block_Buffer &buffer, BSONObj &bsonobj) {
	BSONObj fieldobj = bsonobj.getObjectField(des.value_name);
	uint16_t count = fieldobj.nFields();
	buffer.write_uint16(count);

	BSONObjIterator field_iter(fieldobj);
	BSONObj obj;
	if(is_fixed_type(des.type_name)){
		while (field_iter.more()) {
			obj = field_iter.next().embeddedObject();
			build_struct_fixed(des, buffer, obj);
		}
	}
	else{
		while (field_iter.more()) {
			obj = field_iter.next().embeddedObject();
			build_struct_arg(des, buffer, obj);
		}
	}
}

void DB_Definition::build_struct_fixed(DB_Type_Description &des, Block_Buffer &buffer, BSONObj &bsonobj) {
	DB_Manager::DB_Name_Definition_Map::iterator iter = DB_MANAGER->db_name_definition_map().find(des.type_name);
	if(iter == DB_MANAGER->db_name_definition_map().end()){
		LOG_ERROR("Can not find the module %s", des.type_name.c_str());
		return;
	}
	DB_Definition *def  = iter->second;

	BSONObj fieldobj = bsonobj.getObjectField(des.type_name);
	std::vector<DB_Type_Description> type_vec = def->type_vec();
	for(std::vector<DB_Type_Description>::iterator iter = type_vec.begin();
			iter != type_vec.end(); iter++){
		if((*iter).label_name == "arg"){
			build_struct_arg(*iter, buffer, fieldobj);
		}
		else if((*iter).label_name == "vector"){
			build_struct_vector(*iter, buffer, fieldobj);
		}
		else if((*iter).label_name == "fixed"){
			build_struct_fixed(*iter, buffer, fieldobj);
		}
	}
}

bool DB_Definition::is_fixed_type(std::string type){
	if(type == "int8" || type == "int16" || type == "int32" || type == "int64" ||type == "uint8" ||
			type == "uint16" || type == "uint32" || type == "uint64" || type == "double" || type == "bool" ||
			type == "string")
		return false;
	return true;
}

bool DB_Definition::is_arg(std::string &dst_label, std::string src_label){
	if(src_label == "arg" || src_label == "fixed"){
		dst_label = src_label;
		return true;
	}
	else if(src_label == "vector" || src_label == "map" || src_label == "unordered_map"){
		dst_label = "vector";
		return true;
	}
	return false;
}
