var player_data = new Object();
player_data.player_info = new Player_Info();
player_data.mail_info = new Mail_Info();

function load_player_data(buffer) {
	if (buffer == null) {
		return;
	}
	
	player_data.player_info.deserialize(buffer);
	player_data.mail_info.deserialize(buffer);	
	print('------load_data,role_id:', player_data.player_info.role_id, " role_name:", player_data.player_info.role_name);
}