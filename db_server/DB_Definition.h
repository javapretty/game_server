/*
 * DB_Definition.h
 *
 *  Created on: May 30, 2016
 *      Author: lijunliang
 */
 
#ifndef DB_DEFINITION_H_
#define DB_DEFINITION_H_

#include "Xml.h"
#include "Block_Buffer.h"
#include "mongo/client/dbclient.h"
#include "Public_Struct.h"
#include <vector>

using namespace mongo;
class DB_Definition {
public:
	DB_Definition(Xml &xml, TiXmlNode *node);
	~DB_Definition();

	void init_table(int64_t index);
	void save_data(Block_Buffer &buffer);
	void save_data_vector(Block_Buffer &buffer);
	void load_data(int64_t index, Block_Buffer &buffer);
	void delete_data(Block_Buffer &buffer);

	int32_t msgid(){return msgid_;}
	std::string defname(){return defname_;}
	std::string dbname(){return dbname_;}
	std::string index(){return index_;}
	std::vector<Field_Info>& field_vec(){return field_vec_;}

private:
	void init_table_arg(Field_Info &field_info, BSONObjBuilder &builder, int64_t role_id);
	void init_table_vector(Field_Info &field_info, BSONObjBuilder &builder);
	void init_table_struct(Field_Info &field_info, BSONObjBuilder &builder);

	void build_bson_arg(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_vector(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_struct(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);

	void build_buffer_arg(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_buffer_vector(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);

	bool is_struct(std::string type);
	bool set_label(std::string src_label, std::string &dst_label);

private:
	std::string defname_;
	std::string dbname_;
	std::string index_;
	int32_t msgid_;
	std::vector<Field_Info> field_vec_;
};

#endif
