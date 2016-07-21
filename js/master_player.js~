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
	
	this.cid = gate_cid * 10000 + player_cid;
	print('------master_player load_data, cid:', this.cid, ' role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
	
	this.cplayer = get_master_player_by_cid(gate_cid, player_cid);
	if(this.cplayer == null){
		print('master_player ', this.player_info.role_id, ' cid is ', this.cid, ' cplayer is null')
	}
	
	master_player_cid_map.insert(this.cid, this);
	master_player_role_id_map.insert(this.player_info.role_id, this);

	rank_manager.update_rank_level(this);
}
	
//玩家离线，保存数据
Master_Player.prototype.save_player_data = function() {
	print('------master_player leave save_data, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
	
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
		master_player_cid_map.each(function(key,value,index) {
			value.cplayer.respond_success_result(Msg_MC.RES_SEND_CHAT_INFO, buf);
   		});
		break;
	}
	case 2: {
		//私密聊天
		var player = get_master_player_by_name(msg_req.role_name);
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

Master_Player.prototype.sync_game_player_data = function(buffer) {
	print('sync game player data, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg = new MSG_165000();
	msg.deserialize(buffer);

	this.player_info.level = msg.level;

	rank_manager.update_rank_level(this);
}
