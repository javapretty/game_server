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
int Game_Client_Messager::process_login_buffer(Block_Buffer &buf) {
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
		LOG_INFO("msg_id:%d error", msg_id);
		break;
	}

	return 0;
}

int Game_Client_Messager::fetch_role_info(int gate_cid, int player_cid, MSG_120001 &msg) {
	if (GAME_MANAGER->server_status() != Game_Manager::STATUS_NORMAL) {
		LOG_INFO("server closing");
		return -1;
	}

	/// 超时验证
	bool validate = false;
	if (validate && abs(atoi(msg.timestamp.c_str()) - GAME_MANAGER->tick_time().sec()) > 120) {
		LOG_INFO("login validate timeout account:%s  time:%s", msg.account.c_str(), msg.timestamp.c_str());
		return GAME_MANAGER->close_client(gate_cid, player_cid, ERROR_LOGIN_VERIFY_FAIL);
    }

	/// 帐号还在登录/登出流程中判断
	if (GAME_MANAGER->logining_map().count(msg.account) || GAME_MANAGER->saving_map().count(msg.role_id)) {
		LOG_INFO("account has in logining status, account = %s.", msg.account.c_str());
		return GAME_MANAGER->close_client(gate_cid, player_cid, ERROR_DISCONNECT_RELOGIN);
	}

	/// 重复登录判断
	Game_Player *player = GAME_MANAGER->find_role_id_game_player(msg.role_id);
	if (! player) {
		if (GAME_MANAGER->db_cache()->account_player_cache_map.count(msg.account)) {
			//登录加载玩家信息，玩家存在，直接从数据库取数据
			LOG_DEBUG("push_load_account_info, role exist, account=%s",msg.account.c_str());
			if (! GAME_MANAGER->logining_map().insert(std::make_pair(msg.account, Cid_Info(gate_cid, player_cid))).second) {
				LOG_INFO("insert logining_map failure");
				return -1;
			}
			Block_Buffer msg_buf;
			msg_buf.make_inner_message(SYNC_GAME_DB_LOAD_PLAYER_INFO);
			MSG_150001 db_msg;
			db_msg.account_info.account = msg.account;
			db_msg.account_info.agent_num = msg.agent_num;
			db_msg.account_info.server_num = msg.server_num;
			db_msg.serialize(msg_buf);
			msg_buf.finish_message();
			GAME_MANAGER->send_to_db(msg_buf);
		} else {
			//登录加载玩家信息，玩家不存在，返回客户端创建玩家
			LOG_DEBUG("push_load_account_info, role not exist, create role, account=%s",msg.account.c_str());
			Block_Buffer res_buf;
			res_buf.make_player_message(RES_FETCH_ROLE_INFO, ERROR_ROLE_NOT_EXIST, player_cid);
			MSG_520001 res_msg;
			res_msg.serialize(res_buf);
			res_buf.finish_message();
			GAME_MANAGER->send_to_gate(gate_cid, res_buf);
		}

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
	if (msg.account.empty() || msg.role_name.empty() || msg.gender > 1) {
		LOG_INFO("invalid parameter account = [%s], role_name = [%s], gender = %d", msg.account.c_str(), msg.role_name.c_str(), msg.gender);
		return -1;
	}

	if (GAME_MANAGER->logining_map().count(msg.account)) {
		LOG_INFO("account has in logining status, account = %s.", msg.account.c_str());
		return GAME_MANAGER->close_client(gate_cid, player_cid, ERROR_DISCONNECT_RELOGIN);
	}

	if (! GAME_MANAGER->logining_map().insert(std::make_pair(msg.account, Cid_Info(gate_cid, player_cid))).second) {
		LOG_INFO("invalid parameter account = [%s], role_name = [%s], gender = %d", msg.account.c_str(), msg.role_name.c_str(), msg.gender);
	}

	LOG_DEBUG("process_120002, push_create_player_data,account=%s, role_name=%s",msg.account.c_str(), msg.role_name.c_str());
	Block_Buffer msg_buf;
	msg_buf.make_inner_message(SYNC_GAME_DB_CREATE_PLAYER);
	MSG_150002 db_msg;
	db_msg.player_info.agent_num = msg.agent_num;
	db_msg.player_info.server_num = msg.server_num;
	db_msg.player_info.account = msg.account;
	db_msg.player_info.role_name = msg.role_name;
	db_msg.player_info.gender = msg.gender;
	db_msg.player_info.level = 1;
	db_msg.player_info.create_time = Time_Value::gettimeofday().sec();
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
