require('error.js');
require('message.js');
require('player.js');
require('mail.js');

while (true) {
	var all_empty = true;
	var buffer = pop_buffer(true);
	if (buffer != null) {
		all_empty = false;
		process_client_buffer(buffer);
	}
	
	buffer = get_player_data_buffer();
	if (buffer != null) {
		all_empty = false;
		load_data(buffer);
	}
	
	if (all_empty) {
		sleep();
		continue;
	}
}

function process_client_buffer(buffer) {
	var gate_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	var player_cid = buffer.read_int32();
	
	if (msg_id == msg_req.REQ_FETCH_ROLE_INFO || msg_id == msg_req.REQ_CREATE_ROLE || msg_id == msg_req.SYNC_GATE_GAME_PLAYER_SIGNOUT) {
		process_login_buffer(buffer, gate_cid, player_cid, msg_id);
	} else {
		var player = get_player(gate_cid, player_cid);
		if (player) {
			print('find game player success, role_id:', player.role_id(), " role_name:", player.role_name());
			switch(msg_id) {
			case msg_req.REQ_FETCH_MAIL_INFO:
				fetch_mail_info(player);
				break;
			case msg_req.REQ_PICKUP_MAIL:
				pickup_mail(player, buffer);
				break;
			case msg_req.REQ_DEL_MAIL:
				delete_mail(player, buffer);
				break;
			case msg_req.REQ_SEND_MAIL:
				send_mail(player, buffer);
				break;
			default:
				break;
			}
		}
	}
	push_buffer(buffer, gate_cid);
}