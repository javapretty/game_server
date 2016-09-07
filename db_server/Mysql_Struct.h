/*
 * Mysql_Struct.h
 *
 *  Created on: Jul 20, 2016
 *      Author: zhangyalei
 */

#ifndef MYSQL_STRUCT_H_
#define MYSQL_STRUCT_H_

#include "Base_Struct.h"
#include "Mysql_Conn.h"

class Mysql_Struct: public Base_Struct {
public:
	Mysql_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Mysql_Struct();

	virtual void create_data(int64_t key_index, Block_Buffer &buffer);
	virtual void load_data(int64_t key_index, Block_Buffer &buffer);
	virtual void save_data(Block_Buffer &buffer);
	virtual void save_data_vector(Block_Buffer &buffer);
	virtual void delete_data(Block_Buffer &buffer);

private:
	int create_data_arg(const Field_Info &field_info, Block_Buffer &buffer, Block_Buffer &blob_buffer);
	int create_data_vector(const Field_Info &field_info, Block_Buffer &buffer, Block_Buffer &blob_buffer);
	int create_data_struct(const Field_Info &field_info, Block_Buffer &buffer, Block_Buffer &blob_buffer);

	void build_buffer_arg(const Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result);
	void build_buffer_vector(const Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result);
	void build_buffer_struct(const Field_Info &field_info, Block_Buffer &buffer, sql::ResultSet *result);

	int build_len_arg(const Field_Info &field_info, Block_Buffer &buffer);
	int build_len_vector(const Field_Info &field_info, Block_Buffer &buffer);
	int build_len_struct(const Field_Info &field_info, Block_Buffer &buffer);
};

#endif /* MYSQL_STRUCT_H_ */
