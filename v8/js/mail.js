function fetch_mail_info(player) {
	print('fetch_mail_info, role_id:', player.role_id(), " role_name:", player.role_name());
	var buf = pop_buf();
	buf.write_int32(1990);
	buf.write_string('zhangyalei');
	player.respond_success_result(msg_res.RES_FETCH_MAIL_INFO, buf);
	push_buf(buf);
}

function pickup_mail(player, buf) {
	print('pickup_mail, role_id:', player.role_id(), " role_name:", player.role_name());
}

function delete_mail(player, buf) {
	print('delete_mail, role_id:', player.role_id(), " role_name:", player.role_name());
}

function send_mail(player, buf) {
	print('send_mail, role_id:', player.role_id(), " role_name:", player.role_name());
}