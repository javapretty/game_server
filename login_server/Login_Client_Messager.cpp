/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Login_Client_Messager.h"
#include "Login_Manager.h"

Login_Client_Messager::Login_Client_Messager(void) { }

Login_Client_Messager::~Login_Client_Messager(void) { }

Login_Client_Messager *Login_Client_Messager::instance_;

Login_Client_Messager *Login_Client_Messager::instance(void) {
	if (! instance_)
		instance_ = new Login_Client_Messager;
	return instance_;
}

int Login_Client_Messager::process_block(Block_Buffer &buf) {
	int32_t cid = 0;
	int16_t len = 0;
	int32_t msg_id = 0;
	int32_t status = 0;
	int32_t serial_cipher = 0;
	int32_t msg_time_cipher = 0;
	buf.read_int32(cid);
	buf.read_int16(len);
	buf.read_int32(msg_id);
	buf.read_int32(status);
	buf.read_int32(serial_cipher);
	buf.read_int32(msg_time_cipher);

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case REQ_CONNECT_LOGIN:{
			connect_login(cid, msg_id, buf);
			break;
	}
	default:	{
		LOG_ERROR("error msg_id:%d", msg_id);
		break;
	}
	}

	return 0;
}

int Login_Client_Messager::connect_login(int cid, int msg_id, Block_Buffer &buf){
	MSG_100001 msg;
	msg.deserialize(buf);
	Login_Player *player = LOGIN_MANAGER->find_account_login_player(msg.account);
	if (!player) {
		int status = LOGIN_MANAGER->client_login(msg.account, msg.password);
		if (status < 0) {
			LOG_ERROR("connect login fail, account:%s, password:%d, status:%d", msg.account.c_str(), msg.password.c_str(), status);
			Block_Buffer res_buf;
			res_buf.make_server_message(RES_CONNECT_LOGIN, ERROR_LOGIN_FAIL);
			res_buf.finish_message();
			LOGIN_MANAGER->send_to_client(cid, res_buf);
			LOGIN_MANAGER->close_client(cid);
			return -1;
		}

		//success
		MSG_500001 res_msg;
		res_msg.reset();
		LOGIN_MANAGER->get_gate_ip(msg.account, res_msg.gate_ip, res_msg.gate_port);
		make_session(msg.account, res_msg.session);

		player = LOGIN_MANAGER->pop_login_player();
		if (!player) {
			LOG_ERROR("login_player_pool_ pop error");
			return -1;
		}

		player->reset();
		player->set_player_cid(cid);
		Login_Player_Info player_info;
		player_info.session_tick = Time_Value::gettimeofday().sec();
		player_info.account = msg.account;
		player_info.gate_ip = res_msg.gate_ip;
		player_info.gate_port = res_msg.gate_port;
		player_info.session = res_msg.session;
		player->set_player_info(player_info);
		LOGIN_MANAGER->bind_cid_login_player(cid, player);
		LOGIN_MANAGER->bind_account_login_player(msg.account, player);

		Block_Buffer res_buf;
		res_buf.make_server_message(RES_CONNECT_LOGIN, 0);
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);

		LOG_INFO("connect login, account:%s, gate_ip:%s, gate_port:%d, session:%s",
				msg.account.c_str(), res_msg.gate_ip.c_str(), res_msg.gate_port, res_msg.session.c_str());
	}
	else
	{//fail
		LOG_ERROR("connect login, player have login, account:%s, password:%d", msg.account.c_str(), msg.password.c_str());
		Block_Buffer res_buf;
		res_buf.make_server_message(RES_CONNECT_LOGIN, ERROR_LOGIN_FAIL);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
		LOGIN_MANAGER->close_client(cid);
	}
	return 0;
}
