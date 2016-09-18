/*
*	描述：master_player信息类
*	作者：张亚磊
*	时间：2016/03/26
*/

function Master_Player() {
	this.gate_cid = 0;
	this.game_cid = 0;
	this.player_cid = 0;
	this.player_info = new Master_Player_Info();
}

Master_Player.prototype.set_gate_cid = function(gate_cid, player_cid, role_id) {
	print('***************gate sync master player login, gate_cid:', gate_cid, ' player_cid:', player_cid, ' role_id:', role_id);
	this.gate_cid = gate_cid;
	this.player_cid = player_cid;
	//设置master_player gate_cid
	set_master_player_gate_cid(gate_cid, player_cid, role_id);
	this.player_info.role_id = role_id;
	master_player_gate_cid_map.set(get_cid(gate_cid, player_cid), this);
	master_player_role_id_map.set(role_id, this);
}

//玩家上线，加载数据
Master_Player.prototype.load_player_data = function(game_cid, player_cid, player_info) {
	this.game_cid = game_cid;
	this.player_cid = player_cid;
	this.player_info = player_info;
	
	print('***************master_player load_data, role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
	//设置master_player game_cid
	set_master_player_game_cid(game_cid, player_cid, player_info.role_id);
	master_player_game_cid_map.set(get_cid(game_cid, player_cid), this);
	master_player_role_id_map.set(this.player_info.role_id, this);
	master_player_role_name_map.set(this.player_info.role_name, this);
	
	rank_manager.update_rank(Rank_Type.LEVEL_RANK, this);
	offline_manager.handle_offline_info(this);
}
	
//玩家离线，保存数据
Master_Player.prototype.save_player_data = function() {
	print('***************master_player save_data, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
	master_player_gate_cid_map.delete(get_cid(this.gate_cid, this.player_cid));
	master_player_game_cid_map.delete(get_cid(this.game_cid, this.player_cid));
	master_player_role_id_map.delete(this.player_info.role_id);
	master_player_role_name_map.delete(this.player_info.role_name);
}

Master_Player.prototype.tick = function(now) {
}

Master_Player.prototype.daily_refresh = function() {

}

Master_Player.prototype.send_success_msg = function(msg_id, msg) {
	send_master_msg_to_gate(this.gate_cid, this.player_cid, msg_id, 0, msg);
}

Master_Player.prototype.send_error_msg = function(msg_id, error_code) {
	send_master_msg_to_gate(this.gate_cid, this.player_cid, msg_id, error_code);
}

Master_Player.prototype.send_chat_info = function(obj) {
	print('send_chat_info, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());

	if (obj.chat_content.length > 100) {
		return this.send_error_msg(Msg.RES_SEND_CHAT_INFO, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var msg = new MSG_510001();
	msg.chat_type = obj.chat_type;
	msg.chat_content = obj.chat_content;
	msg.role_name = this.player_info.role_name;
	msg.gender = this.player_info.gender;
	msg.career = this.player_info.career;
	msg.vip_level = this.player_info.vip_level;
	switch(obj.chat_type) {
	case 1: {
		//世界聊天
		for (var value of master_player_gate_cid_map.values()) {
  			value.send_success_msg(Msg.RES_SEND_CHAT_INFO, msg);
		}
		break;
	}
	case 2: {
		//私密聊天
		var player = master_player_role_name_map.get(obj.role_name);
		if (!player) {
			return this.send_error_msg(Msg.RES_SEND_CHAT_INFO, Error_Code.ERROR_ROLE_OFFLINE);
		}
		player.send_success_msg(Msg.RES_SEND_CHAT_INFO, msg);
		break;
	}
	default:
		print("chat type error, role_id:", this.player_info.role_id, " chat_type:", obj.chat_type);
		break;
	}
}