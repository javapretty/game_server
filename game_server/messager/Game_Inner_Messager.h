/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef GAME_INNER_MESSAGER_H_
#define GAME_INNER_MESSAGER_H_

#include "Public_Struct.h"

class Game_Inner_Messager {
public:
	static Game_Inner_Messager *instance(void);

	/// 逻辑服自身回环消息处理
	int process_self_loop_block(Block_Buffer &buf);

private:
	Game_Inner_Messager(void);
	virtual ~Game_Inner_Messager(void);
	Game_Inner_Messager(const Game_Inner_Messager &);
	const Game_Inner_Messager &operator=(const Game_Inner_Messager &);

private:
	static Game_Inner_Messager *instance_;
};

#define GAME_INNER_MESSAGER Game_Inner_Messager::instance()

#endif /* GAME_INNER_MESSAGER_H_ */
