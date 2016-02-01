/*
 * myclass_tempate.cpp
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#include "myclass_template.h"
#include "object_wrap.h"
#include "V8_Base.h"

using namespace v8;

Global<ObjectTemplate> gMyClassTemplate;
ObjectWrap* gMyClass = NULL;

void jsMyClassGetNumber(
	Local<String> property,
	const PropertyCallbackInfo<Value>& info)
{
	Local<Object> self = info.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	int value = static_cast<MyClass*>(pMyClass)->getNumber();
	info.GetReturnValue().Set(value);
}

void jsMyClassSetNumber(
	Local<String> property,
	Local<Value> value,
	const PropertyCallbackInfo<void>& info)
{
	Local<Object> self = info.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	static_cast<MyClass*>(pMyClass)->setNumber(value->Int32Value());
}

void jsMyClassGetName(
	Local<String> property,
	const PropertyCallbackInfo<Value>& info)
{
	Local<Object> self = info.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	const char* name = static_cast<MyClass*>(pMyClass)->getName();
	Local<String> v8Str = String::NewFromUtf8(info.GetIsolate(), name);
	info.GetReturnValue().Set(v8Str);
}

void jsMyClassSetName(
	Local<String> property,
	Local<Value> value,
	const PropertyCallbackInfo<void>& info)
{
	Local<Object> self = info.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	v8::String::Utf8Value str(value);
	static_cast<MyClass*>(pMyClass)->setName(ToCString(str));
}

void jsMyClassMethod1(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Local<Object> self = args.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	pMyClass->Method1();
}

void jsMyClassMethod2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
		return;
	}

	v8::String::Utf8Value str(args[0]);
	const char* cstr = ToCString(str);

	Local<Object> self = args.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	pMyClass->Method2(cstr);
}

void jsCreateMyClass(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
		return;
	}

	Isolate* isolate = args.GetIsolate();

	Local<ObjectTemplate> myClassTemplate =
		Local<ObjectTemplate>::New(isolate, gMyClassTemplate);

	Local<Object> myClassObj = myClassTemplate->NewInstance(
		isolate->GetCurrentContext()).ToLocalChecked();

	int numValue =
		args[0]->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);

	gMyClass = new ObjectWrap(numValue);
	gMyClass->Wrap(myClassObj);

	args.GetReturnValue().Set(myClassObj);
}

void jsMyFunction(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	myFunction();
}

void jsMyFunction1(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
		return;
	}

	v8::String::Utf8Value str(args[0]);
	const char* cstr = ToCString(str);

	myFunction1(cstr);
}

void jsMyFunction2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			v8::NewStringType::kNormal).ToLocalChecked());
		return;
	}

	Isolate* isolate = args.GetIsolate();

	int numValue =
		args[0]->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);

	const char* result = myFunction2(numValue);

	Local<String> str = String::NewFromUtf8(isolate, result);
	args.GetReturnValue().Set(str);
}

void RegisterMyClass(Isolate* isolate)
{
	if (!gMyClassTemplate.IsEmpty())
	{
		return;
	}

	Local<ObjectTemplate> localTemplate = ObjectTemplate::New(isolate);
	localTemplate->SetInternalFieldCount(1);

	localTemplate->SetAccessor(
		String::NewFromUtf8(isolate, "num", NewStringType::kInternalized)
		.ToLocalChecked(),
		jsMyClassGetNumber,
		jsMyClassSetNumber);
	localTemplate->SetAccessor(
		String::NewFromUtf8(isolate, "name", NewStringType::kInternalized)
		.ToLocalChecked(),
		jsMyClassGetName,
		jsMyClassSetName);
	localTemplate->Set(
		v8::String::NewFromUtf8(isolate, "Method1", v8::NewStringType::kNormal)
		.ToLocalChecked(),
		v8::FunctionTemplate::New(isolate, jsMyClassMethod1));
	localTemplate->Set(
		v8::String::NewFromUtf8(isolate, "Method2", v8::NewStringType::kNormal)
		.ToLocalChecked(),
		v8::FunctionTemplate::New(isolate, jsMyClassMethod2));

	gMyClassTemplate.Reset(isolate, localTemplate);
}

v8::Local<v8::Context> CreateShellContext(v8::Isolate* isolate)
{
	// Register classes
	RegisterMyClass(isolate);

	// Register functions
	v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
	global->Set(
		v8::String::NewFromUtf8(isolate, "MyClass", v8::NewStringType::kNormal)
		.ToLocalChecked(),
		v8::FunctionTemplate::New(isolate, jsCreateMyClass));
	global->Set(
		v8::String::NewFromUtf8(isolate, "jsMyFunction", v8::NewStringType::kNormal)
		.ToLocalChecked(),
		v8::FunctionTemplate::New(isolate, jsMyFunction));
	global->Set(
		v8::String::NewFromUtf8(isolate, "jsMyFunction1", v8::NewStringType::kNormal)
		.ToLocalChecked(),
		v8::FunctionTemplate::New(isolate, jsMyFunction1));
	global->Set(
		v8::String::NewFromUtf8(isolate, "jsMyFunction2", v8::NewStringType::kNormal)
		.ToLocalChecked(),
		v8::FunctionTemplate::New(isolate, jsMyFunction2));

	return v8::Context::New(isolate, NULL, global);
}

void UnregisterAll()
{
	gMyClassTemplate.Reset();
}
