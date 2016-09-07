/*
 * Login_Player.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: zhangyalei
 */

#include "Login_Manager.h"
#include "Login_Server.h"

Login_Player::Login_Player(void) { }

Login_Player::~Login_Player(void) { }

void Login_Player::reset(void) {
	Player::reset();
	session_info_.reset();
}

int Login_Player::tick(Time_Value &now) {
	if (now - session_info_.session_tick > Time_Value(10)) {
		//session到期，断开客户端与login的连接
		LOG_INFO("player drop from login, cid:%d, account:%s", player_cid(), account().c_str());
		LOGIN_CLIENT_SERVER->receive().push_drop(player_cid());
		link_close();
	}

	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		LOGIN_MANAGER->unbind_player(*this);
		reset();
		LOGIN_MANAGER->push_player(this);
	}
	return 0;
}

int Login_Player::link_close(bool server_close) {
	if (Player::link_close() < 0) return -1;

	//服务器主动关闭，通知客户端
	if (server_close) {
		Block_Buffer buf;
		buf.make_server_message(ACTIVE_DISCONNECT, ERROR_DISCONNECT_SELF);
		buf.finish_message();
		LOGIN_MANAGER->send_to_client(player_cid(), buf);
	}

	//修改session_tick的值，防止login_manager对同一个链接重复push_drop
	session_info_.session_tick += Time_Value(10);
	return 0;
}
