/*
*	描述：game_player信息类
*	作者：张亚磊
*	时间：2016/02/24
*/

function Game_Player() {
	this.sync_player_data_tick = util.now_sec();
	this.gate_cid = 0;
	this.player_cid = 0;
	this.cplayer = null;
	this.is_change = false;
	this.player_info = new Game_Player_Info();
	this.hero = new Hero();
	this.bag = new Bag();
	this.mail = new Mail();
	this.shop = new Shop();
}

//玩家上线，加载数据
Game_Player.prototype.load_player_data = function(gate_cid, player_cid, client_ip, buffer) {
	this.gate_cid = gate_cid;
	this.player_cid = player_cid; 
	this.player_info.deserialize(buffer);
	this.player_info.client_ip = client_ip;
	this.hero.load_data(this, buffer);
	this.bag.load_data(this, buffer);
	this.mail.load_data(this, buffer);
	this.shop.load_data(this, buffer);
	
	print('***************game_player load_data, role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
	this.cplayer = get_game_player_by_gate_cid(gate_cid, player_cid);
	if(this.cplayer == null) {
		print('get game_player null, role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
		game_close_client(gate_cid, player_cid, Error_Code.ERROR_CLIENT_PARAM);
		return;
	}
	
	this.sync_login_to_client();
	this.sync_login_to_master();
	login_map.remove(this.player_info.account);
	game_player_cid_map.insert(this.gate_cid * 10000 + this.player_cid, this);
	game_player_role_id_map.insert(this.player_info.role_id, this);
	game_player_role_name_map.insert(this.player_info.role_name, this);
}

//玩家离线，保存数据
Game_Player.prototype.save_player_data = function() {
	print('***************game_player save_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
	this.player_info.logout_time = util.now_sec();
	this.sync_player_data_to_db(1);
	this.sync_logout_to_log();
	
	logout_map.insert(this.player_info.account, this.player_info.logout_time);
	game_player_cid_map.remove(this.gate_cid * 10000 + this.player_cid);
	game_player_role_id_map.remove(this.player_info.role_id);
	game_player_role_name_map.remove(this.player_info.role_name);
}

Game_Player.prototype.sync_player_data_to_db = function(status) {
	var buf = pop_game_buffer();
	buf.make_inner_message(Msg_GD.SYNC_GAME_DB_SAVE_PLAYER, status);
	this.player_info.serialize(buf);
	this.hero.save_data(buf);
	this.bag.save_data(buf);
	this.mail.save_data(buf);
	this.shop.save_data(buf);
	buf.finish_message();
	send_game_buffer_to_db(buf);
	push_game_buffer(buf);
	
	this.is_change = false;
}

Game_Player.prototype.set_data_change = function() {
	this.is_change = true;
}

Game_Player.prototype.tick = function(now) {
	//同步玩家数据到数据库
	if(this.is_change == true){
		if (now - this.sync_player_data_tick >= 20) {
			this.sync_player_data_to_db(0);
			this.sync_player_data_tick = now;
		}
	}
}

Game_Player.prototype.daily_refresh = function() {
	this.player_info.buy_vitality_times = 0;
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
	msg.role_info.vitality = this.player_info.vitality;
	msg.role_info.buy_vitality_times = this.player_info.buy_vitality_times;
	msg.role_info.vip_level = this.player_info.vip_level;
	msg.role_info.vip_exp = this.player_info.vip_exp;
	msg.role_info.charge_gold = this.player_info.charge_gold;
	
  var buf = pop_game_buffer();
  msg.serialize(buf);
	this.cplayer.respond_success_result(Msg_GC.RES_FETCH_ROLE_INFO, buf);
	push_game_buffer(buf);
}

Game_Player.prototype.sync_login_to_master = function() {
	var msg = new MSG_160000();
	msg.player_info.role_id = this.player_info.role_id;
	msg.player_info.account = this.player_info.account;
	msg.player_info.role_name = this.player_info.role_name;
	msg.player_info.level = this.player_info.level;
	msg.player_info.gender = this.player_info.gender;
	msg.player_info.career = this.player_info.career;
	msg.player_info.vip_level = this.player_info.vip_level;
	
	var buf = pop_game_buffer();
	buf.make_player_message(Msg_GM.SYNC_GAME_MASTER_PLYAER_LOGIN, 0, this.player_cid);
	msg.serialize(buf);
	buf.finish_message();
	send_game_buffer_to_master(buf);
	push_game_buffer(buf);
}

Game_Player.prototype.sync_logout_to_log = function() {
	var msg = new MSG_180001();
	msg.role_id = this.player_info.role_id;
	msg.role_name = this.player_info.role_name;
	msg.account = this.player_info.account;
	msg.level = this.player_info.level;
	msg.client_ip = this.player_info.client_ip;
	msg.login_time = this.player_info.login_time;
	msg.logout_time = this.player_info.logout_time;
	
	var buf = pop_game_buffer();
	buf.make_inner_message(Msg_Log.SYNC_LOG_LOGINOUT);
	msg.serialize(buf);
	buf.finish_message();
	send_game_buffer_to_log(buf);
	push_game_buffer(buf);
}

Game_Player.prototype.add_exp = function(exp) {
	print('add_exp, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " exp:", exp);
	
	if (exp <= 0) {
		return Error_Code.ERROR_CLIENT_PARAM;
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
	
	var msg_active = new MSG_300001();
	msg_active.player_level = this.player_info.level;
	msg_active.player_exp = this.player_info.exp;
	var buf = pop_game_buffer();
	msg_active.serialize(buf);
	this.cplayer.respond_success_result(Msg_Active.ACTIVE_PLAYER_INFO, buf);
	push_game_buffer(buf);
	this.set_data_change();
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
	
	var msg_active = new MSG_300002();
	msg_active.vip_level = this.player_info.vip_level;
	msg_active.vip_exp = this.player_info.vip_exp;
	var buf = pop_game_buffer();
	msg_active.serialize(buf);
	this.cplayer.respond_success_result(Msg_Active.ACTIVE_VIP_INFO, buf);
	push_game_buffer(buf);
	this.set_data_change();
}
	
Game_Player.prototype.buy_vitality = function() {
	print('buy_vitality, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());

	//1.检查可以购买体力次数
	var max_buy_times = config.vip_json[this.player_info.vip_level].max_buy_vitality;
	if (this.player_info.buy_vitality_times >= max_buy_times){
		return this.cplayer.respond_error_result(Msg_GC.RES_BUY_VITALITY, Error_Code.ERROR_VITALITY_TIMES_NOT_ENOUGH);
	}

	//2.更新元宝
	var buy_vitality_gold = config.util_json.buy_vitality_gold;
	if (buy_vitality_gold == null || this.player_info.buy_vitality_times >= buy_vitality_gold.length) {
		return this.cplayer.respond_error_result(Msg_GC.RES_BUY_VITALITY, Error_Code.ERROR_CONFIG_NOT_EXIST);
	}	
	var cost_gold = buy_vitality_gold[this.player_info.buy_vitality_times];
	var result = this.bag.bag_sub_money(0, cost_gold);
	if (result != 0) {
		return this.cplayer.respond_error_result(Msg_GC.RES_BUY_VITALITY, Error_Code.ERROR_GOLD_NOT_ENOUGH);
	}
	
	//3.更新体力(120应该为配置)
	this.player_info.buy_vitality_times++;
	var maxVit = config.level_json[this.player_info.level].max_vitality;
	this.player_info.vitality = Math.min(Math.max(0, (this.player_info.vitality + 120)), maxVit);
	
	//4.返回消息给客户端
	var buf = pop_game_buffer();
	buf.write_int32(this.player_info.vitality);
	this.cplayer.respond_success_result(Msg_GC.RES_BUY_VITALITY, buf);
	push_game_buffer(buf);
	this.set_data_change();
}

Game_Player.prototype.set_guild_info = function(buffer) {
	var msg = new MSG_160100();
	msg.deserialize(buffer);
	this.player_info.guild_id = msg.guild_id;
	this.player_info.guild_name = msg.guild_name;
	this.set_data_change();
	print('set_guild_info, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, 
	" guild_id:", this.player_info.guild_id, " guild_name:", this.player_info.guild_name);
}