var count = 0;
while (1) {
	var buf = server.Pop_V8_Block();
	if (buf == null) {
		count++;
		if (count >= 20)
			break;
		server.Sleep();
		server.Print("buf is null");
		continue;
	}

	server.Print("buf right");
	var gate_cid = buf.read_int32();
	var len = buf.read_uint16();
	var msg_id = buf.read_uint32();
	var status =  buf.read_int32();
	var player_cid = buf.read_int32();
}