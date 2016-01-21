/*
 *  Created on: Dec 16, 2015
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
	int32_t cid = 0;
	uint16_t len = 0;
	uint32_t serial_cipher;
	uint32_t msg_time_cipher;
	uint32_t msg_id = 0;
	int32_t  status = 0;

	buf.read_int32(cid);
	buf.read_uint16(len);
	buf.read_uint32(serial_cipher);
	buf.read_uint32(msg_time_cipher);
	buf.read_uint32(msg_id);
	buf.read_int32(status);

	Perf_Mon perf_mon(msg_id);
	switch (msg_id) {
	case REQ_CLIENT_REGISTER:{
			process_110000(cid, msg_id, buf);
			break;
	}
	case REQ_CLIENT_LOGIN: {
			process_110001(cid, msg_id, buf);
			break;
	}
	default:	{
		MSG_USER("error msg_id:%d", msg_id);
		break;
	}
	}

	return 0;
}

//注册
int Login_Client_Messager::process_110000(int cid, int msg_id, Block_Buffer &buf){
	int ret = 0;

	MSG_110000 msg;
	msg.deserialize(buf);
	int status = LOGIN_MANAGER->check_account().client_register(msg.account, msg.password);
	if (status == 0){
		//success
		MSG_510000 res_msg;
		res_msg.reset();
		LOGIN_MANAGER->get_gate_ip(msg.account, res_msg.ip, res_msg.port);
		make_session(msg.account, res_msg.session);
		LOGIN_MANAGER->bind_account_session(msg.account, res_msg.session);
		MSG_DEBUG("client register, account:%s, gate_ip:%s, gate_port:%d, session:%s",
				msg.account.c_str(), res_msg.ip.c_str(), res_msg.port, res_msg.session.c_str());

		Block_Buffer res_buf;
		res_buf.make_message(RES_CLIENT_REGISTER);
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
	}
	else
	{//fail
		LOG_DEBUG("cant not found the user status:%d", status);
		Block_Buffer res_buf;
		res_buf.make_message(RES_CLIENT_REGISTER, ERROR_REGISTER_VERIFY_FAIL);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
		LOGIN_MANAGER->close_client(cid);
	}

	return ret;
}

//登录
int Login_Client_Messager::process_110001(int cid, int msg_id, Block_Buffer &buf) {
	int ret = 0;

	MSG_110001 msg;
	msg.deserialize(buf);
	int status = LOGIN_MANAGER->check_account().client_login(msg.account, msg.password);
	if (status == 0){
		//success
		MSG_510001 res_msg;
		res_msg.reset();
		LOGIN_MANAGER->get_gate_ip(msg.account, res_msg.ip, res_msg.port);
		make_session(msg.account, res_msg.session);
		LOGIN_MANAGER->bind_account_session(msg.account, res_msg.session);
		MSG_DEBUG("client login, account:%s, gate_ip:%s, gate_port:%d, session:%s",
				msg.account.c_str(), res_msg.ip.c_str(), res_msg.port, res_msg.session.c_str());

		Block_Buffer res_buf;
		res_buf.make_message(RES_CLIENT_LOGIN);
		res_msg.serialize(res_buf);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
	}
	else
	{//fail
		LOG_DEBUG("cant not found the user status:%d", status);
		Block_Buffer res_buf;
		res_buf.make_message(RES_CLIENT_LOGIN, ERROR_LOGIN_VERIFY_FAIL);
		res_buf.finish_message();
		LOGIN_MANAGER->send_to_client(cid, res_buf);
		LOGIN_MANAGER->close_client(cid);
	}

	return ret;
}
