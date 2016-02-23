require('error.js');
require('message.js');
require('map.js');
require('struct.js');
require('player.js');
require('mail.js');

var player_map = new Map();

while (true) {
	var all_empty = true;
	var buffer = pop_buffer(true);
	if (buffer != null) {
		all_empty = false;
		process_client_buffer(buffer);
	}
	
	buffer = get_player_data();
	if (buffer != null) {
		all_empty = false;
		var player = new Player();
		player.load_player_data(buffer);
		player_map.put(player.cid, player);
	}
	
	var cid = get_drop_player_cid();
	if (cid > 0) {
		all_empty = false;
		var player = player_map.get(cid);
		if (player) {
			player.save_player_data();
		}
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
		var cid = gate_cid * 10000 + player_cid;
		var player = player_map.get(cid);
		if (player) {
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
		} else {
			print('find game player wrong, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);
		}
	}
	push_buffer(buffer, gate_cid);
}