while (1) {	
	var buf = Pop_Block();
	if (buf == null) {
		Sleep();
		continue;	
	}
	
	var gate_cid = buf.Read_Int32();
	var len = buf.Read_Int16();
	var msg_id = buf.Read_Int32();
	var status = buf.Read_Int32();
	var player_cid = buf.Read_Int32();
	
	if (msg_id == 120001 || msg_id == 120002 || msg_id == 113000) {
		buf.Process_Login_Block(gate_cid, player_cid, msg_id);
	} else {
		var player = Get_Player(gate_cid, player_cid);
		if (player) {
			
		}
	}
	
	buf.Push_Block(gate_cid);
}