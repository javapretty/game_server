/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
#include "Master_Manager.h"

Master_Player::Master_Player(void):gate_cid_(0), game_cid_(0), player_cid_(0) { }

Master_Player::~Master_Player(void) { }

int Master_Player::respond_success_result(int msg_id, Block_Buffer *buf) {
	return respond_error_result(msg_id, 0, buf);
}

int Master_Player::respond_error_result(int msg_id, int err, Block_Buffer *buf) {
	if (buf == 0) {
		Block_Buffer msg_buf;
		msg_buf.make_player_message(msg_id, err, player_cid_);
		msg_buf.finish_message();
		return MASTER_MANAGER->send_to_gate(gate_cid_, msg_buf);
	} else {
		size_t head_len = sizeof(int16_t) + 3 * sizeof(int32_t); ///len(int16_t), msg_id(int32_t), status(int32_t), player_cid(int32_t)
		if ((size_t)buf->get_read_idx() < head_len) {
			LOG_INFO("Block_Buffer space error !");
			return 0;
		}

		/// insert buf head
		size_t rd_idx = buf->get_read_idx();
		size_t wr_idx = buf->get_write_idx();

		buf->set_read_idx(buf->get_read_idx() - head_len);
		int16_t len = buf->readable_bytes() - sizeof(int16_t);
		buf->set_write_idx(buf->get_read_idx());

		buf->write_int16(len);
		buf->write_int32(msg_id);
		buf->write_int32(err); /// status
		buf->write_int32(player_cid_);
		buf->set_write_idx(wr_idx);
		MASTER_MANAGER->send_to_gate(gate_cid_, *buf);

		buf->set_read_idx(rd_idx); /// 复位传入的buf参数
		return 0;
	}
}

int Master_Player::sign_in(Master_Player_Info &player_info) {
	player_info_ = player_info;

	LOG_DEBUG("player sign in master_server. account=[%s], gate_cid=%d, game_cid=%d, player_cid=%d, role_id=%ld, name=%s",
			player_info_.account.c_str(), gate_cid_, game_cid_, player_cid_, player_info_.role_id, player_info_.role_name.c_str());
	return 0;
}

int Master_Player::sign_out(void) {
	reset();

	return 0;
}

void Master_Player::reset(void) {
	gate_cid_ = 0;
	game_cid_ = 0;
	player_cid_ = 0;
	recycle_tick_.reset();
	player_info_.reset();
}

int Master_Player::tick(Time_Value &now) {
	if (recycle_tick(now) == 1)
		return 0;

	return 0;
}

int Master_Player::recycle_tick(const Time_Value &now) {
	int ret = 0;
	if (now - recycle_tick_.last_tick_ts_ > Recycle_Tick::tick_interval_) {
		recycle_tick_.last_tick_ts_ = now;
		if (recycle_tick_.status_ == Recycle_Tick::RECYCLE && now - recycle_tick_.last_change_status_ts_ > Recycle_Tick::recycle_time_) {
			ret = 1;
			MASTER_MANAGER->unbind_master_player(*this);
			sign_out();
			MASTER_MANAGER->push_master_player(this);
		}
	}
	return ret;
}

int Master_Player::send_chat_info(Block_Buffer &buf) {
	MSG_110001 msg;
	msg.deserialize(buf);
	if (msg.chat_content.length() > 100) {

	}

	return 0;
}
