/*
*	描述：game_player信息类
*	作者：张亚磊
*	时间：2016/02/24
*/

function Game_Player() {
	this.sync_player_data_tick = util.now_sec();
	this.gate_cid = 0;
	this.player_cid = 0;
	this.centity = null;
	this.is_change = false;
	this.player_info = new Game_Player_Info();
	this.bag = new Bag();
	this.mail = new Mail();
}

//玩家上线，加载数据
Game_Player.prototype.load_player_data = function(gate_cid, player_cid, obj) {
	print('***************game_player load_data, role_id:', obj.player_data.player_info.role_id, ' role_name:', obj.player_data.player_info.role_name);
	this.gate_cid = gate_cid;
	this.player_cid = player_cid; 
	this.player_info = obj.player_data.player_info;
	this.bag.load_data(this, obj);
	this.mail.load_data(this, obj);

	//设置game_player gate_cid
	set_game_player_gate_cid(gate_cid, player_cid);
	this.centity = get_scene_entity_by_gate_cid(gate_cid, player_cid);
	if(this.player_info.scene_id == 0) this.player_info.scene_id = 11001;
	this.enter_scene(this.player_info.scene_id, this.player_info.pos_x, this.player_info.pos_y, this.player_info.pos_z);
	
	this.sync_login_to_client();
	this.sync_login_to_master();
	game_player_cid_map.set(get_cid(this.gate_cid, this.player_cid), this);
	game_player_role_id_map.set(this.player_info.role_id, this);
	game_player_role_name_map.set(this.player_info.role_name, this);
}

//玩家离线，保存数据
Game_Player.prototype.save_player_data = function() {
	print('***************game_player save_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
	this.player_info.logout_time = util.now_sec();
	this.player_info.scene_id = this.centity.scene_id;
	this.player_info.pos_x = this.centity.x;
	this.player_info.pos_y = this.centity.y;
	this.player_info.pos_z = this.centity.z;
	
	this.sync_player_data_to_db(true);
	this.sync_logout_to_log();
	this.leave_scene();
	this.centity.reclaim();
	
	logout_map.set(this.player_info.account, this.player_info.logout_time);
	game_player_cid_map.delete(get_cid(this.gate_cid, this.player_cid));
	game_player_role_id_map.delete(this.player_info.role_id);
	game_player_role_name_map.delete(this.player_info.role_name);
}

Game_Player.prototype.sync_player_data_to_db = function(logout) {
	print('***************sync_player_data_to_db, logout:', logout, ' role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
	var msg = new MSG_150003();
	msg.logout = logout;
	msg.account = this.player_info.account;
	msg.player_data.player_info = this.player_info;
	this.bag.save_data(msg);
	this.mail.save_data(msg);
	send_game_msg_to_db(Msg.SYNC_GAME_DB_SAVE_PLAYER, msg);

	this.is_change = false;
}

Game_Player.prototype.set_data_change = function() {
	this.is_change = true;
}

Game_Player.prototype.tick = function(now) {
	//同步玩家数据到数据库
	if(this.is_change){
		if (now - this.sync_player_data_tick >= 15) {
			this.sync_player_data_to_db(false);
			this.sync_player_data_tick = now;
		}
	}
}

Game_Player.prototype.daily_refresh = function() { }

Game_Player.prototype.send_success_msg = function(msg_id, msg) {
	send_game_msg_to_gate(this.gate_cid, this.player_cid, msg_id, 0, msg);
	this.set_data_change();
}

Game_Player.prototype.send_error_msg = function(msg_id, error_code) {
	send_game_msg_to_gate(this.gate_cid, this.player_cid, msg_id, error_code);
}

Game_Player.prototype.sync_login_to_client = function() {
	var msg = new MSG_520001();
	msg.role_info.role_id = this.player_info.role_id;
	msg.role_info.account = this.player_info.account;
	msg.role_info.role_name = this.player_info.role_name;
	msg.role_info.level = this.player_info.level;
	msg.role_info.exp = this.player_info.exp;
	msg.role_info.career = this.player_info.career;
	msg.role_info.gender = this.player_info.gender;
	msg.role_info.vip_level = this.player_info.vip_level;
	msg.role_info.vip_exp = this.player_info.vip_exp;
	msg.role_info.charge_gold = this.player_info.charge_gold;
	this.send_success_msg(Msg.RES_FETCH_ROLE_INFO, msg);
}

Game_Player.prototype.sync_login_to_master = function() {
	var msg = new MSG_160001();
	msg.player_info.role_id = this.player_info.role_id;
	msg.player_info.account = this.player_info.account;
	msg.player_info.role_name = this.player_info.role_name;
	msg.player_info.level = this.player_info.level;
	msg.player_info.gender = this.player_info.gender;
	msg.player_info.career = this.player_info.career;
	send_game_msg_to_master(this.player_cid, Msg.SYNC_GAME_MASTER_PLYAER_LOGIN, 0, msg);
}

Game_Player.prototype.sync_logout_to_log = function() {
	var msg = new MSG_170000();
	msg.role_id = this.player_info.role_id;
	msg.role_name = this.player_info.role_name;
	msg.account = this.player_info.account;
	msg.level = this.player_info.level;
	msg.client_ip = this.player_info.client_ip;
	msg.login_time = this.player_info.login_time;
	msg.logout_time = this.player_info.logout_time;
	send_game_msg_to_log(Msg.SYNC_LOG_LOGINOUT, msg);
}

Game_Player.prototype.add_exp = function(exp) {
	print('add_exp, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " exp:", exp);
	
	if (exp <= 0) {
		return this.send_error_msg(Msg.ACTIVE_PLAYER_INFO, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	//经验增加升级
	this.player_info.exp += exp;
	var max_player_level = config.util_json.max_player_level;
	for (var i = this.player_info.level; i < max_player_level; ++i) {
		var level_exp = config.level_json[i].level_exp;
		if (this.player_info.exp < level_exp) 
			break;
		
		this.player_info.level++;
		this.player_info.exp -= level_exp;
	}
	
	var msg = new MSG_300001();
	msg.player_level = this.player_info.level;
	msg.player_exp = this.player_info.exp;
	this.send_success_msg(Msg.ACTIVE_PLAYER_INFO, msg);
}
	
Game_Player.prototype.update_vip = function(charge_id) {
	var charge_exp = config.recharge_json[charge_id].vip_exp;
	this.player_info.vip_exp += charge_exp;
	var max_vip_level = config.util_json.max_vip_level;
	for (var i = this.player_info.vip_level; i < max_vip_level; ++i) {
		var level_exp = config.vip_json[i].level_exp;
		if (this.player_info.vip_exp < level_exp) 
			break;
		
		this.player_info.vip_level++;
		this.player_info.vip_exp -= level_exp;
	}
	
	var msg = new MSG_300002();
	msg.vip_level = this.player_info.vip_level;
	msg.vip_exp = this.player_info.vip_exp;
	this.send_success_msg(Msg.ACTIVE_VIP_INFO, msg);
}

Game_Player.prototype.set_guild_info = function(obj) {
	this.player_info.guild_id = obj.guild_id;
	this.player_info.guild_name = obj.guild_name;
	this.set_data_change();
	print('set_guild_info, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, 
	" guild_id:", this.player_info.guild_id, " guild_name:", this.player_info.guild_name);
}

Game_Player.prototype.move_to_point = function(obj) {
	print('move to point, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());
	this.centity.x = obj.pos.x;
	this.centity.y = obj.pos.y;
	this.centity.z = obj.pos.z;
	this.centity.update_position();
	
	var msg = new MSG_520300();
	msg.cid = get_cid(this.gate_cid, this.player_cid);
	msg.role_name = this.player_info.role_name;
	msg.pos.x = this.centity.x;
	msg.pos.y = this.centity.y;
	msg.pos.z = this.centity.z;
	var aoi_list = this.centity.get_aoi_list();
	this.centity.broadcast_msg_to_all(aoi_list, Msg.RES_UPDATE_POSITION, msg);
}

Game_Player.prototype.enter_scene = function(scene_id, x, y, z) {
	print('enter scene, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());
	this.centity.enter_scene(scene_id, x, y, z);
	
	var msg = new MSG_520300();
	msg.cid = get_cid(this.gate_cid, this.player_cid);
	msg.role_name = this.player_info.role_name;
	msg.pos.x = this.centity.x;
	msg.pos.y = this.centity.y;
	msg.pos.z = this.centity.z;
	var aoi_list = this.centity.get_aoi_list();
	this.centity.broadcast_msg_to_all(aoi_list, Msg.RES_UPDATE_POSITION, msg);
}

Game_Player.prototype.leave_scene = function(scene_id, x, y, z) {
	print('leave scene, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg = new MSG_300003();
	msg.cid = get_cid(this.gate_cid, this.player_cid);
	var aoi_list = this.centity.get_aoi_list();
	this.centity.broadcast_msg_to_all_without_self(aoi_list, Msg.ACTIVE_LEAVE_SCENE, msg);
	this.centity.leave_scene();
}

Game_Player.prototype.change_scene = function(obj) {
	print('change scene, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());
	if(is_scene_in_process(obj.target_scene)) {
		this.leave_scene();
		this.enter_scene(obj.target_scene, obj.pos.x, obj.pos.y, obj.pos.z);
		var msg = new MSG_520301();
		this.send_success_msg(Msg.RES_CHANGE_SCENE, msg);
	}
	else {
		var msg = new MSG_160002();
		msg.target_scene = obj.target_scene;
		msg.role_id = this.player_info.role_id;
		send_game_msg_to_master(this.player_cid, Msg.SYNC_GAME_MASTER_PLAYER_CHANGE_SCENE, 0, msg);
		
		this.save_player_data();
	}
}