/*
*	描述：邮件系统
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function fetch_mail_info(player) {
	print('fetch_mail_info, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name, " msec:", msec());
	
	var buf = pop_buffer();
	buf.write_uint16(player.player_data.mail_info.mail_map.size());
	player.player_data.mail_info.mail_map.each(function(key,value,index) {
		value.serialize(buf);
    });
	player.cplayer.respond_success_result(msg_res.RES_FETCH_MAIL_INFO, buf);
	push_buffer(buf);
}

function pickup_mail(player, buffer) {
	print('pickup_mail, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name, " msec:", msec());
	
	var mail_id = buffer.read_int32();
	var mail_array = new Array();
	if (mail_id == 0) {
		player.player_data.mail_info.mail_map.each(function(key,value,index) {
			var result = bag_add_money(value.copper, value.gold);
			if (result == 0) {
				mail_array.push(key);
				value.pickup = true;
			}
    	});
	} else {
		var mail_detail = player.player_data.mail_info.mail_map.get(mail_id);
		if (mail_detail == null) {
			return player.cplayer.respond_error_result(msg_res.RES_PICKUP_MAIL, error.ERROR_CLIENT_PARAM);
		}
		var result = bag_add_money(mail_detail.copper, mail_detail.gold);
		if (result == 0) {
			mail_array.push(key);
			mail_detail.pickup = true;
		}
	}
	
	var buf = pop_buffer();
	buf.write_uint16(mail_array.length);
	for (var i = 0; i < mail_array.length; ++i) {
		buffer.write_int32(mail_array[i]);
	}
	player.cplayer.respond_success_result(msg_res.RES_PICKUP_MAIL, buf);
	push_buffer(buffer);
	player.player_data.mail_info.save_change();
}

function delete_mail(player, buffer) {
	print('delete_mail, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name, " msec:", msec());
	
	var mail_id = buffer.read_int32();
	var mail_array = new Array();
	if (mail_id == 0) {
		player.player_data.mail_info.mail_map.each(function(key,value,index) {
			var result = bag_add_money(value.copper, value.gold);
			if (result == 0) {
				mail_array.push(key);
				value.pickup = true;
			}
    	});
    	player.player_data.mail_info.mail_map.clear();
	} else {
		var mail_detail = player.player_data.mail_info.mail_map.get(mail_id);
		if (mail_detail == null) {
			return player.cplayer.respond_error_result(msg_res.RES_DEL_MAIL, error.ERROR_CLIENT_PARAM);
		}
		var result = bag_add_money(mail_detail.copper, mail_detail.gold);
		if (result == 0) {
			mail_array.push(key);
			mail_detail.pickup = true;
			player.player_data.mail_info.mail_map.remove(mail_id);
		}
	}
	
	var buf = pop_buffer();
	buf.write_uint16(mail_array.length);
	for (var i = 0; i < mail_array.length; ++i) {
		buffer.write_int32(mail_array[i]);
	}
	player.cplayer.respond_success_result(msg_res.RES_DEL_MAIL, buf);
	push_buffer(buffer);
	player.player_data.mail_info.save_change();
}

function send_mail(player, buffer) {
	print('fetch_mail_info, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name, " msec:", msec());
	
	var receiver_name = buffer.read_string();
	var mail_detail = new Mail_Detail();
	mail_detail.deserialize(buffer);
	var receiver = get_player_by_name(receiver_name);
	if (receiver == null) {
		return player.cplayer.respond_error_result(msg_res.RES_SEND_MAIL, error.ERROR_ROLE_NOT_EXIST);
	}
	var receiver_id = receiver.role_id();
	if (receiver_id == player.player_data.player_info.role_id || mail_detail.mail_title.length > 64 || mail_detail.mail_content.length > 512)
		return player.cplayer.respond_error_result(msg_res.RES_SEND_MAIL, error.ERROR_CLIENT_PARAM);
	
	var result = bag_add_money(mail_detail.copper, mail_detail.gold);
	if (result != 0) {
		return player.cplayer.respond_error_result(msg_res.RES_SEND_MAIL, result);
	}

	var result = send_mail_inner(receiver_id, mail_detail);
	player.cplayer.respond_error_result(msg_res.RES_SEND_MAIL, result);
}

function send_mail_inner(receiver_id, mail_detail) {
	//参数验证
	if (receiver_id <= 0 || mail_detail.sender_type <= 0 || mail_detail.sender_id <= 0 || mail_detail.sender_name.empty()
			|| mail_detail.mail_title.empty() || mail_detail.gold < 0 || mail_detail.copper < 0) {
		return error.ERROR_CLIENT_PARAM;
	}

	var receiver = player_role_id_map.get(receiver_id);
	if (receiver != null) {
		var mail_info = receiver.player_data.mail_info;
		mail_info.total_count++;
		mail_detail.mail_id = mail_info.total_count + 1000000;
		mail_detail.send_time = sec();
		mail_info.mail_map.put(mail_detail.mail_id, mail_detail);

		//邮件数量超过100，删除最后一封
		if (mail_info.mail_map.size() > 100) {
			mail_info.mail_map.pop_front();
		}

		var buf = pop_buffer();
		buf.write_uint16(1);
		mail_detail.serialize(buf);
		receiver.cplayer.respond_success_result(msg_active.ACTIVE_RECEIVE_MAIL, buf);
		push_buffer(buf);
	} else {
		var buf = pop_buffer();
		buf.make_inner_message(SYNC_GAME_DB_SAVE_MAIL_INFO);
		buf.write_int64(receiver_id);
		mail_detail.serialize(buf);
		buf.finish_message();
		send_buffer_to_db(buf);
		push_buffer(buf);
	}
}