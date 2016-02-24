/*
*	描述：玩家功能类
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function Player() {
	this.cid = 0;
	this.cplayer = null;
	this.player_data = new Object();
	this.player_data.player_info = new Player_Info();
	this.player_data.mail_info = new Mail_Info();

	this.load_player_data = function(buffer) {
		if (buffer == null) {
			return;
		}
	
		var gate_cid = buffer.read_int32();
		var player_cid = buffer.read_int32();
		this.cid = gate_cid * 10000 + player_cid;
		this.cplayer = get_player_by_cid(gate_cid, player_cid);
		this.player_data.player_info.deserialize(buffer);
		this.player_data.mail_info.deserialize(buffer);	
		print('------load_data,role_id:', this.player_data.player_info.role_id, " role_name:", this.player_data.player_info.role_name);
	}
	
	this.save_player_data = function() {
		var buffer = this.cplayer.player_data_buffer();
		if (buffer == null) {
			return;
		}
		this.player_data.player_info.serialize(buffer);
		this.player_data.mail_info.serialize(buffer);
		print('------save_data,role_id:', this.player_data.player_info.role_id, " role_name:", this.player_data.player_info.role_name);
	}
}