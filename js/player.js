var player_data = new Object();
player_data.mail_info = new Mail_Info();

function load_data(buffer) {
	if (buffer == null) {
		return;
	}
	
	player_data.mail_info.deserialize(buffer);	
	print('------load_data,role_id:', player_data.mail_info.role_id, " total_count:", player_data.mail_info.total_count, " is_change:", player_data.mail_info.is_change);
}