/*
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef MYCLASS_WRAP_H_
#define MYCLASS_WRAP_H_

#include "include/libplatform/libplatform.h"
#include "include/v8.h"
#include "V8_Wrap.h"
#include "Object_Wrap.h"

using namespace v8;

void RegisterMyClass(Isolate* isolate, Local<ObjectTemplate> global);
void UnregisterMyClass(void);

#endif /* MYCLASS_WRAP_H_ */
