var player_data = null;

function load_data(buffer) {
	if (buffer == null) {
		return;
	}
	var role_id = buffer.read_int64();
	var total_count = buffer.read_int32();
	print('---------------load_data,role_id:', role_id, " total_count:", total_count);
}

function save_data(buffer) {
}