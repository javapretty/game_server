/*
 *
 *  Created on: 2016年1月23日
 *      Author: zhangyalei
 */

#include <context.h>
#include "test.h"

void test_context()
{
	v8pp::context context;

	v8::HandleScope scope(context.isolate());
	int const r = context.run_script("42")->Int32Value();
	check_eq("run_script", r, 42);
}
