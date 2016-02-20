var player_data = null;

function load_data(buf) {
	if (buf == null) 
		return ;
		
	var role_id = buf.read_int64();
	var total_count = buf.read_int32();
	print('load_data,role_id:', role_id, " total_count:", total_count);
}

function save_data(buf) {

}