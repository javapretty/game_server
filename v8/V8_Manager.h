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

using namespace v8;
using namespace v8_wrap;

class V8_Manager: public Thread {
public:
	typedef Block_List<Thread_Mutex> Data_List;

	static V8_Manager *instance(void);
	void run_handler(void);
	int start_v8(void);

	int push_game_gate_data(Block_Buffer *buf);

	int process_list();
	int process_script(int msg_id, Block_Buffer &buf, Game_Player *player);

	int wrap_block(void);

private:
	V8_Manager(void);
	virtual ~V8_Manager(void);
	V8_Manager(const V8_Manager &);
	const V8_Manager &operator=(const V8_Manager &);

private:
	static V8_Manager *instance_;
	context *context_;

	Data_List game_gate_data_list_;					///gate-->game
};

#define V8_MANAGER V8_Manager::instance()

inline int V8_Manager::push_game_gate_data(Block_Buffer *buf) {
	game_gate_data_list_.push_back(buf);
	return 0;
}

#endif /* V8_MANAGER_H_ */
