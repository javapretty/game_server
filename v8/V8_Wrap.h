/*
 * V8_Wrap.h
 *
 *  Created on: Feb 1, 2016
 *      Author: zhangyalei
 */

#ifndef V8_WRAP_H_
#define V8_WRAP_H_

#include "include/v8.h"

using namespace v8;

Local<Context> Create_Context(Isolate* isolate);
const char* ToCString(const String::Utf8Value& value);
int Run_Script(Isolate* isolate, const char* file_path);
MaybeLocal<String> Read_File(Isolate* isolate, const char* file_path);
void Report_Exception(Isolate* isolate, TryCatch* handler, const char* file_path = NULL);

//函数说明：引用js文件		参数：1,文件路径	返回值：无
void require(const FunctionCallbackInfo<Value>& args);
//函数说明：读取json配置文件	参数：1,文件路径	返回值：文件内容字符串对象
void read_json(const FunctionCallbackInfo<Value>& args);
//函数说明：打印参数到控制台 	参数：可变参数列表	返回值：无
void print(const FunctionCallbackInfo<Value>& args);
//函数说明：程序睡眠Time_Value(0,100)	参数：无	返回值：无
void sleep(const FunctionCallbackInfo<Value>& args);

#endif /* V8_WRAP_H_ */
