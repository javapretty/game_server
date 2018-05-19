/*
 * Public_Define.h
 *
 *  Created on: Aug 6,2016
 *      Author: zhangyalei
 */

#ifndef PUBLIC_DEFINE_H_
#define PUBLIC_DEFINE_H_

#include <vector>
#include "Time_Value.h"

const static int SVC_MAX_LIST_SIZE = 1000;
const static int SVC_MAX_PACK_SIZE = 60 * 1024;

enum NetWork_Protocol {
	TCP = 0,
	UDP = 1,
	WEBSOCKET = 2,
	HTTP = 3,
};

enum Event_Type {
	EVENT_INPUT 		= 0x1,
	EVENT_OUTPUT 		= 0x2,
	EVENT_TIMEOUT 		= 0x4,
	EVENT_ONCE_IO_IN 	= 0x8,		/// 一次性IO输入事件
	EVENT_ONCE_IO_OUT 	= 0x10,		/// 一次性IO输出事件
	EVENT_ONCE_TIMER 	= 0x20,		/// 一次性定时器事件
	WITH_IO_HEARTBEAT 	= 0x40,		/// IO附带心跳机制
};

enum Log_Type {
	LOG_TRACE = 0,				//打印程序运行堆栈，跟踪记录数据信息，与DEBUG相比更细致化的记录信息
	LOG_DEBUG = 1,				//细粒度信息事件对调试应用程序是非常有帮助的
	LOG_INFO = 2,					//消息在粗粒度级别上突出强调应用程序的运行过程
	LOG_WARN = 3,					//会出现潜在错误的情形
	LOG_ERROR = 4,				//虽然发生错误事件，但仍然不影响系统的继续运行
	LOG_FATAL = 5,				//严重的错误事件，将会导致应用程序的退出
};

enum Color {
	BLACK = 30,
	RED = 31,
	GREEN = 32,
	BROWN = 33,
	BLUE = 34,
	MAGENTA = 35,
	CYAN = 36,
	GREY = 37,
	LRED = 41,
	LGREEN = 42,
	YELLOW = 43,
	LBLUE = 44,
	LMAGENTA = 45,
	LCYAN = 46,
	WHITE = 47
};

enum Websocket_Frame {
	FRAME_NORMAL = 0x0,
	FRAME_FINAL = 0x1,
};

enum Websocket_Opcode {
	OPCODE_CONTINUATION = 0x0,
	OPCODE_TEXT = 0x1,
	OPCODE_BINARY = 0x2,
	OPCODE_CLOSE = 0x8,
	OPCODE_PING = 0x9,
	OPCODE_PONG = 0xa,
};

struct Data_Info {
	int file_size;
	int data_len;
	char* data;
} ;

class Block_Buffer;
struct Block_Group_Info {
	int free_list_size_;
	int used_list_size_;
	int sum_bytes_;

	void serialize(Block_Buffer &buf);
	void deserialize(Block_Buffer &buf);
	void reset(void);
};

struct Server_Info {
	int svc_pool_free_list_size_;
	int svc_pool_used_list_size_;
	int svc_list_size_;
	std::vector<Block_Group_Info> block_group_info_;

	void serialize(Block_Buffer &buf);
	void deserialize(Block_Buffer &buf);
	void reset(void);
};

#endif /* PUBLIC_DEFINE_H_ */
