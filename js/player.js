/*
*	描述：玩家系统
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function Player() {
	this.cid = 0;
	this.cplayer = null;
	this.player_info = new Player_Info();
	this.bag = new Bag();
	this.mail = new Mail();

	this.load_player_data = function(buffer) {
		if (buffer == null) {
			return;
		}
	
		var gate_cid = buffer.read_int32();
		var player_cid = buffer.read_int32();
		this.cid = gate_cid * 10000 + player_cid;
		this.cplayer = get_player_by_cid(gate_cid, player_cid);
		this.player_info.deserialize(buffer);
		this.bag.load_data(this, buffer);
		this.mail.load_data(this, buffer);
		print('------load_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
		
		player_cid_map.put(this.cid, this);
		player_role_id_map.put(this.player_info.role_id, this);
	}
	
	this.save_player_data = function() {
		var buffer = this.cplayer.player_data_buffer();
		if (buffer == null) {
			return;
		}
		
		this.player_info.serialize(buffer);
		this.bag.save_data(buffer);
		this.mail.save_data(buffer);
		print('------save_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
		
		player_cid_map.remove(this.cid);
		player_role_id_map.remove(this.player_info.role_id);
	}
}