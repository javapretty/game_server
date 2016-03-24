/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Game_Manager.h"

Game_Player::Game_Player(void):
	gate_cid_(0),
	player_cid_(0),
  read_player_data_buffer_(0),
  write_player_data_buffer_(0)
{ }

Game_Player::~Game_Player(void) { }

const Time_Value Game_Player::save_interval_ = Time_Value(30, 0);

int Game_Player::respond_success_result(int msg_id, Block_Buffer *buf) {
		return respond_error_result(msg_id, 0, buf);
}

int Game_Player::respond_error_result(int msg_id, int err, Block_Buffer *buf) {
	if (buf == 0) {
		Block_Buffer msg_buf;
		msg_buf.make_player_message(msg_id, err, player_cid_);
		msg_buf.finish_message();
		return GAME_MANAGER->send_to_gate(gate_cid_, msg_buf);
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
		GAME_MANAGER->send_to_gate(gate_cid_, *buf);

		buf->set_read_idx(rd_idx); /// 复位传入的buf参数
		return 0;
	}
}

int Game_Player::load_player(Player_Data &player_data) {
	GAME_MANAGER->logining_map().erase(player_data.player_info.account);
	player_data_ = player_data;

	read_player_data_buffer_ = GAME_MANAGER->pop_block_buffer();
	write_player_data_buffer_ = GAME_MANAGER->pop_block_buffer();
	read_player_data_buffer_->write_int32(gate_cid_);
	read_player_data_buffer_->write_int32(player_cid_);
	player_data_.serialize(*read_player_data_buffer_);
	GAME_MANAGER->push_player_data(read_player_data_buffer_);
	return 0;
}

int Game_Player::save_player(bool is_logout) {
	//js端写入值后，反序列化获取值
	if (write_player_data_buffer_->readable_bytes() > 0) {
		player_data_.deserialize(*write_player_data_buffer_);
	}

	if (is_logout) {
		player_data_.status = Player_Data::ROLE_SAVE_OFFLINE;
		player_data_.player_info.last_sign_out_time = GAME_MANAGER->tick_time().sec();
	} else {
		if (player_data_.change_module.size() <= 0) {
			LOG_INFO("no player data module change, role_id=%ld, name=%s", player_data_.player_info.role_id, player_data_.player_info.role_name.c_str());
			return -1;
		}
	}

	MSG_150003 msg;
	msg.player_data = player_data_;
	Block_Buffer buf;
	buf.make_inner_message(SYNC_GAME_DB_SAVE_PLAYER_INFO);
	msg.serialize(buf);
	buf.finish_message();
	GAME_MANAGER->send_to_db(buf);

	// 登出的时候要确保玩家信息正常保存后才可登录
	if (is_logout) {
		GAME_MANAGER->saving_map().insert(std::make_pair(player_data_.player_info.role_id, Saving_Info(player_data_.player_info.role_id, Time_Value::gettimeofday())));
	}
	return 0;
}

int Game_Player::sign_in(std::string account) {
	LOG_DEBUG("player sign in game_server. account=[%s], gate_cid = %d, player_cid = %d, role_id=%ld, name=%s",
			account.c_str(), gate_cid_, player_cid_, player_data_.player_info.role_id, player_data_.player_info.role_name.c_str());

	respond_role_login();
	sync_signin_to_master();
	return 0;
}

int Game_Player::sign_out(void) {
	save_player(true);
	sync_signout_to_master();
	reset();
	GAME_MANAGER->push_block_buffer(read_player_data_buffer_);
	GAME_MANAGER->push_block_buffer(write_player_data_buffer_);
	return 0;
}

void Game_Player::reset(void) {
	gate_cid_ = 0;
	player_cid_ = 0;
	recycle_tick_.reset();
	last_save_tick_ = Time_Value::gettimeofday();
	player_data_.reset();
}

int Game_Player::sync_signin_to_master(void) {
	Block_Buffer buf;
	buf.make_player_message(SYNC_GAME_MASTER_PLYAER_SIGNIN, 0, player_cid_);
	MSG_160001 msg;
	msg.player_info.role_id = player_data_.player_info.role_id;
	msg.player_info.account = player_data_.player_info.account;
	msg.player_info.role_name = player_data_.player_info.role_name;
	msg.player_info.level = player_data_.player_info.level;
	msg.serialize(buf);
	buf.finish_message();
	return GAME_MANAGER->send_to_master(buf);
}

int Game_Player::sync_signout_to_master(void) {
	Block_Buffer buf;
	buf.make_player_message(SYNC_GAME_MASTER_PLAYER_SIGNOUT, 0, player_cid_);
	MSG_160002 msg;
	msg.role_id = player_data_.player_info.role_id;
	msg.serialize(buf);
	buf.finish_message();
	return GAME_MANAGER->send_to_master(buf);
}

int Game_Player::tick(Time_Value &now) {
	if (recycle_tick(now) == 1)
		return 0;

	if (now - last_save_tick_ > save_interval_) {
		save_player();
		last_save_tick_ = now;
	}

	return 0;
}

int Game_Player::respond_role_login(void) {
	MSG_520001 msg;
	msg.role_info.role_id = player_data_.player_info.role_id;
	msg.role_info.account = player_data_.player_info.account;
	msg.role_info.role_name = player_data_.player_info.role_name;
	msg.role_info.level = player_data_.player_info.level;
	msg.role_info.exp = player_data_.player_info.exp;
	msg.role_info.career = player_data_.player_info.career;
	msg.role_info.gender = player_data_.player_info.gender;
	msg.role_info.vitality = player_data_.player_info.vitality;
	msg.role_info.buy_vitality_times = player_data_.player_info.buy_vitality_times;
	msg.role_info.vip_level = player_data_.player_info.vip_level;
	msg.role_info.vip_exp = player_data_.player_info.vip_exp;
	msg.role_info.charge_gold = player_data_.player_info.charge_gold;
	msg.role_info.skill_point = player_data_.player_info.skill_point;

	Block_Buffer buf;
	buf.make_player_message(RES_FETCH_ROLE_INFO, 0, player_cid_);
	msg.serialize(buf);
	buf.finish_message();
	GAME_MANAGER->send_to_gate(gate_cid_, buf);

	return 0;
}

int Game_Player::link_close() {
	if (recycle_tick_.status_ == Recycle_Tick::RECYCLE) return 0;

	this->set_recycle();
	return 0;
}

void Game_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);

	int cid = gate_cid_ * 10000 + player_cid_;
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
			LOG_DEBUG("game_player recyle, role_id = %ld, role_name = %s", player_data_.player_info.role_id, player_data_.player_info.role_name.c_str());
			ret = 1;
			GAME_MANAGER->unbind_game_player(*this);
			sign_out();
			GAME_MANAGER->push_game_player(this);
		}
	}
	return ret;
}
