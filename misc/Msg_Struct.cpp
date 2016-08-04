/*
 * Msg_Struct.cpp
 *
 *  Created on: 2016年8月2日
 *      Author: zhangyalei
 */

#include "Log.h"
#include "Msg_Manager.h"

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
		for(std::vector<Field_Info>::iterator iter = field_vec_.begin();
				iter != field_vec_.end(); iter++){
			if((*iter).field_label == "arg"){
				build_object_arg(*iter, buffer, isolate, buf_obj);
			}
			else if((*iter).field_label == "vector"){
				build_object_vector(*iter, buffer, isolate, buf_obj);
			}
			else if((*iter).field_label == "struct"){
				build_object_struct(*iter, buffer, isolate, buf_obj);
			}
		}
	}
	return handle_scope.Escape(buf_obj);
}

void Msg_Struct::build_object_arg(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object) {
	if(field_info.field_type == "int8"){
		int8_t value = buffer.read_int8();
		object->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
				Int32::New(isolate, value)).FromJust();
	}
	else if(field_info.field_type == "int16"){
		int16_t value = buffer.read_int16();
		object->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
				Int32::New(isolate, value)).FromJust();
	}
	else if(field_info.field_type == "int32"){
		int32_t value = buffer.read_int32();
		object->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
				Int32::New(isolate, value)).FromJust();
	}
	else if(field_info.field_type == "int64"){
		double value = buffer.read_int64();
		object->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
				Number::New(isolate, value)).FromJust();
	}
	else if(field_info.field_type == "string"){
		std::string value = buffer.read_string();
		object->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
				String::NewFromUtf8(isolate, value.c_str(), NewStringType::kNormal).ToLocalChecked()).FromJust();
	}
	else if(field_info.field_type == "bool"){
		bool value = buffer.read_bool();
		object->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
				Boolean::New(isolate, value)).FromJust();
	}
	else if(field_info.field_type == "double"){
		double value = buffer.read_double();
		object->Set(isolate->GetCurrentContext(),
				String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
				Number::New(isolate, value)).FromJust();
	}
	else {
		LOG_ERROR("Can not find the field_type:%s", field_info.field_type.c_str());
	}
}

void Msg_Struct::build_object_vector(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object) {
	EscapableHandleScope handle_scope(isolate);
	uint16_t vec_size = buffer.read_uint16();
	Local<Array> array = Array::New(isolate, vec_size);
	if(is_struct(field_info.field_type)){
		for(uint16_t i = 0; i < vec_size; ++i) {
			Local<ObjectTemplate> localTemplate = ObjectTemplate::New(isolate);
			Local<Object> sub_object = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
			build_object_struct(field_info, buffer, isolate, sub_object);
			array->Set(isolate->GetCurrentContext(), i, sub_object).FromJust();
		}
	}
	else{
		for(uint16_t i = 0; i < vec_size; ++i) {
			if(field_info.field_type == "int8"){
				int8_t value = buffer.read_int8();
				array->Set(isolate->GetCurrentContext(), i, Int32::New(isolate, value)).FromJust();
			}
			else if(field_info.field_type == "int16"){
				int16_t value = buffer.read_int16();
				array->Set(isolate->GetCurrentContext(), i, Int32::New(isolate, value)).FromJust();
			}
			else if(field_info.field_type == "int32"){
				int32_t value = buffer.read_int32();
				array->Set(isolate->GetCurrentContext(), i, Int32::New(isolate, value)).FromJust();
			}
			else if(field_info.field_type == "int64"){
				double value = buffer.read_int64();
				array->Set(isolate->GetCurrentContext(), i, Number::New(isolate, value)).FromJust();
			}
			else if(field_info.field_type == "string"){
				std::string value = buffer.read_string();
				array->Set(isolate->GetCurrentContext(), i, String::NewFromUtf8(isolate, value.c_str(), NewStringType::kNormal).ToLocalChecked()).FromJust();
			}
			else if(field_info.field_type == "bool"){
				bool value = buffer.read_bool();
				array->Set(isolate->GetCurrentContext(), i, Boolean::New(isolate, value)).FromJust();
			}
			else if(field_info.field_type == "double"){
				double value = buffer.read_double();
				array->Set(isolate->GetCurrentContext(), i, Number::New(isolate, value)).FromJust();
			}
			else {
				LOG_ERROR("Can not find the field_type:%s", field_info.field_type.c_str());
			}
		}
	}
	//设置数组变量的值
	object->Set(isolate->GetCurrentContext(),
			String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
			handle_scope.Escape(array)).FromJust();
}

void Msg_Struct::build_object_struct(Field_Info &field_info, Block_Buffer &buffer, Isolate* isolate, v8::Local<v8::Object> object) {
	Struct_Name_Map::iterator iter = MSG_MANAGER->msg_struct_name_map().find(field_info.field_type);
	if(iter == MSG_MANAGER->msg_struct_name_map().end()){
		LOG_ERROR("Can not find the struct_name:%s", field_info.field_type.c_str());
		return;
	}

	EscapableHandleScope handle_scope(isolate);
	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(isolate);
	Local<Object> sub_object = localTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
	Base_Struct *msg_struct  = iter->second;
	std::vector<Field_Info> field_vec = msg_struct->field_vec();
	for(std::vector<Field_Info>::iterator iter = field_vec.begin();
			iter != field_vec.end(); iter++){
		if((*iter).field_label == "arg"){
			build_object_arg(*iter, buffer, isolate, sub_object);
		}
		else if((*iter).field_label == "vector"){
			build_object_vector(*iter, buffer, isolate, sub_object);
		}
		else if((*iter).field_label == "struct"){
			build_object_struct(*iter, buffer, isolate, sub_object);
		}
	}
	//设置结构体变量类型的值
	object->Set(isolate->GetCurrentContext(),
			String::NewFromUtf8(isolate, field_info.field_name.c_str(), NewStringType::kNormal).ToLocalChecked(),
			handle_scope.Escape(sub_object)).FromJust();
}
