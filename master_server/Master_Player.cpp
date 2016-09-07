/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Master_Manager.h"

Master_Player::Master_Player(void):gate_cid_(0), game_cid_(0), player_cid_(0), role_id_(0) { }

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

void Master_Player::reset(void) {
	gate_cid_ = 0;
	game_cid_ = 0;
	player_cid_ = 0;
	role_id_ = 0;
	recycle_tick_.reset();
}

int Master_Player::tick(Time_Value &now) {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		MASTER_MANAGER->unbind_master_player(*this);
		reset();
		MASTER_MANAGER->push_master_player(this);
	}

	return 0;
}

int Master_Player::link_close() {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE)
		return 0;

	recycle_tick_.set(Recycle_Tick::RECYCLE);

	int cid = GET_CID(gate_cid_, player_cid_);
	MASTER_MANAGER->push_drop_player_cid(cid);
	return 0;
}
