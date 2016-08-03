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

	v8::Local<v8::Object> build_object(Isolate* isolate, Block_Buffer &buffer);

private:
	void build_object_arg(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object);
	void build_object_vector(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object);
	void build_object_struct(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object);
};

class Msg_Manager {
public:
	static Msg_Manager *instance(void);

	int init(void);

	inline Struct_Id_Map& msg_struct_id_map() { return msg_struct_id_map_; }
	inline Struct_Name_Map& msg_struct_name_map() { return msg_struct_name_map_; }

private:
	Msg_Manager(void);
	virtual ~Msg_Manager(void);
	Msg_Manager(const Msg_Manager &);
	const Msg_Manager &operator=(const Msg_Manager &);

private:
	static Msg_Manager *instance_;

	Struct_Id_Map msg_struct_id_map_;
	Struct_Name_Map msg_struct_name_map_;
};

#define MSG_MANAGER	Msg_Manager::instance()

#endif /* MSG_STRUCT_H_ */
