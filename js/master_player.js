/*
*	描述：master_player信息类
*	作者：张亚磊
*	时间：2016/03/26
*/

function Master_Player() {
	this.sync_player_data_tick = util.now_sec();
	this.cid = 0;
	this.cplayer = null;
	this.player_info = new Master_Player_Info();
}

//玩家上线，加载数据
Master_Player.prototype.load_player_data = function(buffer) {
	if (buffer == null) {
		return;
	}
	
	var gate_cid = buffer.read_int32();
	var player_cid = buffer.read_int32();
	this.player_info.deserialize(buffer);
	
	print('------master_player load_data, role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
	this.cid = gate_cid * 10000 + player_cid;
	this.cplayer = get_master_player_by_cid(gate_cid, player_cid);
	
	master_player_cid_map.insert(this.cid, this);
	master_player_role_id_map.insert(this.player_info.role_id, this);
}
	
//玩家离线，保存数据
Master_Player.prototype.save_player_data = function() {
	print('------master_player save_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
	
	this.sync_player_data();
	master_player_cid_map.remove(this.cid);
	master_player_role_id_map.remove(this.player_info.role_id);
}

Master_Player.prototype.sync_player_data = function() {
	var buffer = this.cplayer.get_save_data_buffer();
	if (buffer == null) {
		return;
	}
	
	this.player_info.serialize(buffer);
}

Master_Player.prototype.tick = function(now) {
	//同步玩家数据到C++,15s一次
	if (now - this.sync_player_data_tick >= 15) {
		this.sync_player_data();
		this.sync_player_data_tick = now;
	}
}

Master_Player.prototype.daily_refresh = function() {

}