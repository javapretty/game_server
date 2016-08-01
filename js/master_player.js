/*
*	描述：master_player信息类
*	作者：张亚磊
*	时间：2016/03/26
*/

function Master_Player() {
	this.gate_cid = 0;
	this.game_cid = 0;
	this.player_cid = 0;
	this.cplayer = null;
	this.player_info = new Master_Player_Info();
}

Master_Player.prototype.set_gate_cid = function(gate_cid, player_cid, role_id) {
	print('***************gate sync master player login, gate_cid:', gate_cid, ' player_cid:', player_cid, ' role_id:', role_id);
	this.gate_cid = gate_cid;
	this.player_cid = player_cid;
	this.cplayer = get_master_player_by_gate_cid(gate_cid, player_cid, role_id);
	if(this.cplayer == null) {
		print('get master_player null, role_id:', this.player_info.role_id);
		master_close_client(gate_cid, player_cid, Error_Code.ERROR_CLIENT_PARAM);
		return;
	}
	master_player_gate_cid_map.insert(this.gate_cid * 10000 + player_cid, this);
	master_player_role_id_map.insert(role_id, this);
}

//玩家上线，加载数据
Master_Player.prototype.load_player_data = function(game_cid, player_cid, player_info) {
	this.game_cid = game_cid;
	this.player_cid = player_cid;
	this.player_info = player_info;
	
	print('***************master_player load_data, role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
	this.cplayer = get_master_player_by_game_cid(game_cid, player_cid, this.player_info.role_id);
	master_player_game_cid_map.insert(this.game_cid * 10000 + player_cid, this);
	master_player_role_id_map.insert(this.player_info.role_id, this);
	master_player_role_name_map.insert(this.player_info.role_name, this);

	rank_manager.update_rank_level(this);
}
	
//玩家离线，保存数据
Master_Player.prototype.save_player_data = function() {
	print('***************master_player save_data, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
	master_player_gate_cid_map.remove(this.gate_cid * 10000 + this.player_cid);
	master_player_game_cid_map.remove(this.game_cid * 10000 + this.player_cid);
	master_player_role_id_map.remove(this.player_info.role_id);
	master_player_role_name_map.remove(this.player_info.role_name);
}

Master_Player.prototype.tick = function(now) {
}

Master_Player.prototype.daily_refresh = function() {

}

Master_Player.prototype.send_chat_info = function(buffer) {
	print('send_chat_info, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());

	var msg_req = new MSG_110001();
	msg_req.deserialize(buffer);
	if (msg_req.chat_content.length > 100) {
		return this.cplayer.respond_error_result(Msg_MC.RES_SEND_CHAT_INFO, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var msg_res = new MSG_510001();
	msg_res.chat_type = msg_req.chat_type;
	msg_res.chat_content = msg_req.chat_content;
	msg_res.role_name = this.player_info.role_name;
	msg_res.gender = this.player_info.gender;
	msg_res.career = this.player_info.career;
	msg_res.vip_level = this.player_info.vip_level;
	
	var buf = pop_master_buffer();
	msg_res.serialize(buf);
	
	switch(msg_req.chat_type) {
	case 1: {
		//世界聊天
		master_player_gate_cid_map.each(function(key,value,index) {
			value.cplayer.respond_success_result(Msg_MC.RES_SEND_CHAT_INFO, buf);
   		});
		break;
	}
	case 2: {
		//私密聊天
		var player = master_player_role_name_map.get(msg_req.role_name);
		if (!player) {
			return this.cplayer.respond_error_result(Msg_MC.RES_SEND_CHAT_INFO, Error_Code.ERROR_ROLE_OFFLINE);
		}
		player.cplayer.respond_success_result(Msg_MC.RES_SEND_CHAT_INFO, buf);
		break;
	}
	default:
		print("chat type error, role_id:", this.player_info.role_id, " chat_type:", msg_req.chat_type);
		break;
	}
	push_master_buffer(buf);
}