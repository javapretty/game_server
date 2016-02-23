function fetch_mail_info(player) {
	print('fetch_mail_info, role_id:', player.role_id(), " role_name:", player.role_name());
	var buffer = pop_buffer();
	buffer.write_uint16(player_data.mail_info.mail_map.size());
	player_data.mail_info.mail_map.each(function(key,value,index) {
		value.serialize(buffer);
    });
	player.respond_success_result(msg_res.RES_FETCH_MAIL_INFO, buffer);
	push_buffer(buffer);
}

function pickup_mail(player, buffer) {
	print('pickup_mail, role_id:', player.role_id(), " role_name:", player.role_name());
}

function delete_mail(player, buffer) {
	print('delete_mail, role_id:', player.role_id(), " role_name:", player.role_name());
}

function send_mail(player, buffer) {
	print('send_mail, role_id:', player.role_id(), " role_name:", player.role_name());
}