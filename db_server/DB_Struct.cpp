/*
 * DB_Struct.cpp
 *
 *  Created on: May 30, 2016
 *      Author: lijunliang
 */

#include "DB_Struct.h"

DB_Struct::DB_Struct(Xml &xml, TiXmlNode *node):
	struct_name_(),
	db_name_(),
	index_(),
	msg_id_(0)
{
	field_vec_.clear();

	if(node){
		struct_name_ = xml.get_key(node);
		TiXmlNode* sub = xml.enter_node(node, struct_name_.c_str());
		std::string label_name;
		XML_LOOP_BEGIN(sub)
			if((label_name = xml.get_key(sub)) == "db"){
				db_name_ = xml.get_attr_str(sub, "dbname");
				msg_id_ = xml.get_attr_int(sub, "msgid");
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

DB_Struct::~DB_Struct(){
	struct_name_.clear();
	db_name_.clear();
	msg_id_ = 0;
	field_vec_.clear();
}
