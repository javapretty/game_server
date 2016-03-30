/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Msg_Define.h"
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

int Master_Player::load_player(Master_Player_Info &player_info) {
	player_info_ = player_info;

	load_player_data_buffer_ = MASTER_MANAGER->pop_block_buffer();
	save_player_data_buffer_ = MASTER_MANAGER->pop_block_buffer();
	load_player_data_buffer_->write_int32(gate_cid_);
	load_player_data_buffer_->write_int32(player_cid_);
	player_info_.serialize(*load_player_data_buffer_);
	MASTER_MANAGER->push_player_load_data_buffer(load_player_data_buffer_);
	return 0;
}

int Master_Player::save_player(bool is_logout) {
	//js端写入值后，反序列化获取值
	if (save_player_data_buffer_->readable_bytes() > 0) {
		player_info_.deserialize(*save_player_data_buffer_);
	}

	return 0;
}

int Master_Player::sign_in(void) {
	LOG_DEBUG("player sign in master_server. account=[%s], gate_cid = %d, player_cid = %d, role_id=%ld, name=%s",
			player_info_.account.c_str(), gate_cid_, player_cid_, player_info_.role_id, player_info_.role_name.c_str());

	return 0;
}

int Master_Player::sign_out(void) {
	save_player(true);
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
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		MASTER_MANAGER->unbind_master_player(*this);
		sign_out();
		MASTER_MANAGER->push_master_player(this);
	}

	return 0;
}

int Master_Player::send_chat_info(Block_Buffer &buf) {
	MSG_110001 msg;
	msg.deserialize(buf);
	if (msg.chat_content.length() > 100) {
		respond_error_result(RES_SEND_CHAT_INFO, ERROR_CLIENT_PARAM);
	}

	MSG_510001 msg_res;
	msg_res.chat_type = msg.chat_type;
	msg_res.chat_content = msg.chat_content;
	msg_res.role_name = player_info_.role_name;
	msg_res.gender = player_info_.gender;
	msg_res.career = player_info_.career;
	msg_res.vip_level = player_info_.vip_level;
	Block_Buffer buf_res;
	buf_res.make_player_message(RES_SEND_CHAT_INFO, 0, player_cid_);
	msg_res.serialize(buf_res);
	buf_res.finish_message();

	switch(msg.chat_type) {
	case 1: {
		//世界聊天
		MASTER_MANAGER->boardcast_msg_to_all(buf_res);
		break;
	}
	case 2: {
		//私密聊天
		Master_Player *player = MASTER_MANAGER->find_role_name_master_player(msg.role_name);
		if (!player) {
			LOG_DEBUG("send private chat, role_name:%s offline", msg.role_name.c_str());
			return respond_error_result(RES_SEND_CHAT_INFO, ERROR_ROLE_OFFLINE);
		}
		MASTER_MANAGER->send_to_gate(player->gate_cid(), buf_res);
		break;
	}
	default:
		LOG_ERROR("chat type error, role_id:%ld, type:%d", player_info_.role_id, msg.chat_type);
	}

	return 0;
}
