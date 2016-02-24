/*
*	描述：邮件系统
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function fetch_mail_info(player) {
	print('fetch_mail_info, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name);
	var buf = pop_buffer();
	buf.write_uint16(player.player_data.mail_info.mail_map.size());
	player.player_data.mail_info.mail_map.each(function(key,value,index) {
		value.serialize(buf);
    });
	player.cplayer.respond_success_result(msg_res.RES_FETCH_MAIL_INFO, buf);
	push_buffer(buf);
}

function pickup_mail(player, buffer) {
	print('pickup_mail, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name);
	var mail_id = buffer.read_int32();
	var mail_array = new Array();
	if (mail_id == 0) {
		player.player_data.mail_info.mail_map.each(function(key,value,index) {
			var result = player.cplayer.bag_add_money(value.bind_copper, value.copper, value.bind_gold, value.gold);
			if (result == 0) {
				mail_array.push(key);
				value.pickup = 1;
			}
    	});
	} else {
		var mail_detail = player.player_data.mail_info.mail_map.get(mail_id);
		if (mail_detail == null) {
			return player.cplayer.respond_error_result(msg_res.RES_PICKUP_MAIL, error.ERROR_CLIENT_PARAM);
		}
		var result = player.cplayer.bag_add_money(mail_detail.bind_copper, mail_detail.copper, mail_detail.bind_gold, mail_detail.gold);
		if (result == 0) {
			mail_array.push(key);
			mail_detail.pickup = 1;
		}
	}
	
	var buf = pop_buffer();
	buf.write_uint16(mail_array.length);
	for (var i = 0; i < mail_array.length; ++i) {
		buffer.write_int32(mail_array[i]);
	}
	player.cplayer.respond_success_result(msg_res.RES_PICKUP_MAIL, buffer);
	push_buffer(buffer);
	player.player_data.mail_info.save_change();
}

function delete_mail(player, buffer) {
	print('delete_mail, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name);
	var mail_id = buffer.read_int32();
	var mail_array = new Array();
	if (mail_id == 0) {
		player.player_data.mail_info.mail_map.each(function(key,value,index) {
			var result = player.cplayer.bag_add_money(value.bind_copper, value.copper, value.bind_gold, value.gold);
			if (result == 0) {
				mail_array.push(key);
				value.pickup = 1;
			}
    	});
    	player.player_data.mail_info.mail_map.clear();
	} else {
		var mail_detail = player.player_data.mail_info.mail_map.get(mail_id);
		if (mail_detail == null) {
			return player.cplayer.respond_error_result(msg_res.RES_DEL_MAIL, error.ERROR_CLIENT_PARAM);
		}
		var result = player.cplayer.bag_add_money(mail_detail.bind_copper, mail_detail.copper, mail_detail.bind_gold, mail_detail.gold);
		if (result == 0) {
			mail_array.push(key);
			mail_detail.pickup = 1;
			player.player_data.mail_info.mail_map.remove(mail_id);
		}
	}
	
	var buf = pop_buffer();
	buf.write_uint16(mail_array.length);
	for (var i = 0; i < mail_array.length; ++i) {
		buffer.write_int32(mail_array[i]);
	}
	player.cplayer.respond_success_result(msg_res.RES_DEL_MAIL, buffer);
	push_buffer(buffer);
	player.player_data.mail_info.save_change();
}

function send_mail(player, buffer) {
	print('send_mail, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name);
	
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
	
	var result = player.cplayer.bag_add_money(mail_detail.bind_copper, mail_detail.copper, mail_detail.bind_gold, mail_detail.gold);
	if (result != 0) {
		return player.cplayer.respond_error_result(msg_res.RES_SEND_MAIL, result);
	}

	player.cplayer.send_mail(receiver_id, mail_detail);
	player.cplayer.respond_success_result(msg_res.RES_SEND_MAIL, null);
}