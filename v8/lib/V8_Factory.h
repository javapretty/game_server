/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#ifndef V8_FACTORY_H_
#define V8_FACTORY_H_

#include <utility>
#include "include/v8.h"

namespace v8_wrap {

// Factory that calls C++ constructor
template<typename T>
struct factory
{
	static size_t const object_size = sizeof(T);

	template<typename ...Args>
	static T* create(v8::Isolate* isolate, Args... args)
	{
		T* object = new T(std::forward<Args>(args)...);
		isolate->AdjustAmountOfExternalAllocatedMemory(static_cast<int64_t>(object_size));
		return object;
	}

	static void destroy(v8::Isolate* isolate, T* object)
	{
		delete object;
		isolate->AdjustAmountOfExternalAllocatedMemory(-static_cast<int64_t>(object_size));
	}
};

} //namespace v8_wrap

#endif // V8_FACTORY_H_
