
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
		result = player_->pickup_mail(iter->second);
		if (result == 0) {
			res_msg.mail_id_vec.push_back(msg.mail_id);
		}
	} else {
		for (Mail_Info::Mail_Map::iterator iter = mail_info_.mail_map.begin();
					iter != mail_info_.mail_map.end(); ++iter) {
			result = player_->pickup_mail(iter->second);
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
		result = player_->pickup_mail(iter->second);
		if (result == 0) {
			res_msg.mail_id_vec.push_back(msg.mail_id);
		}
		mail_info_.mail_map.erase(iter);
	} else {
		for (Mail_Info::Mail_Map::iterator iter = mail_info_.mail_map.begin();
				iter != mail_info_.mail_map.end();) {
			result = player_->pickup_mail(iter->second);
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

	player_->send_mail(receiver_id, msg.mail_detail);

	return player_->respond_success_result(RES_SEND_MAIL);
}
