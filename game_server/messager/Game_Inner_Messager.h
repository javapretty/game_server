/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef GAME_INNER_MESSAGER_H_
#define GAME_INNER_MESSAGER_H_

#include "Msg_Define.h"
#include "Public_Struct.h"

class Block_Buffer;

class Game_Inner_Messager {
public:
	static Game_Inner_Messager *instance(void);

	/// 处理从数据库回来的数据
	int process_db_block(Block_Buffer &buf);

	/// master message
	int process_master_block(Block_Buffer &buf);

	/// 逻辑服自身回环消息处理
	int process_self_loop_block(Block_Buffer &buf);

	////////////////////////////db message///////////////////////
	//加载db缓存
	int process_load_db_cache(Block_Buffer &buf);
	//玩家登录时候从数据库加载信息
	int process_loaded_player_data(Block_Buffer &buf);
	/// 登录, 创建角色 成功处理
	int process_success_login(int gate_cid, int player_cid, Player_Data &data);
	/// 保存玩家信息完成
	int process_save_player_complete(Block_Buffer &buf);

	////////////////////////////inner message/////////////////////
	/// 配制文件热更消息
	int process_400001(Block_Buffer &buf);

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
