/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef MASTER_INNER_MESSAGER_H_
#define MASTER_INNER_MESSAGER_H_

#include <Msg_Define.h>
#include "Public_Struct.h"

class Block_Buffer;

class Master_Inner_Messager {
public:
	static Master_Inner_Messager *instance(void);

	//game message
	int process_game_block(Block_Buffer &buf);

	/// 逻辑服自身回环消息处理
	int process_self_loop_block(Block_Buffer &buf);

	int process_160001(int game_cid, int player_cid, Block_Buffer &buf);
	int process_160002(Block_Buffer &buf);

	/// 配制文件热更消息
	int process_410001(Block_Buffer &buf);

private:
	Master_Inner_Messager(void);
	virtual ~Master_Inner_Messager(void);
	Master_Inner_Messager(const Master_Inner_Messager &);
	const Master_Inner_Messager &operator=(const Master_Inner_Messager &);

private:
	static Master_Inner_Messager *instance_;
};

#define MASTER_INNER_MESSAGER Master_Inner_Messager::instance()

#endif /* MASTER_INNER_MESSAGER_H_ */
