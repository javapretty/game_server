/*
 * Mongo_Struct.h
 *
 *  Created on: Jul 20, 2016
 *      Author: zhangyalei
 */

#ifndef MONGO_STRUCT_H_
#define MONGO_STRUCT_H_

#include "Base_Struct.h"
#include "mongo/client/dbclient.h"

using namespace mongo;
class Mongo_Struct: public Base_Struct {
public:
	Mongo_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Mongo_Struct();

	virtual void create_data(int64_t key_index, Block_Buffer &buffer);
	virtual void load_data(int64_t key_index, Block_Buffer &buffer);
	virtual void save_data(Block_Buffer &buffer);
	virtual void save_data_vector(Block_Buffer &buffer);
	virtual void delete_data(Block_Buffer &buffer);

private:
	void create_data_arg(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder, int64_t key_index);
	void create_data_vector(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
	void create_data_struct(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);

	void build_buffer_arg(const Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_buffer_vector(const Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);
	void build_buffer_struct(const Field_Info &field_info, Block_Buffer &buffer, BSONObj &bsonobj);

	void build_bson_arg(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_vector(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
	void build_bson_struct(const Field_Info &field_info, Block_Buffer &buffer, BSONObjBuilder &builder);
};

#endif /* MONGO_STRUCT_H_ */
