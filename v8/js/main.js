while (1) {	
	var buf = Pop_Block;
	if (buf == null) {
		Sleep();
		continue;	
	}
	
	var gate_cid = buf.read_int32();
	var len = buf.read_int16();
	var msg_id = buf.read_int32();
	var status = buf.read_int32();
	var player_cid = buf.read_int32();
	
	Print(msg_id);
	if (msg_id == 120001 || msg_id == 120002 || msg_id == 113000) {
		buf.Process_Login_Block(gate_cid, player_cid, msg_id);
	}
	buf.Push_Block(gate_cid);
}