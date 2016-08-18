/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Gate_Manager.h"
#include "Common_Func.h"

Gate_Player::Gate_Player(void) :
	player_cid_(0),
	game_cid_(0) { }

Gate_Player::~Gate_Player(void) { }

void Gate_Player::reset(void) {
	player_cid_ = 0;
	game_cid_ = 0;
	account_.clear();
	msg_info_.reset();
	recycle_tick_.reset();
}

int Gate_Player::tick(Time_Value &now) {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE && now > recycle_tick_.recycle_tick) {
		GATE_MANAGER->unbind_gate_player(*this);
		reset();
		GATE_MANAGER->push_gate_player(this);
	}

	return 0;
}

int Gate_Player::link_close() {
	if (recycle_tick_.status == Recycle_Tick::RECYCLE)
		return 0;

	recycle_tick_.set(Recycle_Tick::RECYCLE);

	//gate同步玩家下线到game
	Block_Buffer game_buf;
	game_buf.make_player_message(SYNC_GATE_GAME_PLAYER_LOGOUT, 0, player_cid_);
	game_buf.finish_message();
	GATE_MANAGER->send_to_game(game_cid_, game_buf);

	//gate同步玩家下线到master
	Block_Buffer master_buf;
	master_buf.make_player_message(SYNC_GATE_MASTER_PLAYER_LOGOUT, 0, player_cid_);
	master_buf.finish_message();
	GATE_MANAGER->send_to_master(master_buf);

	return 0;
}

int Gate_Player::verify_msg_info(uint32_t serial_cipher, uint32_t msg_time_cipher) {
	if (! msg_info_.is_inited) {
		msg_info_.hash_key = elf_hash(account_.c_str(), account_.length());
	}
	uint32_t serial = serial_cipher ^ msg_info_.hash_key;
	uint32_t msg_time = msg_time_cipher ^ serial_cipher;

	if (! msg_info_.is_inited) {
		msg_info_.is_inited = true;
		msg_info_.msg_serial = serial; /// 第一条消息序号
		msg_info_.msg_timestamp = GATE_MANAGER->tick_time(); /// 第一条消息时间戳
		msg_info_.msg_interval_count_ = 1;
		msg_info_.msg_interval_timestamp = GATE_MANAGER->tick_time();
		return 0;
	}

	///判断发包频率,3s内发包超过90个判断为非法
	if (GATE_MANAGER->tick_time() - msg_info_.msg_interval_timestamp < Time_Value(3, 0)) {
		if (msg_info_.msg_interval_count_ > 90) {
			return ERROR_MSG_COUNT;
		}
	} else {
		msg_info_.msg_interval_count_ = 0;
		msg_info_.msg_interval_timestamp = GATE_MANAGER->tick_time();
	}

	///判断包的序号
	if (serial <= msg_info_.msg_serial || serial - msg_info_.msg_serial > static_cast<uint32_t>(10)) {
		LOG_ERROR("serial = %d, msg_detail_.msg_serial = %d error", serial, msg_info_.msg_serial);
		return ERROR_MSG_SERIAL;
	}

	/// 判断包时间戳
	Time_Value msg_time_tv(msg_time, 0);
	if (std::abs(msg_time - msg_info_.msg_timestamp.sec()) > 900) {
		LOG_ERROR("msg_time = %d, msg_timestamp = %d error", msg_time, msg_info_.msg_timestamp.sec());
		return ERROR_MSG_TIME;
	}

	msg_info_.msg_serial = serial;
	msg_info_.msg_timestamp = msg_time_tv;
	++msg_info_.msg_interval_count_;

	return 0;
}
