/*
 * DB_Struct.h
 *
 *  Created on: May 30, 2016
 *      Author: lijunliang
 */
 
#ifndef DB_STRUCT_H_
#define DB_STRUCT_H_

#include "Xml.h"
#include "Public_Struct.h"

class DB_Struct {
public:
	DB_Struct(Xml &xml, TiXmlNode *node);
	virtual ~DB_Struct();

	virtual void create_data(int64_t index) = 0;
	virtual void load_data(int64_t index, Block_Buffer &buffer) = 0;
	virtual void save_data(Block_Buffer &buffer) = 0;
	virtual void save_data_vector(Block_Buffer &buffer) = 0;
	virtual void delete_data(Block_Buffer &buffer) = 0;

	inline std::string &struct_name() { return struct_name_; }
	inline std::string &db_name() { return db_name_; }
	inline std::string &index() { return index_; }
	inline int32_t msg_id() { return msg_id_; }
	inline std::vector<Field_Info>& field_vec() { return field_vec_; }

protected:
	inline bool is_struct(const std::string &type);
	inline bool set_label(const std::string &src_label, std::string &dst_label);

	std::string struct_name_;
	std::string db_name_;
	std::string index_;
	int32_t msg_id_;
	std::vector<Field_Info> field_vec_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
inline bool DB_Struct::is_struct(const std::string &field_type){
	if(field_type == "int8" || field_type == "int16" || field_type == "int32" || field_type == "int64" ||field_type == "uint8" ||
			field_type == "uint16" || field_type == "uint32" || field_type == "uint64" || field_type == "double" || field_type == "bool" ||
			field_type == "string") return false;
	return true;
}

inline bool DB_Struct::set_label(const std::string &src_label, std::string &dst_label){
	if(src_label == "arg" || src_label == "struct"){
		dst_label = src_label;
		return true;
	} else if(src_label == "vector" || src_label == "unordered_map"){
		dst_label = "vector";
		return true;
	}
	return false;
}

#endif
