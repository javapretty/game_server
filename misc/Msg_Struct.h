/*
 * Msg_Struct.h
 *
 *  Created on: Aug 2, 2016
 *      Author: zhangyalei
 */

#ifndef MSG_STRUCT_H_
#define MSG_STRUCT_H_

#include "include/v8.h"
#include "json/json.h"
#include "Base_Struct.h"

using namespace v8;

class Msg_Struct: public Base_Struct {
public:
	Msg_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Msg_Struct();

	v8::Local<v8::Object> build_http_msg_object(Isolate* isolate, int cid, int msg_id, const Json::Value &value);
	void build_http_msg_buffer(Isolate* isolate, v8::Local<v8::Object> object, std::string &str);

	v8::Local<v8::Object> build_msg_object(Isolate* isolate, int cid, int player_cid, int msg_id, int status, Block_Buffer &buffer);
	void build_msg_buffer(Isolate* isolate, v8::Local<v8::Object> object, Block_Buffer &buffer);

private:
	v8::Local<v8::Value> build_object_arg(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate);
	v8::Local<v8::Array> build_object_vector(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate);
	v8::Local<v8::Map> build_object_map(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate);
	v8::Local<v8::Object> build_object_struct(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate);

	void build_buffer_arg(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value);
	void build_buffer_vector(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value);
	void build_buffer_map(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value);
	void build_buffer_struct(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value);
};

#endif /* MSG_STRUCT_H_ */
