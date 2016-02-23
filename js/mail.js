function fetch_mail_info(player) {
	print('fetch_mail_info, role_id:', player.player_data.player_info.role_id, " role_name:", player.player_data.player_info.role_name);
	var buffer = pop_buffer();
	buffer.write_uint16(player.player_data.mail_info.mail_map.size());
	player.player_data.mail_info.mail_map.each(function(key,value,index) {
		value.serialize(buffer);
    });
	player.cplayer.respond_success_result(msg_res.RES_FETCH_MAIL_INFO, buffer);
	push_buffer(buffer);
}

function pickup_mail(player, buffer) {
	
}

function delete_mail(player, buffer) {
	
}

function send_mail(player, buffer) {
	
}