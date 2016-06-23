/*
 * DB_Definition.h
 *
 *  Created on: May 30, 2016
 *      Author: lijunliang
 */
 
#ifndef DB_DEFINITION_H_
#define DB_DEFINITION_H_

#include "xml.h"
#include "Block_Buffer.h"
#include "mongo/client/dbclient.h"
#include <vector>

using namespace mongo;

struct DB_Type_Description {
	std::string label_name;
	std::string type_name;
	std::string value_name;
};

typedef std::vector<DB_Type_Description> DB_TYPE_DESC_VEC;

class DB_Definition {
public:
	DB_Definition(Xml &xml, TiXmlNode *node);
	~DB_Definition();
	void init_table(int64_t index);
	void save_data(Block_Buffer &buffer);
	void load_data(int64_t index, Block_Buffer &buffer);
	void delete_data(Block_Buffer &buffer);
	int32_t cmdid(){return cmdid_;}
	std::string def_name(){return def_name_;}
	std::string dbname(){return dbname_;}
	std::string index(){return index_;}
	DB_TYPE_DESC_VEC& type_vec(){return type_vec_;}
private:
	void init_table_arg(DB_Type_Description &des, BSONObjBuilder &builder, int64_t role_id);
	void init_table_vector(DB_Type_Description &des, BSONObjBuilder &builder);
	void init_table_fixed(DB_Type_Description &des, BSONObjBuilder &builder);

	void build_bson_obj_arg(DB_Type_Description &des, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_obj_vector(DB_Type_Description &des, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_obj_fixed(DB_Type_Description &des, Block_Buffer &buffer, BSONObjBuilder &builder);

	void build_struct_arg(DB_Type_Description &des, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_struct_vector(DB_Type_Description &des, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_struct_fixed(DB_Type_Description &des, Block_Buffer &buffer, BSONObj &bsonobj);
	bool is_fixed_type(std::string type);
	bool is_arg(std::string &dst_label, std::string src_label);
private:
	std::string def_name_;
	std::string dbname_;
	std::string index_;
	int32_t cmdid_;
	DB_TYPE_DESC_VEC type_vec_;
};

#endif
