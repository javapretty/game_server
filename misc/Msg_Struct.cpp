/*
 * Msg_Struct.cpp
 *
 *  Created on: 2016年8月2日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "V8_Wrap.h"
#include "Msg_Manager.h"

Msg_Struct::Msg_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {}

Msg_Struct::~Msg_Struct() {}

v8::Local<v8::Object> Msg_Struct::build_msg_object(Isolate* isolate, int cid, int player_cid, int msg_id, int status, Block_Buffer &buffer) {
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(isolate);
	Local<Object> buf_obj = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	buf_obj->Set(isolate->GetCurrentContext(),
			String::NewFromUtf8(isolate, "cid", NewStringType::kNormal).ToLocalChecked(),
			Int32::New(isolate, cid)).FromJust();
	buf_obj->Set(isolate->GetCurrentContext(),
			String::NewFromUtf8(isolate, "player_cid", NewStringType::kNormal).ToLocalChecked(),
			Int32::New(isolate, player_cid)).FromJust();
	buf_obj->Set(isolate->GetCurrentContext(),
			String::NewFromUtf8(isolate, "msg_id", NewStringType::kNormal).ToLocalChecked(),
			Int32::New(isolate, msg_id)).FromJust();
	buf_obj->Set(isolate->GetCurrentContext(),
			String::NewFromUtf8(isolate, "status", NewStringType::kNormal).ToLocalChecked(),
			Int32::New(isolate, status)).FromJust();
	if (msg_id == SYNC_DB_GAME_LOAD_PLAYER || msg_id == SYNC_DB_GAME_CREATE_PLAYER) {
		std::string account = buffer.read_string();
		buf_obj->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, "account", NewStringType::kNormal).ToLocalChecked(),
				String::NewFromUtf8(isolate, account.c_str(), NewStringType::kNormal).ToLocalChecked()).FromJust();
	}

	//消息返回成功加载数据
	if (status == 0) {
		for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
				iter != field_vec().end(); iter++) {
			if(iter->field_label == "arg") {
				Local<Value> value = build_object_arg(*iter, buffer, isolate);
				buf_obj->Set(isolate->GetCurrentContext(),
						String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
						value).FromJust();
			}
			else if(iter->field_label == "vector") {
				Local<Array> array = build_object_vector(*iter, buffer, isolate);
				buf_obj->Set(isolate->GetCurrentContext(),
						String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
						array).FromJust();
			}
			else if(iter->field_label == "map") {
				Local<Map> map = build_object_map(*iter, buffer, isolate);
				buf_obj->Set(isolate->GetCurrentContext(),
						String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
						map).FromJust();
			}
			else if(iter->field_label == "struct") {
				Local<Object> object = build_object_struct(*iter, buffer, isolate);
				buf_obj->Set(isolate->GetCurrentContext(),
						String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
						object).FromJust();
			}
		}
	}
	return handle_scope.Escape(buf_obj);
}

void Msg_Struct::build_msg_buffer(Isolate* isolate, v8::Local<v8::Object> object, Block_Buffer &buffer) {
	for(std::vector<Field_Info>::const_iterator iter = field_vec().begin();
			iter != field_vec().end(); iter++) {
		Local<Value> value = object->Get(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
		if(iter->field_label == "arg") {
			build_buffer_arg(*iter, buffer, isolate, value);
		}
		else if(iter->field_label == "vector") {
			build_buffer_vector(*iter, buffer, isolate, value);
		}
		else if(iter->field_label == "map") {
			build_buffer_map(*iter, buffer, isolate, value);
		}
		else if(iter->field_label == "struct") {
			build_buffer_struct(*iter, buffer, isolate, value);
		}
	}
}

v8::Local<v8::Value> Msg_Struct::build_object_arg(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate) {
	EscapableHandleScope handle_scope(isolate);

	Local<Value> value;
	if(field_info.field_type == "int8") {
		int8_t val = buffer.read_int8();
		value = Int32::New(isolate, val);
	}
	else if(field_info.field_type == "int16") {
		int16_t val = buffer.read_int16();
		value = Int32::New(isolate, val);
	}
	else if(field_info.field_type == "int32") {
		int32_t val = buffer.read_int32();
		value = Int32::New(isolate, val);
	}
	else if(field_info.field_type == "int64") {
		double val = buffer.read_int64();
		value = Number::New(isolate, val);
	}
	else if(field_info.field_type == "double") {
		double val = buffer.read_double();
		value = Number::New(isolate, val);
	}
	else if(field_info.field_type == "bool") {
		bool val = buffer.read_bool();
		value = Boolean::New(isolate, val);
	}
	else if(field_info.field_type == "string") {
		std::string val = buffer.read_string();
		value = String::NewFromUtf8(isolate, val.c_str(), NewStringType::kNormal).ToLocalChecked();
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
		return handle_scope.Escape(Local<Value>());
	}
	return handle_scope.Escape(value);
}

v8::Local<v8::Array> Msg_Struct::build_object_vector(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate) {
	EscapableHandleScope handle_scope(isolate);

	uint16_t vec_size = buffer.read_uint16();
	Local<Array> array = Array::New(isolate, vec_size);
	if(is_struct(field_info.field_type)) {
		for(uint16_t i = 0; i < vec_size; ++i) {
			Local<Object> object = build_object_struct(field_info, buffer, isolate);
			array->Set(isolate->GetCurrentContext(), i, object).FromJust();
		}
	}
	else {
		for(uint16_t i = 0; i < vec_size; ++i) {
			Local<Value> value = build_object_arg(field_info, buffer, isolate);
			array->Set(isolate->GetCurrentContext(), i, value).FromJust();
		}
	}

	return handle_scope.Escape(array);
}

v8::Local<v8::Map> Msg_Struct::build_object_map(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate) {
	EscapableHandleScope handle_scope(isolate);

	uint16_t vec_size = buffer.read_uint16();
	Local<Map> map = Map::New(isolate);
	if(is_struct(field_info.field_type)) {
		for(uint16_t i = 0; i < vec_size; ++i) {
			Local<Object> object = build_object_struct(field_info, buffer, isolate);
			Local<Value> key = object->Get(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, field_info.key_name.c_str(), NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
			map->Set(isolate->GetCurrentContext(), key, object).ToLocalChecked();
		}
	}
	else {
		Field_Info key_info;
		key_info.field_label = "args";
		key_info.field_type = field_info.key_type;
		key_info.field_name = field_info.key_name;
		Local<Value> key = build_object_arg(key_info, buffer, isolate);
		Local<Value> value = build_object_arg(field_info, buffer, isolate);

		map->Set(isolate->GetCurrentContext(), key, value).ToLocalChecked();
	}

	return handle_scope.Escape(map);
}

v8::Local<v8::Object> Msg_Struct::build_object_struct(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate) {
	EscapableHandleScope handle_scope(isolate);

	Struct_Name_Map::iterator it = MSG_MANAGER->msg_struct_name_map().find(field_info.field_type);
	if(it == MSG_MANAGER->msg_struct_name_map().end()){
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return handle_scope.Escape(Local<Object>());
	}

	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(isolate);
	Local<Object> object = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		if(iter->field_label == "arg") {
			Local<Value> value = build_object_arg(*iter, buffer, isolate);
			object->Set(isolate->GetCurrentContext(),
								String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
								value).FromJust();
		}
		else if(iter->field_label == "vector") {
			Local<Array> array = build_object_vector(*iter, buffer, isolate);
			object->Set(isolate->GetCurrentContext(),
								String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
								array).FromJust();
		}
		else if(iter->field_label == "map") {
			Local<Map> map = build_object_map(*iter, buffer, isolate);
			object->Set(isolate->GetCurrentContext(),
								String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
								map).FromJust();
		}
		else if(iter->field_label == "struct") {
			Local<Object> sub_object = build_object_struct(*iter, buffer, isolate);
			object->Set(isolate->GetCurrentContext(),
								String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
								sub_object).FromJust();
		}
	}

	return handle_scope.Escape(object);
}

void Msg_Struct::build_buffer_arg(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value) {
	if(field_info.field_type == "int8") {
		int8_t val = 0;
		if (value->IsInt32()) {
			val = value->Int32Value(isolate->GetCurrentContext()).FromJust();
		}
		buffer.write_int8(val);
	}
	else if(field_info.field_type == "int16") {
		int16_t val = 0;
		if (value->IsInt32()) {
			val = value->Int32Value(isolate->GetCurrentContext()).FromJust();
		}
		buffer.write_int16(val);
	}
	else if(field_info.field_type == "int32") {
		int32_t val = 0;
		if (value->IsInt32()) {
			val = value->Int32Value(isolate->GetCurrentContext()).FromJust();
		}
		buffer.write_int32(val);
	}
	else if(field_info.field_type == "int64") {
		int64_t val = 0;
		if (value->IsNumber()) {
			val = value->NumberValue(isolate->GetCurrentContext()).FromJust();
		}
		buffer.write_int64(val);
	}
	else if(field_info.field_type == "double") {
		double val = 0;
		if (value->IsNumber()) {
			val = value->NumberValue(isolate->GetCurrentContext()).FromJust();
		}
		buffer.write_double(val);
	}
	else if(field_info.field_type == "bool") {
		bool val = 0;
		if (value->IsBoolean()) {
			val = value->BooleanValue(isolate->GetCurrentContext()).FromJust();
		}
		buffer.write_bool(val);
	}
	else if(field_info.field_type == "string") {
		std::string val = "";
		if (value->IsString()) {
			String::Utf8Value str(value->ToString(isolate->GetCurrentContext()).ToLocalChecked());
			std::stringstream stream;
			stream << ToCString(str);
			val = stream.str();
		}
		buffer.write_string(val);
	}
	else {
		LOG_ERROR("Can not find the field_type:%s, struct_name:%s", field_info.field_type.c_str(), struct_name().c_str());
	}
}

void Msg_Struct::build_buffer_vector(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value) {
	if (!value->IsArray()) {
		LOG_ERROR("field_name:%s is not array, struct_name:%s", field_info.field_name.c_str(), struct_name().c_str());
		buffer.write_uint16(0);
		return;
	}

	Local<Array> array = Local<Array>::Cast(value);
	int16_t len = array->Length();
	buffer.write_uint16(len);
	for (int i = 0; i < len; ++i) {
		Local<Value> element = array->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
		if(is_struct(field_info.field_type)) {
			build_buffer_struct(field_info, buffer, isolate, element);
		}
		else {
			build_buffer_arg(field_info, buffer, isolate, element);
		}
	}
}

void Msg_Struct::build_buffer_map(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value) {
	if (!value->IsMap()) {
		LOG_ERROR("field_name:%s is not map, struct_name:%s", field_info.field_name.c_str(), struct_name().c_str());
		buffer.write_uint16(0);
		return;
	}

	Local<Map> map = Local<Map>::Cast(value);
	int16_t len = map->Size();
	buffer.write_uint16(len);
	Local<Array> array = map->AsArray();
	//index N is the Nth key and index N + 1 is the Nth value.
	if(is_struct(field_info.field_type)){
		for (int i = 0; i < len * 2; i = i + 2) {
			Local<Value> element = array->Get(isolate->GetCurrentContext(), i + 1).ToLocalChecked();
			build_buffer_struct(field_info, buffer, isolate, element);
		}
	}
	else {
		Field_Info key_info;
		key_info.field_label = "args";
		key_info.field_type = field_info.key_type;
		key_info.field_name = field_info.key_name;
		for (int i = 0; i < len * 2; i = i + 2) {
			Local<Value> key = array->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
			Local<Value> element = array->Get(isolate->GetCurrentContext(), i + 1).ToLocalChecked();
			build_buffer_struct(key_info, buffer, isolate, key);
			build_buffer_struct(field_info, buffer, isolate, element);
		}
	}
}

void Msg_Struct::build_buffer_struct(const Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Value> value) {
	Struct_Name_Map::iterator it = MSG_MANAGER->msg_struct_name_map().find(field_info.field_type);
	if(it == MSG_MANAGER->msg_struct_name_map().end()) {
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	if (!value->IsObject()) {
		LOG_ERROR("field_type:%s field_name:%s is not object, struct_name:%s", field_info.field_type.c_str(), field_info.field_name.c_str(), struct_name().c_str());
		return;
	}

	Local<Object> object = value->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
	std::vector<Field_Info> field_vec = it->second->field_vec();
	for(std::vector<Field_Info>::const_iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++) {
		Local<Value> element = object->Get(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, iter->field_name.c_str(), NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
		if(iter->field_label == "arg") {
			build_buffer_arg(*iter, buffer, isolate, element);
		}
		else if(iter->field_label == "vector") {
			build_buffer_vector(*iter, buffer, isolate, element);
		}
		else if(iter->field_label == "map") {
			build_buffer_map(*iter, buffer, isolate, element);
		}
		else if(iter->field_label == "struct") {
			build_buffer_struct(*iter, buffer, isolate, element);
		}
	}
}
