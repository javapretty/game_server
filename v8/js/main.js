while (1) {	
	var buf = Pop_Block;
	if (buf == null) {
		Sleep();
		continue;	
	}
	
	Print("server is right");
	var gate_cid = buf.read_int32();
	var len = buf.read_int16();
	var msg_id = buf.read_int32();
	var status = buf.read_int32();
	var player_cid = buf.read_int32();
	
	Print(gate_cid);
	Print(len);
	Print(msg_id);
	Print(status);
	Print(player_cid);
	
	buf.Push_Block(gate_cid);
}