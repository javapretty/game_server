/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#ifndef V8_OBJECT_H_
#define V8_OBJECT_H_

#include <cstring>
#include "include/v8.h"
#include "V8_Convert.h"

namespace v8_wrap {

/// Get optional value form V8 object by name.
/// Dot symbols in option name delimits subobjects name.
/// return false if the value doesn't exist in the options object
template<typename T>
bool get_option(v8::Isolate* isolate, v8::Handle<v8::Object> options,
	char const* name, T& value)
{
	char const* dot = strchr(name, '.');
	if (dot)
	{
		std::string const subname(name, dot);
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> suboptions;
		return get_option(isolate, options, subname.c_str(), suboptions)
			&& get_option(isolate, suboptions, dot + 1, value);
	}
	v8::Local<v8::Value> val = options->Get(v8_wrap::to_v8(isolate, name));
	if (val.IsEmpty() || val == v8::Undefined(isolate))
	{
		return false;
	}
	value = from_v8<T>(isolate, val);
	return true;
}

/// Set named value in V8 object
/// Dot symbols in option name delimits subobjects name.
/// return false if the value doesn't exists in the options subobject
template<typename T>
bool set_option(v8::Isolate* isolate, v8::Handle<v8::Object> options,
	char const* name, T const& value)
{
	char const* dot = strchr(name, '.');
	if (dot)
	{
		std::string const subname(name, dot);
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> suboptions;
		return get_option(isolate, options, subname.c_str(), suboptions)
			&& set_option(isolate, suboptions, dot + 1, value);
	}
	options->Set(v8_wrap::to_v8(isolate, name), to_v8(isolate, value));
	return true;
}

/// Set named constant in V8 object
/// Subobject names are not supported
template<typename T>
void set_const(v8::Isolate* isolate, v8::Handle<v8::Object> options,
	char const* name, T const& value)
{
	options->ForceSet(v8_wrap::to_v8(isolate, name), to_v8(isolate, value),
		v8::PropertyAttribute(v8::ReadOnly | v8::DontDelete));
}

} // namespace v8_wrap

#endif // V8_OBJECT_H_
