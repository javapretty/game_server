/*
 * Login_Player.cpp
 *
 *  Created on: 2016年1月21日
 *      Author: root
 */

#include "Login_Manager.h"
#include "Login_Player.h"
#include "Log.h"

Login_Player::Login_Player(void)
: cid_(0),
  is_register_timer_(0)
{ }

Login_Player::~Login_Player(void) { }

int Login_Player::respond_success_result(int msg_id, Block_Buffer *buf) {
	return this->respond_error_result(msg_id, 0, buf);
}

int Login_Player::respond_error_result(int msg_id, int err, Block_Buffer *buf) {
	if (buf == 0) {
		Block_Buffer msg_buf;
		msg_buf.make_inner_message(msg_id, err);
		msg_buf.finish_message();
		return LOGIN_MANAGER->send_to_client(cid_, msg_buf);
	} else {
		if ((size_t)buf->get_read_idx() < (sizeof(uint16_t) + 2 * sizeof(uint32_t))) {
			MSG_USER("Block_Buffer space error !");
			return 0;
		}
		/// insert head msg_id, status
		size_t rd_idx = buf->get_read_idx();
		size_t wr_idx = buf->get_write_idx();

		size_t head_len = sizeof(uint16_t) + 2 * sizeof(uint32_t); /// len(uint16_t, msg_id(uint32_t), status(uint16_t)

		buf->set_read_idx(buf->get_read_idx() - head_len);
		uint16_t len = buf->readable_bytes() - sizeof(uint16_t);
		buf->set_write_idx(buf->get_read_idx());

		buf->write_uint16(len);
		buf->write_uint32(msg_id);
		buf->write_uint32(err); /// status

		buf->set_write_idx(wr_idx);

		LOGIN_MANAGER->send_to_client(cid_, *buf);

		buf->set_read_idx(rd_idx); /// 复位传入的buf参数

		return 0;
	}
}

void Login_Player::reset(void) {
	cid_ = -1;
	is_register_timer_ = false;
	recycle_tick_.reset();
}

int Login_Player::tick(Time_Value &now) {

	if (recycle_tick(now) == 1)
		return 0;

	if (! is_register_timer_)
		return 0;
	return 0;
}

int Login_Player::register_timer(void) {
	is_register_timer_ = true;
	return 0;
}

int Login_Player::unregister_timer(void) {
	is_register_timer_ = false;
	return 0;
}

int Login_Player::recycle_tick(const Time_Value &now) {
	int ret = 0;
	if (now - recycle_tick_.last_tick_ts_ > Recycle_Tick::tick_interval_) {
		recycle_tick_.last_tick_ts_ = now;
		if (recycle_tick_.status_ == Recycle_Tick::RECYCLE && now - recycle_tick_.last_change_status_ts_ > Recycle_Tick::recycle_time_) {
			ret = 1;
			LOGIN_MANAGER->unbind_login_player(*this);
			reset();
			LOGIN_MANAGER->push_login_player(this);
		}
	}
	return ret;
}
