/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Game_Manager.h"

Game_Player::Game_Player(void):
  is_register_timer_(0),
  player_data_buffer_(0)
{ }

Game_Player::~Game_Player(void) { }

const Time_Value Game_Player::game_player_save_interval_ = Time_Value(30, 0);

int Game_Player::respond_success_result(int msg_id, Block_Buffer *buf) {
		return respond_error_result(msg_id, 0, buf);
}

int Game_Player::respond_error_result(int msg_id, int err, Block_Buffer *buf) {
	if (buf == 0) {
		Block_Buffer msg_buf;
		msg_buf.make_player_message(msg_id, err, cid_info_.player_cid);
		msg_buf.finish_message();
		return GAME_MANAGER->send_to_gate(cid_info_.gate_cid, msg_buf);
	} else {
		size_t head_len = sizeof(int16_t) + 3 * sizeof(int32_t); ///len(int16_t), msg_id(int32_t), status(int32_t), player_cid(int32_t)
		if ((size_t)buf->get_read_idx() < head_len) {
			MSG_USER("Block_Buffer space error !");
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
		buf->write_int32(cid_info_.player_cid);
		buf->set_write_idx(wr_idx);
		GAME_MANAGER->send_to_gate(cid_info_.gate_cid, *buf);

		buf->set_read_idx(rd_idx); /// 复位传入的buf参数
		return 0;
	}
}

int Game_Player::load_player(Player_Data &player_data) {
	GAME_MANAGER->logining_map().erase(player_data.game_player_info.account);
	player_data_ = player_data;
	player_data_.role_id = player_data_.game_player_info.role_id;
	return 0;
}

int Game_Player::save_player(bool is_logout) {
	MSG_150003 msg;
	msg.player_data = player_data_;
	if (!is_logout && !msg.player_data.can_save()) {
		return -1;
	}
	if (is_logout) {
		msg.player_data.status = Player_Data::ROLE_SAVE_OFFLINE;
	}
	Block_Buffer buf;
	buf.make_inner_message(SYNC_GAME_DB_SAVE_PLAYER_INFO);
	msg.serialize(buf);
	buf.finish_message();
	GAME_MANAGER->send_to_db(buf);

	// 登出的时候要确保玩家信息正常保存后才可登录
	if (is_logout) {
		GAME_MANAGER->saving_map().insert(std::make_pair(this->game_player_info().role_id, Saving_Info(this->game_player_info().role_id, Time_Value::gettimeofday())));
	}

	return 0;
}

int Game_Player::sign_in(std::string account) {
	MSG_DEBUG("player sign in game_server. account=[%s], gate_cid = %d, player_cid = %d, role_id=%ld, name=%s",
			account.c_str(), cid_info_.gate_cid, cid_info_.player_cid, player_data_.game_player_info.role_id, player_data_.game_player_info.role_name.c_str());

	player_data_buffer_ = GAME_MANAGER->pop_block_buffer();
	player_data_buffer_->write_int32(cid_info_.gate_cid);
	player_data_buffer_->write_int32(cid_info_.player_cid);
	player_data_.game_player_info.serialize(*player_data_buffer_);
	player_data_.bag_info.serialize(*player_data_buffer_);
	player_data_.mail_info.serialize(*player_data_buffer_);
	GAME_MANAGER->push_player_data(player_data_buffer_);

	login_success();
	register_timer();
	sync_signin_to_master();
	return 0;
}

int Game_Player::sign_out(void) {
	player_data_.game_player_info.deserialize(*player_data_buffer_);
	player_data_.bag_info.deserialize(*player_data_buffer_);
	player_data_.mail_info.deserialize(*player_data_buffer_);
	GAME_MANAGER->push_block_buffer(player_data_buffer_);

	Time_Value now = GAME_MANAGER->tick_time();
	this->player_data_.game_player_info.last_sign_out_time = now.sec();

	save_player(true);
	unregister_timer();
	sync_signout_to_master();
	reset();

	return 0;
}

void Game_Player::reset(void) {
	cid_info_.reset();
	is_register_timer_ = false;
	recycle_tick_.reset();
	last_save_timestamp_ = Time_Value::zero;

	player_data_.reset();
}

int Game_Player::sync_signin_to_master(void) {
	Block_Buffer buf;
	buf.make_player_message(SYNC_GAME_MASTER_PLYAER_SIGNIN, 0, cid_info_.player_cid);
	MSG_160001 msg;
	msg.player_info.role_id = player_data_.game_player_info.role_id;
	msg.player_info.account = player_data_.game_player_info.account;
	msg.player_info.role_name = player_data_.game_player_info.role_name;
	msg.player_info.level = player_data_.game_player_info.level;
	msg.serialize(buf);
	buf.finish_message();
	return GAME_MANAGER->send_to_master(buf);
}

int Game_Player::sync_signout_to_master(void) {
	Block_Buffer buf;
	buf.make_player_message(SYNC_GAME_MASTER_PLAYER_SIGNOUT, 0, cid_info_.player_cid);
	MSG_160002 msg;
	msg.role_id = player_data_.game_player_info.role_id;
	msg.serialize(buf);
	buf.finish_message();
	return GAME_MANAGER->send_to_master(buf);
}

int Game_Player::tick(Time_Value &now) {
	if (recycle_tick(now) == 1)
		return 0;

	if (! is_register_timer_)
		return 0;

	if (now - last_save_timestamp_ > game_player_save_interval_) {
		save_player();
		last_save_timestamp_ = now;
	}

	return 0;
}

int Game_Player::register_timer(void) {
	is_register_timer_ = true;
	return 0;
}

int Game_Player::unregister_timer(void) {
	is_register_timer_ = false;
	return 0;
}

int Game_Player::login_success(void) {
	respond_role_login();
	return 0;
}

int Game_Player::respond_role_login(void) {
	MSG_520001 msg;
	msg.reset();
	msg.role_info.role_id = player_data_.game_player_info.role_id;
	msg.role_info.account = player_data_.game_player_info.account;
	msg.role_info.role_name = player_data_.game_player_info.role_name;
	msg.role_info.career = player_data_.game_player_info.career;
	msg.role_info.gender = player_data_.game_player_info.gender;
	msg.role_info.level = player_data_.game_player_info.level;

	Block_Buffer buf;
	buf.reset();
	buf.make_player_message(RES_FETCH_ROLE_INFO, 0, cid_info_.player_cid);
	msg.serialize(buf);
	buf.finish_message();
	GAME_MANAGER->send_to_gate(cid_info_.gate_cid, buf);

	return 0;
}

int Game_Player::link_close() {
	if (recycle_tick_.status_ == Recycle_Tick::RECYCLE) return 0;

	this->set_recycle();
	return 0;
}

void Game_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);

	int cid = cid_info_.gate_cid * 10000 + cid_info_.player_cid;
	GAME_MANAGER->push_drop_player_cid(cid);
}

int Game_Player::recycle_status(void) {
	return recycle_tick_.status_;
}

int Game_Player::recycle_tick(const Time_Value &now) {
	int ret = 0;
	if (now - recycle_tick_.last_tick_ts_ > Recycle_Tick::tick_interval_) {
		recycle_tick_.last_tick_ts_ = now;
		if (recycle_tick_.status_ == Recycle_Tick::RECYCLE && now - recycle_tick_.last_change_status_ts_ > Recycle_Tick::recycle_time_) {
			MSG_DEBUG("game_player recyle, role_id = %ld, role_name = %s", player_data_.game_player_info.role_id, player_data_.game_player_info.role_name.c_str());
			ret = 1;
			GAME_MANAGER->unbind_game_player(*this);
			sign_out();
			GAME_MANAGER->push_game_player(this);
		}
	}
	return ret;
}
