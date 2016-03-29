/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
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
	int32_t cid = buf.read_int32();
	/*int16_t len*/ buf.read_int16();
	/*int32_t serial_cipher*/ buf.read_int32();
	/*int32_t msg_time_cipher*/ buf.read_int32();
	int32_t msg_id = buf.read_int32();
	/*int32_t status*/ buf.read_int32();

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case REQ_CLIENT_REGISTER:{
			client_register(cid, msg_id, buf);
			break;
	}
	case REQ_CLIENT_LOGIN: {
			client_login(cid, msg_id, buf);
			break;
	}
	default:	{
		LOG_INFO("error msg_id:%d", msg_id);
		break;
	}
	}

	return 0;
}

//注册
int Login_Client_Messager::client_register(int cid, int msg_id, Block_Buffer &buf){
	int ret = 0;

	MSG_100000 msg;
	msg.deserialize(buf);
	int status = LOGIN_MANAGER->client_register(msg.account, msg.password);
	if (status != 0) {
		//注册失败说明玩家已经注册，直接登录
		status = LOGIN_MANAGER->client_login(msg.account, msg.password);
	}
	if (status == 0){
		//success
		MSG_500000 res_msg;
		res_msg.reset();
		LOGIN_MANAGER->get_gate_ip(msg.account, res_msg.ip, res_msg.port);
		make_session(msg.account, res_msg.session);

		LOG_DEBUG("client register, account:%s, gate_ip:%s, gate_port:%d, session:%s",
				msg.account.c_str(), res_msg.ip.c_str(), res_msg.port, res_msg.session.c_str());

		Login_Player *player = LOGIN_MANAGER->pop_login_player();
		if (! player) {
				LOG_INFO("login_player_pool_.pop() return 0.");
				return -1;
		}

		player->reset();
		player->set_cid(cid);
		Login_Player_Info player_info;
		player_info.session_tick = Time_Value::gettimeofday().sec();
		player_info.account = msg.account;
		player_info.gate_ip = res_msg.ip;
		player_info.gate_port = res_msg.port;
		player_info.session = res_msg.session;
		player->set_player_info(player_info);
		LOGIN_MANAGER->bind_cid_login_player(cid, player);
		LOGIN_MANAGER->bind_account_login_player(msg.account, player);

		Block_Buffer res_buf;
		res_buf.make_inner_message(RES_CLIENT_REGISTER);
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
	}
	else
	{//fail
		LOG_DEBUG("client_register fail status:%d", status);
		Block_Buffer res_buf;
		res_buf.make_inner_message(RES_CLIENT_REGISTER, ERROR_REGISTER_VERIFY_FAIL);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
		LOGIN_MANAGER->close_client(cid);
	}

	return ret;
}

//登录
int Login_Client_Messager::client_login(int cid, int msg_id, Block_Buffer &buf) {
	int ret = 0;

	MSG_100001 msg;
	msg.deserialize(buf);
	Login_Player *player = LOGIN_MANAGER->find_account_login_player(msg.account);
	int status = LOGIN_MANAGER->client_login(msg.account, msg.password);
	if (!player && !status)
	{
		MSG_500001 res_msg;
		res_msg.reset();

		LOGIN_MANAGER->get_gate_ip(msg.account, res_msg.ip, res_msg.port);
		make_session(msg.account, res_msg.session);
		player = LOGIN_MANAGER->pop_login_player();

		if (! player) {
				LOG_INFO("login_player_pool_.pop() return 0.");
				return -1;
		}

		player->reset();
		player->set_cid(cid);
		Login_Player_Info player_info;
		player_info.session_tick = Time_Value::gettimeofday().sec();
		player_info.account = msg.account;
		player_info.gate_ip = res_msg.ip;
		player_info.gate_port = res_msg.port;
		player_info.session = res_msg.session;
		player->set_player_info(player_info);
		LOGIN_MANAGER->bind_cid_login_player(cid, player);
		LOGIN_MANAGER->bind_account_login_player(msg.account, player);

		Block_Buffer res_buf;
		res_buf.make_inner_message(RES_CLIENT_LOGIN);
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
	}
	else
	{
		LOG_DEBUG("client_login fail, account:%s, password:%s", msg.account.c_str(), msg.password.c_str());
		Block_Buffer res_buf;
		res_buf.make_inner_message(RES_CLIENT_LOGIN, ERROR_LOGIN_VERIFY_FAIL);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
		LOGIN_MANAGER->close_client(cid);
	}

	return ret;
}
