/*
 * Base_Struct.cpp
 *
 *  Created on: May 30, 2016
 *      Author: zhangyalei
 */

#include "Base_Struct.h"

Base_Struct::Base_Struct(Xml &xml, TiXmlNode *node):
	struct_name_(),
	table_name_(),
	key_index_(),
	msg_id_(0)
{
	field_vec_.clear();

	if(node){
		struct_name_ = xml.get_key(node);
		TiXmlNode* sub_node = xml.enter_node(node, struct_name_.c_str());
		if (sub_node) {
			std::string label_name;
			XML_LOOP_BEGIN(sub_node)
				if((label_name = xml.get_key(sub_node)) == "head"){
					table_name_ = xml.get_attr_str(sub_node, "table_name");
					key_index_ = xml.get_attr_str(sub_node, "key_index");
					msg_id_ = xml.get_attr_int(sub_node, "msg_id");
				} else {
					Field_Info field_info;
					field_info.field_label = label_name;
					field_info.field_type = xml.get_attr_str(sub_node, "type");
					field_info.field_name = xml.get_attr_str(sub_node, "name");
					field_info.key_type = xml.get_attr_str(sub_node, "key_type");
					field_info.key_name = xml.get_attr_str(sub_node, "key_name");
					field_vec_.push_back(field_info);
				}
			XML_LOOP_END(sub_node)
		}
	}
}

Base_Struct::~Base_Struct(){
	struct_name_.clear();
	table_name_.clear();
	msg_id_ = 0;
	field_vec_.clear();
}
