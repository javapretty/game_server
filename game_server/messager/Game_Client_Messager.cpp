/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Game_Client_Messager.h"
#include "Game_Manager.h"
#include "V8_Manager.h"

Game_Client_Messager::Game_Client_Messager(void) { }

Game_Client_Messager::~Game_Client_Messager(void) { }

Game_Client_Messager *Game_Client_Messager::instance_;

Game_Client_Messager *Game_Client_Messager::instance(void) {
	if (! instance_)
		instance_ = new Game_Client_Messager;
	return instance_;
}

int Game_Client_Messager::process_block(Block_Buffer &buf) {
	int32_t gate_cid = buf.read_int32();
	/*uint16_t len*/ buf.read_uint16();
	uint32_t msg_id = buf.read_uint32();
	/*int32_t status*/ buf.read_int32();
	int32_t player_cid = buf.read_int32();

	if (msg_id == REQ_FETCH_ROLE_INFO || msg_id == REQ_CREATE_ROLE)	 {
		return process_init_block(gate_cid, player_cid, msg_id, buf);
	}

	Cid_Info cid_info(gate_cid, 0, player_cid);
	Game_Player *player = GAME_MANAGER->find_cid_game_player(cid_info);
	if (!player) {
		MSG_DEBUG("cannot find player object. gate_cid = %d, player_cid = %d, msg_id = %d ", gate_cid, player_cid, msg_id);
		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_CLIENT_PARAM, player_cid);
		msg_buf.finish_message();
		return GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
	} else {
		if (msg_id == SYNC_GATE_GAME_PLAYER_SIGNOUT) {
			//	  玩家掉线，与gate断开连接
			return player->link_close();
		}
	}

	Perf_Mon perf_mon(msg_id);
	int ret = V8_MANAGER->process_script(msg_id, buf, player);
	if (ret) {
		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_CLIENT_PARAM, player_cid);
		msg_buf.finish_message();
		GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
	}

	return 0;
}

int Game_Client_Messager::process_init_block(int gate_cid, int player_cid, int msg_id, Block_Buffer &buf) {
	Perf_Mon perf_mon(msg_id);
	int ret = 0;
	switch (msg_id) {
	case REQ_FETCH_ROLE_INFO: {
		MSG_120001 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			process_120001(gate_cid, player_cid, msg);
		break;
	}
	case REQ_CREATE_ROLE: {
		MSG_120002 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			process_120002(gate_cid, player_cid, msg);
		break;
	}
	default:
		break;
	}
	return ret;
}

int Game_Client_Messager::process_120001(int gate_cid, int player_cid, MSG_120001 &msg) {
	if (GAME_MANAGER->server_status() != Game_Manager::STATUS_NORMAL) {
		MSG_USER("server closing");
		return -1;
	}

	/// 超时验证
	bool validate = false;
	if (validate && abs(atoi(msg.timestamp.c_str()) - GAME_MANAGER->tick_time().sec()) > 120) {
		MSG_USER("login validate timeout account:%s  time:%s", msg.account.c_str(), msg.timestamp.c_str());
		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_LOGIN_VERIFY_FAIL, player_cid);
		msg_buf.finish_message();
		return GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
    }

	/// 帐号还在登录/登出流程中判断
	if (GAME_MANAGER->logining_map().count(msg.account) || GAME_MANAGER->saving_map().count(msg.role_id)) {
		MSG_DEBUG("account has in logining status, account = %s.", msg.account.c_str());
		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_DISCONNECT_RELOGIN, player_cid);
		msg_buf.finish_message();
		return GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
	}

	/// 重复登录判断
	Game_Player *player = GAME_MANAGER->find_role_id_game_player(msg.role_id);
	if (! player) {
		if (GAME_MANAGER->db_cache()->account_player_cache_map.count(msg.account)) {
			//登录加载玩家信息，玩家存在，直接从数据库取数据
			MSG_DEBUG("push_load_account_info, role exist, account=%s",msg.account.c_str());
			if (! GAME_MANAGER->logining_map().insert(std::make_pair(msg.account, Cid_Info(gate_cid, 0, player_cid))).second) {
				MSG_USER("insert logining_map failure");
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
			MSG_DEBUG("push_load_account_info, role not exist, create role, account=%s",msg.account.c_str());
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
			Block_Buffer msg_buf;
			msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_LOGIN_PLAYER_RECOVERING, player_cid);
			msg_buf.finish_message();
			return GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
		}

		/// 重复登录
		if (player_cid != player->cid_info().player_cid) {
			Cid_Info cid_info(gate_cid, 0, player_cid);
			player->set_cid_info(cid_info);
		} else {
			MSG_USER("same cid resigin");
		}
	}
	return 0;
}

int Game_Client_Messager::process_120002(int gate_cid, int player_cid, MSG_120002 &msg) {
	if (msg.account.empty() || msg.role_name.empty() || msg.gender > 1) {
		MSG_USER("invalid parameter account = [%s], role_name = [%s], gender = %d",
				msg.account.c_str(), msg.role_name.c_str(), msg.gender);
		return -1;
	}

	if (GAME_MANAGER->logining_map().count(msg.account)) {
		MSG_DEBUG("account has in logining status, account = %s.", msg.account.c_str());
		Block_Buffer msg_buf;
		msg_buf.make_player_message(ACTIVE_DISCONNECT, ERROR_DISCONNECT_RELOGIN, player_cid);
		msg_buf.finish_message();
		return GAME_MANAGER->send_to_gate(gate_cid, msg_buf);
	}

	if (! GAME_MANAGER->logining_map().insert(std::make_pair(msg.account, Cid_Info(gate_cid, 0, player_cid))).second) {
		MSG_USER("insert failure");
	}

	MSG_DEBUG("process_120002, push_create_player_data,account=%s, role_name=%s",msg.account.c_str(), msg.role_name.c_str());
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
