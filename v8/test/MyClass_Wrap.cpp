/*
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#include "MyClass_Wrap.h"

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
	String::Utf8Value str(value);
	static_cast<MyClass*>(pMyClass)->setName(ToCString(str));
}

void jsMyClassMethod1(const FunctionCallbackInfo<Value>& args)
{
	Local<Object> self = args.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	pMyClass->Method1();
}

void jsMyClassMethod2(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			NewStringType::kNormal).ToLocalChecked());
		return;
	}

	String::Utf8Value str(args[0]);
	const char* cstr = ToCString(str);

	Local<Object> self = args.Holder();
	ObjectWrap* pMyClass = ObjectWrap::Unwrap<ObjectWrap>(self);
	pMyClass->Method2(cstr);
}

void jsCreateMyClass(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			NewStringType::kNormal).ToLocalChecked());
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

void jsMyFunction(const FunctionCallbackInfo<Value>& args)
{
	myFunction();
}

void jsMyFunction1(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			NewStringType::kNormal).ToLocalChecked());
		return;
	}

	String::Utf8Value str(args[0]);
	const char* cstr = ToCString(str);

	myFunction1(cstr);
}

void jsMyFunction2(const FunctionCallbackInfo<Value>& args)
{
	if (args.Length() != 1)
	{
		args.GetIsolate()->ThrowException(
			String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
			NewStringType::kNormal).ToLocalChecked());
		return;
	}

	Isolate* isolate = args.GetIsolate();

	int numValue =
		args[0]->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);

	const char* result = myFunction2(numValue);

	Local<String> str = String::NewFromUtf8(isolate, result);
	args.GetReturnValue().Set(str);
}

void RegisterMyClass(Isolate* isolate, Local<ObjectTemplate> global)
{
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
		String::NewFromUtf8(isolate, "Method1", NewStringType::kNormal)
		.ToLocalChecked(),
		FunctionTemplate::New(isolate, jsMyClassMethod1));
	localTemplate->Set(
		String::NewFromUtf8(isolate, "Method2", NewStringType::kNormal)
		.ToLocalChecked(),
		FunctionTemplate::New(isolate, jsMyClassMethod2));

	gMyClassTemplate.Reset(isolate, localTemplate);

	global->Set(
		String::NewFromUtf8(isolate, "MyClass", NewStringType::kNormal)
		.ToLocalChecked(),
		FunctionTemplate::New(isolate, jsCreateMyClass));
	global->Set(
		String::NewFromUtf8(isolate, "jsMyFunction", NewStringType::kNormal)
		.ToLocalChecked(),
		FunctionTemplate::New(isolate, jsMyFunction));
	global->Set(
		String::NewFromUtf8(isolate, "jsMyFunction1", NewStringType::kNormal)
		.ToLocalChecked(),
		FunctionTemplate::New(isolate, jsMyFunction1));
	global->Set(
		String::NewFromUtf8(isolate, "jsMyFunction2", NewStringType::kNormal)
		.ToLocalChecked(),
		FunctionTemplate::New(isolate, jsMyFunction2));
}

void UnregisterMyClass()
{
	gMyClassTemplate.Reset();
}
