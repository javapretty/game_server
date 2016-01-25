/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#include "V8_Convert.h"
#include <list>
#include <vector>
#include <map>
#include "test.h"

template<typename T>
void test_conv(v8::Isolate* isolate, T value)
{
	v8::Local<v8::Value> v8_value = v8_wrap::to_v8(isolate, value);
	auto const value2 = v8_wrap::from_v8<T>(isolate, v8_value);
	check_eq(typeid(T).name(), value2, value);
}

template<typename Char, size_t N>
void test_string_conv(v8::Isolate* isolate, Char const (&str)[N])
{
	std::basic_string<Char> const str2(str, 2);

	test_conv(isolate, str[0]);
	test_conv(isolate, str);

	check_eq("string literal",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, str)), str);
	check_eq("string literal2",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, str, 2)), str2);

	Char const* ptr = str;
	check_eq("string pointer",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, ptr)), str);
	check_eq("string pointer2",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, ptr, 2)), str2);

	Char const* empty = str + N - 1; // use last \0 in source string
	check_eq("empty string literal",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, "")), empty);
	check_eq("empty string literal0",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, "", 0)), empty);

	check_eq("empty string pointer",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, empty)), empty);
	check_eq("empty string pointer0",
		v8_wrap::from_v8<Char const*>(isolate, v8_wrap::to_v8(isolate, empty, 0)), empty);
}

void test_convert()
{
	v8_wrap::context context;
	v8::Isolate* isolate = context.isolate();
	v8::HandleScope scope(isolate);

	test_conv(isolate, 1);
	test_conv(isolate, 2.2);
	test_conv(isolate, true);

	test_string_conv(isolate, "qaz");

	using int_vector = std::vector<int>;

	int_vector vector = { 1, 2, 3 };
	test_conv(isolate, vector);

	std::map<char, int> map = { { 'a', 1 }, { 'b', 2 }, { 'c', 3 } };
	test_conv(isolate, map);

	std::array<int, 3> array = { 1, 2, 3 };
	test_conv(isolate, array);

	check_ex<std::runtime_error>("wrong array length", [isolate, array]()
	{
		v8::Local<v8::Array> arr = v8_wrap::to_v8(isolate, std::array<int, 3>{ 1, 2, 3 });
		v8_wrap::from_v8<std::array<int, 2>>(isolate, arr);
	});

	check_eq("initializer list to array",
		v8_wrap::from_v8<int_vector>(isolate, v8_wrap::to_v8(isolate, { 1, 2, 3 })), vector);

	std::list<int> list = { 1, 2, 3 };
	check_eq("pair of iterators to array",
		v8_wrap::from_v8<int_vector>(isolate, v8_wrap::to_v8(isolate, list.begin(), list.end())), vector);
}
