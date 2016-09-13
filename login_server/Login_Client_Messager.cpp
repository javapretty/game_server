/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

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
	LOGIN_MANAGER->add_recv_bytes(buf.readable_bytes());

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

	LOGIN_MANAGER->add_msg_count(msg_id);
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
	Login_Player *player = dynamic_cast<Login_Player*>(LOGIN_MANAGER->find_account_player(msg.account));
	if (!player) {
		int status = LOGIN_MANAGER->client_login(msg.account, msg.password);
		if (status < 0) {
			LOG_ERROR("connect login fail, account:%s, password:%d, status:%d", msg.account.c_str(), msg.password.c_str(), status);
			LOGIN_MANAGER->close_client(0, cid, ERROR_LOGIN_FAIL);
			return -1;
		}

		//success
		MSG_500001 res_msg;
		res_msg.reset();
		LOGIN_MANAGER->get_gate_ip(msg.account, res_msg.gate_ip, res_msg.gate_port);
		make_session(msg.account, res_msg.session);

		player = LOGIN_MANAGER->pop_player();
		if (!player) {
			LOG_ERROR("login_player_pool_ pop error");
			return -1;
		}

		player->reset();
		player->set_player_cid(cid);
		player->set_account(msg.account);
		Session_Info session_info;
		session_info.session = res_msg.session;
		session_info.session_tick = Time_Value::gettimeofday();
		session_info.gate_ip = res_msg.gate_ip;
		session_info.gate_port = res_msg.gate_port;
		player->set_session_info(session_info);
		LOGIN_MANAGER->bind_cid_player(cid, player);
		LOGIN_MANAGER->bind_account_player(msg.account, player);

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
		LOGIN_MANAGER->close_client(0, cid, ERROR_LOGIN_FAIL);
	}
	return 0;
}
