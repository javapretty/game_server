/*
 * Msg_Struct.h
 *
 *  Created on: 2016年8月2日
 *      Author: zhangyalei
 */

#ifndef MSG_STRUCT_H_
#define MSG_STRUCT_H_

#include "include/v8.h"
#include "Base_Struct.h"

using namespace v8;

class Msg_Struct: public Base_Struct {
public:
	Msg_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {}
	virtual ~Msg_Struct() {}

	v8::Local<v8::Object> build_msg_object(Isolate* isolate, int cid, int player_cid, int msg_id, int status, Block_Buffer &buffer);

private:
	void build_object_arg(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object);
	void build_object_vector(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object);
	void build_object_struct(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object);
};

#endif /* MSG_STRUCT_H_ */
