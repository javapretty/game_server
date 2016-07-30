/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Master_Manager.h"

Master_Player::Master_Player(void):gate_cid_(0), game_cid_(0), player_cid_(0), load_player_data_buffer_(0), save_player_data_buffer_(0) { }

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
			LOG_ERROR("Block_Buffer space error, msg_id:%d", msg_id);
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

int Master_Player::sync_data_to_game(int msg_id, Block_Buffer *buf) {
	Block_Buffer buffer;
	buffer.make_inner_message(msg_id);
	buffer.write_int64(player_info_.role_id);
	buffer.copy(buf);
	buffer.finish_message();
	MASTER_MANAGER->send_to_game(game_cid_, buffer);

	return 0;
}

int Master_Player::load_player(Master_Player_Info &player_info) {
	player_info_ = player_info;

	LOG_DEBUG("***********load master_player*********** account=[%s], gate_cid=%d, player_cid=%d, role_id=%ld, name=%s",
			player_info_.account.c_str(), gate_cid_, player_cid_, player_info_.role_id, player_info_.role_name.c_str());

	MASTER_MANAGER->bind_gate_cid_master_player(gate_cid_ * 10000 + player_cid_, *this);
	MASTER_MANAGER->bind_role_name_master_player(player_info_.role_name, *this);

	load_player_data_buffer_ = MASTER_MANAGER->pop_block_buffer();
	save_player_data_buffer_ = MASTER_MANAGER->pop_block_buffer();

	load_player_data_buffer_->reset();
	save_player_data_buffer_->reset();
	load_player_data_buffer_->write_int32(gate_cid_);
	load_player_data_buffer_->write_int32(player_cid_);
	player_info_.serialize(*load_player_data_buffer_);
	MASTER_MANAGER->push_player_load_data_buffer(load_player_data_buffer_);
	return 0;
}

int Master_Player::save_player(bool is_logout) {
	LOG_DEBUG("***********save master_player*********** account=[%s], gate_cid=%d, player_cid=%d, role_id=%ld, name=%s",
			player_info_.account.c_str(), gate_cid_, player_cid_, player_info_.role_id, player_info_.role_name.c_str());

	//js端写入值后，反序列化获取值
	if (save_player_data_buffer_->readable_bytes() > 0) {
		player_info_.deserialize(*save_player_data_buffer_);
	}

	return 0;
}

int Master_Player::sign_in(void) {
	return 0;
}

int Master_Player::sign_out(void) {
	save_player(true);
	reset();
	MASTER_MANAGER->push_block_buffer(load_player_data_buffer_);
	MASTER_MANAGER->push_block_buffer(save_player_data_buffer_);
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
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		MASTER_MANAGER->unbind_master_player(*this);
		sign_out();
		MASTER_MANAGER->push_master_player(this);
	}

	return 0;
}

int Master_Player::link_close() {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE)
		return 0;

	recycle_tick_.set(Recycle_Tick::RECYCLE);

	int cid = gate_cid_ * 10000 + player_cid_;
	MASTER_MANAGER->push_drop_player_cid(cid);
	return 0;
}
