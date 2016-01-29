while (1) {
	var buf = new server.Block_Buffer();
	buf = server.Pop_V8_Block();
	if (buf == null) {
		server.Sleep();
		continue;
	}

	server.Print("buf is right");
	var gate_cid = buf.read_int32();
	var len = buf.read_uint16();
	var msg_id = buf.read_uint32();
	var status =  buf.read_int32();
	var player_cid = buf.read_int32();
	server.Print(gate_cid);
	server.Print(len);
	server.Print(msg_id);
	server.Print(status);
	server.Print(player_cid);	
	
	server.Push_Game_Gate_Server_Block(gate_cid, buf);
}