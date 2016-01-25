/*
 * My_Class_Tempate.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef MY_CLASS_TEMPLATE_H_
#define MY_CLASS_TEMPLATE_H_

#include "My_Class.h"
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

v8::Local<v8::Context> CreateShellContext(v8::Isolate* isolate);
void UnregisterAll();

#endif /* MY_CLASS_TEMPLATE_H_ */
