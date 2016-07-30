/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef GAME_CLIENT_MESSAGER_H_
#define GAME_CLIENT_MESSAGER_H_

#include "Game_Player.h"

class Game_Client_Messager {
public:
	static Game_Client_Messager *instance(void);

	//处理客户端登录game消息，客户端和gate断开连接消息
	int process_client_login_block(Block_Buffer &buf);
	/// 获取角色信息
	int fetch_role_info(int gate_cid, int player_cid, MSG_120001 &msg);
	/// 创建角色
	int create_role(int gate_cid, int player_cid, MSG_120002 &msg);
	/// 玩家与gate断开连接通知game
	int gate_game_player_signout(int gate_cid, int player_cid);

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
