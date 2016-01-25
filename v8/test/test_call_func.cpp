/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#include "V8_Call_Func.h"
#include "V8_Context.h"
#include "V8_Function.h"
#include "test.h"

static int x() { return 0; }
static int y(int a) { return a; }
static int z(v8::Isolate*, int a) { return a; }

static void w(v8::FunctionCallbackInfo<v8::Value> const& args)
{
	return args.GetReturnValue().Set(args.Length());
}

static_assert(v8_wrap::v8_detail::select_call_traits<decltype(&x)>::arg_count == 0, "x argc count");
static_assert(std::is_same<v8_wrap::v8_detail::select_call_traits<decltype(&x)>::arg_type<0>,
	void>::value, "x has no args");

static_assert(v8_wrap::v8_detail::select_call_traits<decltype(&y)>::arg_count == 1, "y argc count");
static_assert(std::is_same<v8_wrap::v8_detail::select_call_traits<decltype(&y)>::arg_type<0>,
	int>::value, "y 1st arg");

static_assert(v8_wrap::v8_detail::select_call_traits<decltype(&z)>::arg_count == 1, "z argc count");
static_assert(std::is_same<v8_wrap::v8_detail::select_call_traits<decltype(&z)>::arg_type<0>,
	v8::Isolate*>::value, "z 1st arg");
static_assert(std::is_same<v8_wrap::v8_detail::select_call_traits<decltype(&z)>::arg_type<1>,
	int>::value, "z 2nd arg");

static_assert(std::is_same<v8_wrap::v8_detail::select_call_traits<decltype(&x)>,
	v8_wrap::v8_detail::call_from_v8_traits<decltype(&x) >> ::value, "");

static_assert(std::is_same<v8_wrap::v8_detail::select_call_traits<decltype(&z)>,
	v8_wrap::v8_detail::isolate_arg_call_traits<decltype(&z) >> ::value, "");

static_assert(std::is_same<v8_wrap::v8_detail::select_call_traits<decltype(&w)>,
	v8_wrap::v8_detail::v8_args_call_traits<decltype(&w) >> ::value, "");

void test_call_func()
{
	v8_wrap::context context;
	v8::Isolate* isolate = context.isolate();
	v8::HandleScope scope(isolate);

	context.set("x", v8_wrap::wrap_function(isolate, "x", &x));
	context.set("y", v8_wrap::wrap_function(isolate, "y", &y));
	context.set("z", v8_wrap::wrap_function(isolate, "z", &z));
	context.set("w", v8_wrap::wrap_function(isolate, "w", &w));

	check_eq("x", run_script<int>(context, "x()"), 0);
	check_eq("y", run_script<int>(context, "y(1)"), 1);
	check_eq("z", run_script<int>(context, "z(2)"), 2);
	check_eq("w", run_script<int>(context, "w(2, 'd', true, null)"), 4);
}
