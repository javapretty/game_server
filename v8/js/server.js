require('error.js');
require('message.js');
require('player.js');
require('mail.js');

while (1) {	
	var buf = pop_buf(true);
	if (buf == null) {
		sleep();
		continue;	
	}
	
	var gate_cid = buf.read_int32();
	var len = buf.read_int16();
	var msg_id = buf.read_int32();
	var status = buf.read_int32();
	var player_cid = buf.read_int32();
	
	if (msg_id == msg_req.REQ_FETCH_ROLE_INFO || msg_id == msg_req.REQ_CREATE_ROLE || msg_id == msg_req.SYNC_GATE_GAME_PLAYER_SIGNOUT) {
		process_login_buf(buf, gate_cid, player_cid, msg_id);
	} else {
		var player = get_player(gate_cid, player_cid);
		if (player) {
			print('find game player success, role_id:', player.role_id(), " role_name:", player.role_name());
			switch(msg_id) {
			case msg_req.REQ_FETCH_MAIL_INFO:
				fetch_mail_info(player);
				break;
			case msg_req.REQ_PICKUP_MAIL:
				pickup_mail(player, buf);
				break;
			case msg_req.REQ_DEL_MAIL:
				delete_mail(player, buf);
				break;
			case msg_req.REQ_SEND_MAIL:
				send_mail(player, buf);
				break;
			default:
				break;
			}
		}
	}
	
	push_buf(buf, gate_cid);
}
