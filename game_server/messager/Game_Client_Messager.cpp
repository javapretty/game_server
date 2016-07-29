/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Game_Client_Messager.h"
#include "Game_Manager.h"

Game_Client_Messager::Game_Client_Messager(void) { }

Game_Client_Messager::~Game_Client_Messager(void) { }

Game_Client_Messager *Game_Client_Messager::instance_;

Game_Client_Messager *Game_Client_Messager::instance(void) {
	if (! instance_)
		instance_ = new Game_Client_Messager;
	return instance_;
}

//处理登录掉线消息
int Game_Client_Messager::process_client_login_block(Block_Buffer &buf) {
	int32_t gate_cid = buf.read_int32();
	/*int16_t len*/  buf.read_int16();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/  buf.read_int32();
	int32_t player_cid = buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	int ret = 0;
	switch (msg_id) {
	case REQ_FETCH_ROLE_INFO: {
		MSG_120001 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			fetch_role_info(gate_cid, player_cid, msg);
		break;
	}
	case REQ_CREATE_ROLE: {
		MSG_120002 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			create_role(gate_cid, player_cid, msg);
		break;
	}
	case SYNC_GATE_GAME_PLAYER_SIGNOUT: {
		ret = gate_game_player_signout(gate_cid, player_cid);
		break;
	}
	default:
		LOG_ERROR("msg_id:%d error", msg_id);
		break;
	}

	return 0;
}

int Game_Client_Messager::fetch_role_info(int gate_cid, int player_cid, MSG_120001 &msg) {
	if (GAME_MANAGER->server_status() != Game_Manager::STATUS_NORMAL) {
		LOG_INFO("server status closing");
		return -1;
	}

	/// 帐号还在登录/登出流程中判断
	if (GAME_MANAGER->logining_map().count(msg.account) || GAME_MANAGER->saving_map().count(msg.role_id)) {
		LOG_INFO("account has in logining status, account = %s.", msg.account.c_str());
		return GAME_MANAGER->close_client(gate_cid, player_cid, ERROR_DISCONNECT_RELOGIN);
	}

	/// 重复登录判断
	Game_Player *player = GAME_MANAGER->find_role_id_game_player(msg.role_id);
	if (! player) {
		//登录加载玩家信息，玩家存在，直接从数据库取数据
		LOG_DEBUG("fetch_role_info, load player info from db, account=%s",msg.account.c_str());
		if (! GAME_MANAGER->logining_map().insert(std::make_pair(msg.account, Cid_Info(gate_cid, player_cid))).second) {
			LOG_INFO("insert logining_map failure");
			return -1;
		}
		Block_Buffer msg_buf;
		msg_buf.make_inner_message(SYNC_GAME_DB_LOAD_PLAYER);
		MSG_150001 db_msg;
		db_msg.account = msg.account;
		db_msg.client_ip = msg.client_ip;
		db_msg.serialize(msg_buf);
		msg_buf.finish_message();
		GAME_MANAGER->send_to_db(msg_buf);
	} else {
		/// 回收中处理
		if (player->recycle_status() == Recycle_Tick::RECYCLE) {
			return GAME_MANAGER->close_client(gate_cid, player_cid, ERROR_DISCONNECT_RECOVERING);
		}

		/// 重复登录
		if (player_cid != player->player_cid()) {
			player->set_cid(gate_cid, player_cid);
		} else {
			LOG_INFO("player login again, account=%s", msg.account.c_str());
		}
	}
	return 0;
}

int Game_Client_Messager::create_role(int gate_cid, int player_cid, MSG_120002 &msg) {
	if (msg.role_info.account.empty() || msg.role_info.role_name.empty()) {
		LOG_INFO("invalid parameter account = [%s], role_name = [%s]", msg.role_info.account.c_str(), msg.role_info.role_name.c_str());
		return -1;
	}

	if (GAME_MANAGER->logining_map().count(msg.role_info.account)) {
		LOG_INFO("account has in logining status, account = %s.", msg.role_info.account.c_str());
		return GAME_MANAGER->close_client(gate_cid, player_cid, ERROR_DISCONNECT_RELOGIN);
	}

	if (! GAME_MANAGER->logining_map().insert(std::make_pair(msg.role_info.account, Cid_Info(gate_cid, player_cid))).second) {
		LOG_INFO("invalid parameter account = [%s], role_name = [%s]", msg.role_info.account.c_str(), msg.role_info.role_name.c_str());
	}

	LOG_DEBUG("process_120002, push_create_player_data,account=%s, role_name=%s",msg.role_info.account.c_str(), msg.role_info.role_name.c_str());
	Block_Buffer msg_buf;
	msg_buf.make_inner_message(SYNC_GAME_DB_CREATE_PLAYER);
	MSG_150002 db_msg;
	db_msg.role_info = msg.role_info;
	db_msg.serialize(msg_buf);
	msg_buf.finish_message();
	GAME_MANAGER->send_to_db(msg_buf);
	return 0;
}

int Game_Client_Messager::gate_game_player_signout(int gate_cid, int player_cid) {
	int cid = gate_cid * 10000 + player_cid;
	Game_Player *player = GAME_MANAGER->find_cid_game_player(cid);
	if (player) {
		player->link_close();
	}
	return 0;
}
