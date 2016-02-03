require('error.js');
print(error.ERROR_CLIENT_SESSION);

while (1) {	
	var buf = pop_block();
	if (buf == null) {
		sleep();
		continue;	
	}
	
	var gate_cid = buf.read_int32();
	var len = buf.read_int16();
	var msg_id = buf.read_int32();
	var status = buf.read_int32();
	var player_cid = buf.read_int32();
	
	if (msg_id == 120001 || msg_id == 120002 || msg_id == 113000) {
		buf.process_login_block(gate_cid, player_cid, msg_id);
	} else {
		var player = get_player(gate_cid, player_cid);
		if (player) {
			
		}
	}
	
	buf.push_block(gate_cid);
}
