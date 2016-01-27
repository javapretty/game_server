/*
 * V8_Manager.h
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#ifndef V8_MANAGER_H_
#define V8_MANAGER_H_

#include "include/v8.h"
#include "V8_Context.h"
#include "Thread.h"
#include "Public_Struct.h"
#include "Block_Buffer.h"
#include "List.h"
#include "Block_List.h"
#include	"Msg_Define.h"
#include "Game_Player.h"
#include "Game_Server.h"

using namespace v8;
using namespace v8_wrap;

class V8_Manager: public Thread {
public:
	typedef Block_List<Thread_Mutex> Data_List;

	static V8_Manager *instance(void);
	void run_handler(void);
	int start_v8(void);

	int push_data_block(Block_Buffer *buf);
	Block_Buffer& pop_data_block(void);

	int process_list();
	int process_script(int msg_id, Block_Buffer &buf, Game_Player *player);

	int wrap_block_buffer(void);

private:
	V8_Manager(void);
	virtual ~V8_Manager(void);
	V8_Manager(const V8_Manager &);
	const V8_Manager &operator=(const V8_Manager &);

private:
	static V8_Manager *instance_;
	context *context_;

	Data_List data_list_;					///gate-->game
};

#define V8_MANAGER V8_Manager::instance()

inline int V8_Manager::push_data_block(Block_Buffer *buf) {
	data_list_.push_back(buf);
	return 0;
}

inline Block_Buffer& V8_Manager::pop_data_block() {
	Block_Buffer *buf = data_list_.pop_front();
	int32_t cid = buf->peek_int32();
	GAME_GATE_SERVER->push_block(cid, buf);
	return *buf;
}

#endif /* V8_MANAGER_H_ */
