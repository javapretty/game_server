/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#ifndef V8_EXCEPTION_H_
#define V8_EXCEPTION_H_

#include <string>
#include <vector>
#include "include/v8.h"

namespace v8_wrap {

inline v8::Handle<v8::Value> throw_ex(v8::Isolate* isolate, char const* str,
	v8::Local<v8::Value> (*exception_ctor)(v8::Handle<v8::String>) = v8::Exception::Error)
{
	v8::EscapableHandleScope scope(isolate);
	v8::Handle<v8::String> message;
	message = v8::String::NewFromUtf8(isolate, str);
	return scope.Escape(isolate->ThrowException(exception_ctor(message)));
}

inline v8::Handle<v8::Value> throw_ex(v8::Isolate* isolate, std::string const& str,
	v8::Local<v8::Value> (*exception_ctor)(v8::Handle<v8::String>) = v8::Exception::Error)
{
	return throw_ex(isolate, str.c_str(), exception_ctor);
}

} // namespace v8_wrap

#endif // V8_EXCEPTION_H_
