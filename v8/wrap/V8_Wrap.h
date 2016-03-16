/*
 * V8_Wrap.h
 *
 *  Created on: 2016年2月1日
 *      Author: zhangyalei
 */

#ifndef V8_WRAP_H_
#define V8_WRAP_H_

#include "include/v8.h"
#include "include/libplatform/libplatform.h"

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

//函数说明：发送消息到db_server 	参数：1,buffer对象	返回值：无
void send_msg_to_db(const FunctionCallbackInfo<Value>& args);
//函数说明：处理登录消息 	参数：1,buffer对象 2,gate_cid 3,player_cid 4,msg_id	返回值：无
void process_login_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取客户端消息buffer	参数：无	返回值：buffer对象
void get_client_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：回收客户端消息buffer	参数：buffer对象	返回值：无
void push_client_buffer(const FunctionCallbackInfo<Value>& args);
//函数说明：获取登录玩家的数据buffer	参数：无	返回值：buffer对象
void get_load_player_data(const FunctionCallbackInfo<Value>& args);
//函数说明：获取下线玩家的cid		参数：无	返回值：cid值
void get_drop_player_cid(const FunctionCallbackInfo<Value>& args);
//函数说明：从c++层获取超时定时器
void get_timeout_timer(const FunctionCallbackInfo<Value>& args);
//函数说明：注册定时器到c++层
void register_timer_in(const FunctionCallbackInfo<Value>& args);

#endif /* V8_WRAP_H_ */
