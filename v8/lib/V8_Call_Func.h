/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#ifndef V8_CALL_FUNC_H_
#define V8_CALL_FUNC_H_

#include <functional>
#include "include/v8.h"
#include "V8_Convert.h"
#include "V8_Utility.h"

namespace v8_wrap {

/// Call a V8 function, converting C++ arguments to v8::Value arguments
/// @param isolate V8 isolate instance
/// @param func  V8 function to call
/// @param recv V8 object used as `this` in the function
/// @param args...  C++ arguments to convert to JS arguments using to_v8
template<typename ...Args>
v8::Handle<v8::Value> call_v8(v8::Isolate* isolate, v8::Handle<v8::Function> func,
	v8::Handle<v8::Value> recv, Args... args)
{
	v8::EscapableHandleScope scope(isolate);

	int const arg_count = sizeof...(Args);
	// +1 to allocate array for arg_count == 0
	v8::Handle<v8::Value> v8_args[arg_count + 1] = { to_v8(isolate, args)... };

	v8::Local<v8::Value> result = func->Call(recv, arg_count, v8_args);

	return scope.Escape(result);
}

namespace v8_detail {

template<typename F, size_t Offset = 0>
struct call_from_v8_traits
{
	static bool const is_mem_fun = std::is_member_function_pointer<F>::value;
	using arguments = typename function_traits<F>::arguments;

	static size_t const arg_count = std::tuple_size<arguments>::value - is_mem_fun - Offset;

	template<size_t Index, bool>
	struct tuple_element
	{
		using type = typename std::tuple_element<Index, arguments>::type;
	};

	template<size_t Index>
	struct tuple_element<Index, false>
	{
		using type = void;
	};

	template<size_t Index>
	using arg_type = typename tuple_element<Index + is_mem_fun,
		Index < (arg_count + Offset)>::type;

	template<size_t Index>
	using convert_type = typename convert<arg_type<Index>>::from_type;

	template<size_t Index>
	static convert_type<Index>
	arg_from_v8(v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		return convert<arg_type<Index>>::from_v8(args.GetIsolate(), args[Index - Offset]);
	}

	static void check(v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		if (args.Length() != arg_count)
		{
			throw std::runtime_error("argument count does not match function definition");
		}
	}
};

template<typename F>
using isolate_arg_call_traits = call_from_v8_traits<F, 1>;

template<typename F, size_t Offset = 0>
struct v8_args_call_traits : call_from_v8_traits<F, Offset>
{
	template<size_t Index>
	using arg_type = v8::FunctionCallbackInfo<v8::Value> const&;

	template<size_t Index>
	using convert_type = v8::FunctionCallbackInfo<v8::Value> const&;

	template<size_t Index>
	static v8::FunctionCallbackInfo<v8::Value> const&
	arg_from_v8(v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		return args;
	}

	static void check(v8::FunctionCallbackInfo<v8::Value> const&)
	{
	}
};

template<typename F>
using isolate_v8_args_call_traits = v8_args_call_traits<F, 1>;

template<typename F, size_t Offset>
using is_direct_args = std::integral_constant<bool,
	call_from_v8_traits<F>::arg_count == (Offset + 1) &&
	std::is_same<typename call_from_v8_traits<F>::template arg_type<Offset>,
		v8::FunctionCallbackInfo<v8::Value> const&>::value>;

template<typename F>
using is_first_arg_isolate = std::integral_constant<bool,
	call_from_v8_traits<F>::arg_count != 0 &&
	std::is_same<typename call_from_v8_traits<F>::template arg_type<0>,
		v8::Isolate*>::value>;

template<typename F>
using select_call_traits = typename std::conditional<is_first_arg_isolate<F>::value,
	typename std::conditional<is_direct_args<F, 1>::value,
		isolate_v8_args_call_traits<F>, isolate_arg_call_traits<F>>::type,
	typename std::conditional<is_direct_args<F, 0>::value,
		v8_args_call_traits<F>, call_from_v8_traits<F>>::type
>::type;

template<typename F, typename CallTraits, size_t ...Indices>
typename function_traits<F>::return_type
call_from_v8_impl(F&& func, v8::FunctionCallbackInfo<v8::Value> const& args,
	CallTraits, index_sequence<Indices...>)
{
	return func(CallTraits::template arg_from_v8<Indices>(args)...);
}

template<typename T, typename F, typename CallTraits, size_t ...Indices>
typename function_traits<F>::return_type
call_from_v8_impl(T& obj, F&& func, v8::FunctionCallbackInfo<v8::Value> const& args,
	CallTraits, index_sequence<Indices...>)
{
	return (obj.*func)(CallTraits::template arg_from_v8<Indices>(args)...);
}

template<typename F, size_t ...Indices>
typename function_traits<F>::return_type
call_from_v8_impl(F&& func, v8::FunctionCallbackInfo<v8::Value> const& args,
	isolate_arg_call_traits<F>, index_sequence<Indices...>)
{
	return func(args.GetIsolate(), isolate_arg_call_traits<F>::template arg_from_v8<Indices + 1>(args)...);
}

template<typename T, typename F, size_t ...Indices>
typename function_traits<F>::return_type
call_from_v8_impl(T& obj, F&& func, v8::FunctionCallbackInfo<v8::Value> const& args,
	isolate_arg_call_traits<F>, index_sequence<Indices...>)
{
	return (obj.*func)(args.GetIsolate(), isolate_arg_call_traits<F>::template arg_from_v8<Indices + 1>(args)...);
}

template<typename F, size_t ...Indices>
typename function_traits<F>::return_type
call_from_v8_impl(F&& func, v8::FunctionCallbackInfo<v8::Value> const& args,
	isolate_v8_args_call_traits<F>, index_sequence<Indices...>)
{
	return func(args.GetIsolate(), args);
}

template<typename T, typename F, size_t ...Indices>
typename function_traits<F>::return_type
call_from_v8_impl(T& obj, F&& func, v8::FunctionCallbackInfo<v8::Value> const& args,
	isolate_v8_args_call_traits<F>, index_sequence<Indices...>)
{
	return (obj.*func)(args.GetIsolate(), args);
}

template<typename F>
typename function_traits<F>::return_type
call_from_v8(F&& func, v8::FunctionCallbackInfo<v8::Value> const& args)
{
	using call_traits = select_call_traits<F>;
	call_traits::check(args);
	return call_from_v8_impl(std::forward<F>(func), args,
		call_traits(), make_index_sequence<call_traits::arg_count>());
}

template<typename T, typename F>
typename function_traits<F>::return_type
call_from_v8(T& obj, F&& func, v8::FunctionCallbackInfo<v8::Value> const& args)
{
	using call_traits = select_call_traits<F>;
	call_traits::check(args);
	return call_from_v8_impl(obj, std::forward<F>(func), args,
		call_traits(), make_index_sequence<call_traits::arg_count>());
}

}} //namespace v8_wrap::v8_detail

#endif // V8_CALL_FUNC_H_
