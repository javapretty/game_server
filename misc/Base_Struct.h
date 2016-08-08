/*
 * Base_Struct.h
 *
 *  Created on: May 30, 2016
 *      Author: zhangyalei
 */
 
#ifndef BASE_STRUCT_H_
#define BASE_STRUCT_H_

#include "Xml.h"
#include "Public_Struct.h"

class Base_Struct {
public:
	Base_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Base_Struct();

	virtual void create_data(int64_t key_index) {}
	virtual void load_data(int64_t key_index, Block_Buffer &buffer) {}
	virtual void save_data(Block_Buffer &buffer) {}
	virtual void save_data_vector(Block_Buffer &buffer) {}
	virtual void delete_data(Block_Buffer &buffer) {}

	inline std::string &struct_name() { return struct_name_; }
	inline std::string &table_name() { return table_name_; }
	inline std::string &key_index() { return key_index_; }
	inline int32_t msg_id() { return msg_id_; }
	inline std::vector<Field_Info>& field_vec() { return field_vec_; }

protected:
	inline bool is_struct(const std::string &type);

	std::string struct_name_;
	std::string table_name_;
	std::string key_index_;
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
