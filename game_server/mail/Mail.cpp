/*
 * aa.cpp
 *
 *  Created on: Jun 7, 2012
 *      Author: ChenLong
 */

#include "Mail.h"
#include "Game_Player.h"
#include "Game_Manager.h"

Mail::Mail(void):player_(0) { }

Mail::~Mail(void) { }

int Mail::init(Game_Player *player) {
	player_ = player;
	return 0;
}

int Mail::load_data(Player_Data &data) {
	mail_info_ = data.mail_info;
	return 0;
}

int Mail::save_data(Player_Data &data) {
	data.mail_info = mail_info_;
	mail_info_.is_change = false;
	return 0;
}

void Mail::reset(void) {
	mail_info_.reset();
}

int Mail::fetch_mail_info(void) {
	Block_Buffer buf;
	MSG_520200 msg;
  for (Mail_Info::Mail_Map::iterator iter = mail_info_.mail_map.begin(); iter != mail_info_.mail_map.end(); ++iter) {
    	msg.mail_detail_vec.push_back(iter->second);
    }
  msg.serialize(buf);
  return player_->respond_success_result(RES_FETCH_MAIL_INFO, &buf);
}

int Mail::pickup_mail(MSG_120201 &msg) {
	Block_Buffer res_buf;
	MSG_520201 res_msg;
	int result = 0;
	if (msg.mail_id != 0) {
		//收取一封邮件附件
		Mail_Info::Mail_Map::iterator iter = mail_info_.mail_map.find(msg.mail_id);
		if (iter == mail_info_.mail_map.end()) {
			player_->respond_error_result(RES_PICKUP_MAIL, ERROR_CLIENT_PARAM);
		}
		result = this->pickup_mail(iter->second);
		if (result == 0) {
			res_msg.mail_id_vec.push_back(msg.mail_id);
		}
	} else {
		for (Mail_Info::Mail_Map::iterator iter = mail_info_.mail_map.begin();
					iter != mail_info_.mail_map.end(); ++iter) {
			result = this->pickup_mail(iter->second);
			if (result == 0) {
				res_msg.mail_id_vec.push_back(iter->first);
			}
		}
	}
	res_msg.serialize(res_buf);
	player_->respond_success_result(RES_PICKUP_MAIL, &res_buf);

	mail_info_.save_change();
	return 0;
}

int Mail::delete_mail(MSG_120202 &msg) {
	Block_Buffer res_buf;
	MSG_520202 res_msg;
	int result = 0;
	if (msg.mail_id != 0) {
		//删除一封邮件附件
		Mail_Info::Mail_Map::iterator iter = mail_info_.mail_map.find(msg.mail_id);
		if (iter == mail_info_.mail_map.end()) {
			player_->respond_error_result(RES_PICKUP_MAIL, ERROR_CLIENT_PARAM);
		}
		result = this->pickup_mail(iter->second);
		if (result == 0) {
			res_msg.mail_id_vec.push_back(msg.mail_id);
		}
		mail_info_.mail_map.erase(iter);
	} else {
		for (Mail_Info::Mail_Map::iterator iter = mail_info_.mail_map.begin();
				iter != mail_info_.mail_map.end();) {
			result = this->pickup_mail(iter->second);
			if (result == 0) {
				res_msg.mail_id_vec.push_back(msg.mail_id);
			}
			mail_info_.mail_map.erase(iter++);	//map erese后迭代器失效，所以要先将迭代器++
		}
	}
	res_msg.serialize(res_buf);
	player_->respond_success_result(RES_DEL_MAIL, &res_buf);

	mail_info_.save_change();
	return 0;
}

int Mail::send_mail(MSG_120203 &msg) {
	Game_Player *player = GAME_MANAGER->find_role_name_game_player(msg.receiver_name);
	if (!player) {
		return player_->respond_error_result(RES_SEND_MAIL, ERROR_ROLE_NOT_EXIST);
	}
	role_id_t receiver_id = player->game_player_info().role_id;
	if (receiver_id == mail_info_.role_id)
		return player_->respond_error_result(RES_SEND_MAIL, ERROR_CLIENT_PARAM);
	if (msg.mail_detail.mail_title.size() > 64 || msg.mail_detail.mail_content.size() > 512)
		return player_->respond_error_result(RES_SEND_MAIL, ERROR_CLIENT_PARAM);

	std::vector<Money_Sub_Info> money_sub_list;
	if (msg.mail_detail.money_info.copper > 0)
		money_sub_list.push_back(Money_Sub_Info(COPPER_ONLY, msg.mail_detail.money_info.copper));
	if (msg.mail_detail.money_info.gold > 0)
		money_sub_list.push_back(Money_Sub_Info(GOLD_ONLY, msg.mail_detail.money_info.gold));
	int result = player_->bag().bag_try_sub_money(money_sub_list);
	if (result != 0)
		return player_->respond_error_result(RES_SEND_MAIL, result);

	std::vector<Item_Info> item_vector;
	for (std::vector<Item_Basic_Info>::iterator it = msg.mail_detail.item_vector.begin();
			it != msg.mail_detail.item_vector.end(); ++it) {
		Bag_Info::Item_Map::const_iterator iter = player_->bag().bag_info().item_map.find(it->index);
		if (iter == player_->bag().bag_info().item_map.end()) {
			return player_->respond_error_result(RES_SEND_MAIL, ERROR_CLIENT_PARAM);
		}
		item_vector.push_back(iter->second);
		break;
	}

	if (msg.mail_detail.item_vector.size() > 0) {
		result = player_->bag().bag_erase_item(Index_Amount(msg.mail_detail.item_vector[0].index, msg.mail_detail.item_vector[0].amount, UNBIND_ONLY));
		if (result != 0)
			return player_->respond_error_result(RES_SEND_MAIL, result);
	}
	player_->bag().bag_sub_money(money_sub_list);

	this->send_mail(receiver_id, msg.mail_detail);

	return player_->respond_success_result(RES_SEND_MAIL);
}

int Mail::pickup_mail(Mail_Detail &mail_detail) {
	std::vector<Money_Add_Info> money_add_list;
	if (mail_detail.money_info.copper > 0)
		money_add_list.push_back(Money_Add_Info(COPPER, mail_detail.money_info.copper));
	if (mail_detail.money_info.bind_copper > 0)
		money_add_list.push_back(Money_Add_Info(BIND_COPPER, mail_detail.money_info.bind_copper));
	if (mail_detail.money_info.gold > 0)
		money_add_list.push_back(Money_Add_Info(GOLD, mail_detail.money_info.gold));
	if (mail_detail.money_info.coupon > 0)
		money_add_list.push_back(Money_Add_Info(COUPON, mail_detail.money_info.coupon));

	if (money_add_list.size() > 0) {
	int result = player_->bag().bag_try_add_money(money_add_list);
	if (result != 0)
		return result;
	}

  std::vector<Item_Info> item_list;
  for (std::vector<Item_Basic_Info>::iterator iter = mail_detail.item_vector.begin(); iter != mail_detail.item_vector.end(); ++iter) {
	  	item_list.push_back(Item_Info(*iter));
   	}
  int result = player_->bag().bag_insert_item(BAG_T_BAG_INDEX, item_list);
  if (result != 0) {
    	return result;
   	}

  if (money_add_list.size() > 0) {
	  player_->bag().bag_add_money(money_add_list);
    }

  mail_detail.pickup = 1;
	return 0;
}

int Mail::send_mail(role_id_t receiver_id, Mail_Detail &mail_detail) {
	int result = 0;
	//参数验证
	if (receiver_id <= 0 || mail_detail.sender_type <= 0 || mail_detail.sender_id <= 0 || mail_detail.sender_name.empty()
			|| mail_detail.mail_title.empty() || mail_detail.money_info.gold < 0 || mail_detail.money_info.copper < 0
			|| mail_detail.money_info.bind_copper < 0 || mail_detail.money_info.coupon < 0) {
		result = ERROR_CLIENT_PARAM;
	}

	Game_Player *receiver = GAME_MANAGER->find_role_id_game_player(receiver_id);
	if (receiver) {
		Mail_Info &mail_info = receiver->mail().mail_info();
		mail_info.total_count++;
		mail_detail.mail_id = mail_info.total_count + 1000000;
		mail_detail.send_time = Time_Value::gettimeofday().sec();
		mail_info.mail_map.insert(std::make_pair(mail_detail.mail_id, mail_detail));

		//邮件数量超过100，删除最后一封
		if (mail_info.mail_map.size() > 100) {
			for (Mail_Info::Mail_Map::iterator iter = mail_info.mail_map.begin();
					iter != mail_info.mail_map.end(); ++iter) {
				mail_info.mail_map.erase(iter);
				break;
			}
		}

		Block_Buffer buf;
		MSG_300200 msg;
		msg.reset();
		msg.mail_detail_vec.push_back(mail_detail);
		msg.serialize(buf);
		receiver->respond_success_result(ACTIVE_RECEIVE_MAIL, &buf);
	} else {
		Block_Buffer buf;
		buf.make_message(SYNC_GAME_DB_SAVE_MAIL_INFO);
		MSG_150004 msg;
		msg.role_id = receiver_id;
		msg.mail_detail = mail_detail;
		msg.serialize(buf);
		buf.finish_message();
		GAME_MANAGER->send_to_db(buf);
	}

	return result;
}
