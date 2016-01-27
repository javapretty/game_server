/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#include <dlfcn.h>
#include <fstream>
#include "V8_Context.h"
#include "V8_Convert.h"
#include "V8_Exception.h"
#include "V8_Function.h"
#include "V8_Module.h"

namespace v8_wrap {

static char const path_sep = '/';

struct context::dynamic_module
{
	void* handle;
	v8::UniquePersistent<v8::Value> exports;

	dynamic_module():handle(0) {}
	dynamic_module(dynamic_module&& other)
		: handle(other.handle)
		, exports(std::move(other.exports))
	{
		other.handle = nullptr;
	}

	dynamic_module(dynamic_module const&) = delete;
};

void context::load_module(v8::FunctionCallbackInfo<v8::Value> const& args)
{
	v8::Isolate* isolate = args.GetIsolate();

	v8::EscapableHandleScope scope(isolate);
	v8::Local<v8::Value> result;
	try
	{
		std::string const name = from_v8<std::string>(isolate, args[0], "");
		if (name.empty())
		{
			throw std::runtime_error("load_module: require module name string argument");
		}

		context* ctx = v8_detail::get_external_data<context*>(args.Data());
		context::dynamic_modules::iterator it = ctx->modules_.find(name);

		// check if module is already loaded
		if (it != ctx->modules_.end())
		{
			result = v8::Local<v8::Value>::New(isolate, it->second.exports);
		}
		else
		{
			std::string filename = name;
			if (!ctx->lib_path_.empty())
			{
				filename = ctx->lib_path_ + path_sep + name;
			}
			std::string const suffix = ".so";
			if (filename.size() >= suffix.size()
				&& filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) != 0)
			{
				filename += suffix;
			}

			dynamic_module module;
			module.handle = dlopen(filename.c_str(), RTLD_LAZY);
			if (!module.handle)
			{
				throw std::runtime_error("load_module(" + name + "): could not load shared library " + filename);
			}

			void *sym = dlsym(module.handle, "v8_module_init");
			if (!sym)
			{
				throw std::runtime_error("load_module(" + name + "): initialization function v8_module_init not found in " + filename);
			}

			using module_init_proc = v8::Handle<v8::Value>(*)(v8::Isolate*);
			module_init_proc init_proc = reinterpret_cast<module_init_proc>(sym);
			result = init_proc(isolate);
			module.exports.Reset(isolate, result);
			ctx->modules_.emplace(name, std::move(module));
		}
	}
	catch (std::exception const& ex)
	{
		result = throw_ex(isolate, ex.what());
	}
	args.GetReturnValue().Set(scope.Escape(result));
}

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

		context* ctx = v8_detail::get_external_data<context*>(args.Data());
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

	v8::Handle<v8::Value> data = v8_detail::set_external_data(isolate_, this);
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate_);

	global->Set(isolate_, "require", v8::FunctionTemplate::New(isolate_, context::load_module, data));
	global->Set(isolate_, "run", v8::FunctionTemplate::New(isolate_, context::run_file, data));

	v8::Handle<v8::Context> impl = v8::Context::New(isolate_, nullptr, global);
	impl->Enter();
	impl_.Reset(isolate_, impl);
}

context::~context()
{
	for (auto& kv : modules_)
	{
		dynamic_module& module = kv.second;
		module.exports.Reset();
		if (module.handle)
		{
			dlclose(module.handle);
		}
	}
	modules_.clear();

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

v8::Local<v8::Value> context::run_file(std::string const& filename)
{
	std::ifstream stream(filename.c_str());
	if (!stream)
	{
		throw std::runtime_error("could not locate file " + filename);
	}

	std::istreambuf_iterator<char> begin(stream), end;
	return run_script(std::string(begin, end), filename);
}

v8::Local<v8::Value> context::run_script(std::string const& source, std::string const& filename)
{
	v8::Local<v8::Context> impl = to_local(isolate_, impl_);
	v8::Local<v8::Script> script = v8::Script::Compile(impl, to_v8(isolate_, source)).ToLocalChecked();

	v8::Local<v8::Value> result;
	if (!script.IsEmpty())
	{
		result = script->Run(impl).ToLocalChecked();
	}
	return result;
}

} // namespace v8_wrap
