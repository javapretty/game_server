/*
*	描述：邮件系统
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function Mail() {
	this.player = null;
	this.mail_info = new Mail_Info();
	
	this.load_data = function(player, buffer) {
		this.player = player;
		this.mail_info.deserialize(buffer);
	}
	
	this.save_data = function(buffer) {
		this.mail_info.serialize(buffer);
	}
	
	this.set_data_change = function() {
		this.player.cplayer.set_player_data_change(Data_Change.MAIL_CHANGE);
	}

	this.fetch_mail_info = function() {
		print('fetch_mail_info, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
		
		var msg_res = new MSG_520200();
		this.mail_info.mail_map.each(function(key,value,index) {
			msg_res.mail_info.push(value);
    	});
    	
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_FETCH_MAIL_INFO, buf);
		push_buffer(buf);
}

	this.pickup_mail = function(buffer) {
		print('pickup_mail, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
		var msg_req = new MSG_120201();
		msg_req.deserialize(buffer);
		
		var msg_res = new MSG_520201();
		if (msg_req.mail_id == 0) {
			this.mail_info.mail_map.each(function(key,value,index) {
				var result = this.pickup_item_money(value);
				if (result == 0) {
					msg_res.mail_id_info.push(key);
				}
    		});
		} else {
			var mail_detail = this.mail_info.mail_map.get(msg_req.mail_id);
			if (mail_detail == null) {
				return this.player.cplayer.respond_error_result(Msg_Res.RES_PICKUP_MAIL, Error_Code.ERROR_CLIENT_PARAM);
			}
			
			var result = this.pickup_item_money(value);
			if (result == 0) {
					msg_res.mail_id_info.push(msg_req.mail_id);
			}
		}
	
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_PICKUP_MAIL, buf);
		push_buffer(buf);
		
		this.set_data_change();
	}

	this.delete_mail = function(buffer) {
		print('delete_mail, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
		var msg_req = new MSG_120202();
		msg_req.deserialize(buffer);
		
		var msg_res = new MSG_520202();
		if (msg_req.mail_id == 0) {
			this.mail_info.mail_map.each(function(key,value,index) {
				var result = this.pickup_item_money(value);
				if (result == 0) {
					msg_res.mail_id_info.push(key);
				}
    		});
    		this.mail_info.mail_map.clear();
		} else {
			var mail_detail = this.mail_info.mail_map.get(msg_req.mail_id);
			if (mail_detail == null) {
				return this.player.cplayer.respond_error_result(Msg_Res.RES_DEL_MAIL, Error_Code.ERROR_CLIENT_PARAM);
			}
			
			var result = this.pickup_item_money(value);
			if (result == 0) {
					msg_res.mail_id_info.push(msg_req.mail_id);
					this.mail_info.mail_map.remove(msg_req.mail_id);
			}
		}
	
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_DEL_MAIL, buf);
		push_buffer(buf);
		
		this.set_data_change();
	}

	this.send_mail = function(buffer) {
		print('send_mail, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
		
		var msg_req = new MSG_120203();
		msg_req.deserialize(buffer);	
		var receiver = get_player_by_name(msg_req.receiver_name);
		if (receiver == null) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_SEND_MAIL, Error_Code.ERROR_ROLE_NOT_EXIST);
		}
		var receiver_id = receiver.role_id();
		if (receiver_id == this.player.player_info.role_id || msg_req.mail_detail.mail_title.length > 64 || msg_req.mail_detail.mail_content.length > 512)
			return this.player.cplayer.respond_error_result(Msg_Res.RES_SEND_MAIL, Error_Code.ERROR_CLIENT_PARAM);
	
		var result = this.player.bag.bag_add_money(msg_req.mail_detail.copper, msg_req.mail_detail.gold);
		if (result != 0) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_SEND_MAIL, result);
		}

		var result = send_mail_inner(receiver_id, msg_req.mail_detail);
		this.player.cplayer.respond_error_result(Msg_Res.RES_SEND_MAIL, result);
	}
	
	this.pickup_item_money = function(mail_detail) {		
		var result = this.player.bag.bag_insert_item(mail_detail.item_info);
		if (result != 0) {
			return result;
		}
		
		result = this.player.bag.bag_add_money(mail_detail.copper, mail_detail.gold);
		if (result != 0) {
			return result;
		}
		
		mail_detail.pickup = true;
		return 0;
	}
}

function send_mail_inner(receiver_id, mail_detail) {
		if (receiver_id <= 0 || mail_detail.sender_type <= 0 || mail_detail.sender_id <= 0 || mail_detail.sender_name.empty()
			|| mail_detail.mail_title.empty() || mail_detail.gold < 0 || mail_detail.copper < 0) {
			return Error_Code.ERROR_CLIENT_PARAM;
		}

		var receiver = player_role_id_map.get(receiver_id);
		if (receiver != null) {
			var mail_info = receiver.mail.mail_info;
			mail_info.total_count++;
			mail_detail.mail_id = mail_info.total_count + 1000000;
			mail_detail.send_time = sec();
			mail_info.mail_map.put(mail_detail.mail_id, mail_detail);

			//邮件数量超过100，删除最后一封
			if (mail_info.mail_map.size() > 100) {
				mail_info.mail_map.pop_front();
			}

			var msg_active = new MSG_300200();
			msg_active.mail_info.push(mail_detail);
			var buf = pop_buffer();
			msg_active.serialize(buf);
			receiver.cplayer.respond_success_result(Msg_Active.ACTIVE_MAIL_INFO, buf);
			push_buffer(buf);
			
			receiver.mail.set_data_change();
		} else {
			var buf = pop_buffer();
			buf.make_inner_message(Msg_Db.SYNC_GAME_DB_SAVE_MAIL_INFO);
			buf.write_int64(receiver_id);
			mail_detail.serialize(buf);
			buf.finish_message();
			send_msg_to_db(buf);
			push_buffer(buf);
		}
	}