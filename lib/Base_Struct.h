/*
 * Base_Struct.h
 *
 *  Created on: May 30, 2016
 *      Author: zhangyalei
 */
 
#ifndef BASE_STRUCT_H_
#define BASE_STRUCT_H_

#include "Xml.h"
#include "Block_Buffer.h"

struct Field_Info {
	std::string field_label;
	std::string field_type;
	std::string field_name;
	std::string key_type;
	std::string key_name;
};

class Base_Struct {
public:
	Base_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Base_Struct();

	virtual void create_data(int64_t key_index, Block_Buffer &buffer) {}
	virtual void load_data(int64_t key_index, Block_Buffer &buffer) {}
	virtual void save_data(Block_Buffer &buffer) {}
	virtual void save_data_vector(Block_Buffer &buffer) {}
	virtual void delete_data(Block_Buffer &buffer) {}

	inline const std::string &struct_name() { return struct_name_; }
	inline const std::string &table_name() { return table_name_; }
	inline void set_table_name(const std::string &table_name) { table_name_ = table_name; }
	inline const std::string &index_name() { return index_name_; }
	inline int32_t msg_id() { return msg_id_; }
	inline const std::vector<Field_Info>& field_vec() { return field_vec_; }
	inline bool is_struct(const std::string &type);

private:
	std::string struct_name_;
	std::string table_name_;
	std::string index_name_;
	int32_t msg_id_;
	std::vector<Field_Info> field_vec_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Base_Struct::is_struct(const std::string &field_type){
	if(field_type == "int8" || field_type == "int16" || field_type == "int32" || field_type == "int64" ||
			field_type == "double" || field_type == "bool" || field_type == "string") return false;
	return true;
}

#endif /* BASE_STRUCT_H_ */
