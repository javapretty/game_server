/*
 * Mysql_Struct.h
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#ifndef MYSQL_STRUCT_H_
#define MYSQL_STRUCT_H_

#include "DB_Struct.h"

class Mysql_Struct: public DB_Struct {
public:
	Mysql_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Mysql_Struct();

	virtual void create_data(int64_t index);
	virtual void load_data(int64_t index, Block_Buffer &buffer);
	virtual void save_data(Block_Buffer &buffer);
	virtual void save_data_vector(Block_Buffer &buffer);
	virtual void delete_data(Block_Buffer &buffer);

private:
	void create_data_arg(Field_Info &field_info, char *str_name, char *str_value, int64_t index);
	void create_data_vector(Field_Info &field_info, char *str_name, char *str_value);
	void create_data_struct(Field_Info &field_info, char *str_name, char *str_value);

	void build_bson_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str);
	void build_bson_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str);
	void build_bson_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str);

	void build_buffer_arg(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str);
	void build_buffer_vector(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str);
	void build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, std::string &sql_str);
};

#endif /* MYSQL_STRUCT_H_ */
