/*
 * Mongo_Struct.h
 *
 *  Created on: 2016年7月20日
 *      Author: zhangyalei
 */

#ifndef MONGO_STRUCT_H_
#define MONGO_STRUCT_H_

#include "mongo/client/dbclient.h"
#include "DB_Struct.h"

using namespace mongo;
class Mongo_Struct: public DB_Struct {
public:
	Mongo_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Mongo_Struct();

	virtual void create_data(int64_t index);
	virtual void load_data(int64_t index, Block_Buffer &buffer);
	virtual void save_data(Block_Buffer &buffer);
	virtual void save_data_vector(Block_Buffer &buffer);
	virtual void delete_data(Block_Buffer &buffer);

private:
	void create_data_arg(Field_Info &field_info, BSONObjBuilder &builder, int64_t index);
	void create_data_vector(Field_Info &field_info, BSONObjBuilder &builder);
	void create_data_struct(Field_Info &field_info, BSONObjBuilder &builder);

	void build_bson_arg(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_vector(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_struct(Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);

	void build_buffer_arg(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_buffer_vector(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_buffer_struct(Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);
};

#endif /* MONGO_STRUCT_H_ */
