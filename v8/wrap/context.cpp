/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#include <fstream>
#include "context.h"
#include "convert.h"
#include "function.h"
#include "module.h"
#include "throw_ex.h"


#define STRINGIZE(s) STRINGIZE0(s)
#define STRINGIZE0(s) #s

namespace v8pp {

void context::run_file(v8::FunctionCallbackInfo<v8::Value> const& args)
{
	v8::Isolate* isolate = args.GetIsolate();

	v8::EscapableHandleScope scope(isolate);
	v8::Local<v8::Value> result;
	try
	{
		std::string const filename = from_v8<std::string>(isolate, args[0], "");
		if (filename.empty())
		{
			throw std::runtime_error("run_file: require filename string argument");
		}

		context* ctx = detail::get_external_data<context*>(args.Data());
		result = to_v8(isolate, ctx->run_file(filename));
	}
	catch (std::exception const& ex)
	{
		result = throw_ex(isolate, ex.what());
	}
	args.GetReturnValue().Set(scope.Escape(result));
}

struct array_buffer_allocator : v8::ArrayBuffer::Allocator
{
	void* Allocate(size_t length)
	{
		return calloc(length, 1);
	}
	void* AllocateUninitialized(size_t length)
	{
		return malloc(length);
	}
	void Free(void* data, size_t length)
	{
		free(data);
	}
};
static array_buffer_allocator array_buffer_allocator_;

context::context(v8::Isolate* isolate)
{
	own_isolate_ = (isolate == nullptr);
	if (own_isolate_)
	{
		v8::Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = &array_buffer_allocator_;

		isolate = v8::Isolate::New(create_params);
		isolate->Enter();
	}
	isolate_ = isolate;

	v8::HandleScope scope(isolate_);

	v8::Handle<v8::Value> data = detail::set_external_data(isolate_, this);
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate_);

	global->Set(isolate_, "run", v8::FunctionTemplate::New(isolate_, context::run_file, data));

	v8::Handle<v8::Context> impl = v8::Context::New(isolate_, nullptr, global);
	impl->Enter();
	impl_.Reset(isolate_, impl);
}

context::~context()
{
	v8::Local<v8::Context> impl = to_local(isolate_, impl_);
	impl->Exit();

	impl_.Reset();
	if (own_isolate_)
	{
		isolate_->Exit();
		isolate_->Dispose();
	}
}

context& context::set(char const* name, v8::Handle<v8::Value> value)
{
	v8::HandleScope scope(isolate_);
	to_local(isolate_, impl_)->Global()->Set(to_v8(isolate_, name), value);
	return *this;
}

context& context::set(char const* name, module& m)
{
	return set(name, m.new_instance());
}

v8::Handle<v8::Value> context::run_file(std::string const& filename)
{
	std::ifstream stream(filename.c_str());
	if (!stream)
	{
		throw std::runtime_error("could not locate file " + filename);
	}

	std::istreambuf_iterator<char> begin(stream), end;
	return run_script(std::string(begin, end), filename);
}

v8::Handle<v8::Value> context::run_script(std::string const& source, std::string const& filename)
{
	v8::EscapableHandleScope scope(isolate_);

	v8::Local<v8::Script> script = v8::Script::Compile(
		to_v8(isolate_, source), to_v8(isolate_, filename));

	v8::Local<v8::Value> result;
	if (!script.IsEmpty())
	{
		result = script->Run();
	}
	return scope.Escape(result);
}

} // namespace v8pp
