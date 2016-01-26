/*
 * myclass_tempate.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef MYCLASS_TEMPLATE_H_
#define MYCLASS_TEMPLATE_H_

#include "include/libplatform/libplatform.h"
#include "include/v8.h"

v8::Local<v8::Context> CreateShellContext(v8::Isolate* isolate);
void UnregisterAll();

#endif /* MYCLASS_TEMPLATE_H_ */
