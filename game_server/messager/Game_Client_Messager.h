/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_CLIENT_MESSAGER_H_
#define GAME_CLIENT_MESSAGER_H_

#include "Msg_Define.h"
#include "Block_Buffer.h"
#include "Game_Player.h"

class Game_Client_Messager {
public:
	static Game_Client_Messager *instance(void);

	/// 获取角色信息
	int process_120001(int gate_cid, int player_cid, MSG_120001 &msg);
	/// 创建角色
	int process_120002(int gate_cid, int player_cid, MSG_120002 &msg);
	/// 玩家与gate断开连接通知game
	int process_140002(int gate_cid, int player_cid);

private:
	Game_Client_Messager(void);
	virtual ~Game_Client_Messager(void);
	Game_Client_Messager(const Game_Client_Messager &);
	const Game_Client_Messager &operator=(const Game_Client_Messager &);

private:
	static Game_Client_Messager *instance_;
};

#define GAME_CLIENT_MESSAGER Game_Client_Messager::instance()

#endif /* GAME_CLIENT_MESSAGER_H_ */
