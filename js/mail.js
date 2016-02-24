function pickup_mail_money(mail_detail) {

	mail_detail.pickup = 1;	
}

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
			pickup_mail_money(value);
			mail_array.push(key);
    	});
	} else {
		var mail_detail = player.player_data.mail_info.mail_map.get(mail_id);
		if (mail_detail == null) {
			player.cplayer.respond_error_result(msg_res.RES_PICKUP_MAIL, error.ERROR_CLIENT_PARAM);
		}
		pickup_mail_money(mail_detail);
		mail_array.push(mail_id);
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
			pickup_mail_money(value);
			mail_array.push(key);
    	});
    	player.player_data.mail_info.mail_map.clear();
	} else {
		var mail_detail = player.player_data.mail_info.mail_map.get(mail_id);
		if (mail_detail == null) {
			player.cplayer.respond_error_result(msg_res.RES_DEL_MAIL, error.ERROR_CLIENT_PARAM);
		}
		pickup_mail_money(mail_detail);
		mail_array.push(mail_id);
		player.player_data.mail_info.mail_map.remove(mail_id);
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
	var buffer = pop_buffer();
	
	player.cplayer.respond_success_result(msg_res.RES_SEND_MAIL, buffer);
	push_buffer(buffer);
}