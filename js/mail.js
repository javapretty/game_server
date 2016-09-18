/*
*	描述：邮件系统
*	作者：张亚磊
*	时间：2016/02/24
*/

function Mail() {
	this.game_player = null;
	this.mail_info = new Mail_Info();
}

Mail.prototype.load_data = function(game_player, obj) {
	this.game_player = game_player;
	this.mail_info = obj.player_data.mail_info;
}
	
Mail.prototype.save_data = function(obj) {
	obj.player_data.mail_info = this.mail_info;
}

Mail.prototype.fetch_mail_info = function() {
	print('fetch_mail_info, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var msg = new MSG_520200();
	for (var value of this.mail_info.mail_map.values()) {
  		msg.mail_info.push(value);
	}
  this.game_player.send_success_msg(Msg.RES_FETCH_MAIL_INFO, msg);
}

Mail.prototype.pickup_mail = function(obj) {
	print('pickup_mail, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var msg = new MSG_520201();
	if (obj.mail_id == 0) {
		this.mail_info.mail_map.forEach(function(value, key, map) {
			var result = this.pickup_item_money(value);
			if (result == 0) {
				msg.mail_id_info.push(key);
			}
    	});
	} else {
		var mail_detail = this.mail_info.mail_map.get(obj.mail_id);
		if (mail_detail == null) {
			return this.game_player.send_error_msg(Msg.RES_PICKUP_MAIL, Error_Code.ERROR_CLIENT_PARAM);
		}
		
		var result = this.pickup_item_money(value);
		if (result == 0) {
			msg.mail_id_info.push(obj.mail_id);
		}
	}
	this.game_player.send_success_msg(Msg.RES_PICKUP_MAIL, msg);
}

Mail.prototype.delete_mail = function(obj) {
	print('delete_mail, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var msg = new MSG_520202();
	if (obj.mail_id == 0) {
		this.mail_info.mail_map.forEach(function(value, key, map) {
			var result = this.pickup_item_money(value);
			if (result == 0) {
				msg.mail_id_info.push(key);
			}
    	});
    	this.mail_info.mail_map.clear();
	} else {
		var mail_detail = this.mail_info.mail_map.get(obj.mail_id);
		if (mail_detail == null) {
			return this.game_player.send_error_msg(Msg.RES_DEL_MAIL, Error_Code.ERROR_CLIENT_PARAM);
		}
		
		var result = this.pickup_item_money(value);
		if (result == 0) {
			msg.mail_id_info.push(obj.mail_id);
			this.mail_info.mail_map.delete(obj.mail_id);
		}
	}
	this.game_player.send_success_msg(Msg.RES_DEL_MAIL, msg);
}

Mail.prototype.send_mail = function(obj) {
	print('send_mail, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var receiver = game_player_role_name_map.get(obj.receiver_name);
	if (receiver == null) {
		return this.game_player.send_error_msg(Msg.RES_SEND_MAIL, Error_Code.ERROR_ROLE_NOT_EXIST);
	}
	var receiver_id = receiver.role_id();
	if (receiver_id == this.game_player.player_info.role_id || obj.mail_detail.mail_title.length > 64 || obj.mail_detail.mail_content.length > 512) {
		return this.game_player.send_error_msg(Msg.RES_SEND_MAIL, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var result = this.game_player.bag.bag_sub_money(obj.mail_detail.copper, obj.mail_detail.gold);
	if (result != 0) {
		return this.game_player.send_error_msg(Msg.RES_SEND_MAIL, result);
	}

	var result = this.send_mail_inner(receiver_id, obj.mail_detail);
	this.game_player.send_error_msg(Msg.RES_SEND_MAIL, result);
}
	
Mail.prototype.pickup_item_money = function(mail_detail) {	
	var result = this.game_player.bag.bag_insert_item(mail_detail.item_info);
	if (result != 0) {
		return result;
	}
	
	result = this.game_player.bag.bag_add_money(mail_detail.copper, mail_detail.gold);
	if (result != 0) {
		return result;
	}
	
	mail_detail.pickup = true;
	return 0;
}

Mail.prototype.send_mail_inner = function(receiver_id, mail_detail) {
	if (receiver_id <= 0 || mail_detail.sender_type <= 0 || mail_detail.sender_id <= 0 || mail_detail.sender_name.length <= 0
		|| mail_detail.mail_title.length <= 0 || mail_detail.gold < 0 || mail_detail.copper < 0) {
		return Error_Code.ERROR_CLIENT_PARAM;
	}

	//玩家在线才能发邮件，离线暂时不处理
	var receiver = game_player_role_id_map.get(receiver_id);
	if (receiver) {	
		var mail_info = receiver.mail.mail_info;
		mail_info.total_count++;
		mail_detail.mail_id = mail_info.total_count + 1000000;
		mail_detail.send_time = sec();
		mail_info.mail_map.set(mail_detail.mail_id, mail_detail);

		//邮件数量超过100，删除最后一封
		if (mail_info.mail_map.size() > 100) {
			mail_info.mail_map.pop_front();
		}

		var msg = new MSG_300200();
		msg.mail_info.push(mail_detail);
		receiver.send_success_msg(Msg.RES_BUY_VITALITY, msg);
		receiver.set_data_change();
	}
}
