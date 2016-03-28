/*
*	描述：game_player信息类
*	作者：张亚磊
*	时间：2016/02/24
*/

function Game_Player() {
	this.sync_player_data_tick = util.now_sec();
	this.cid = 0;
	this.cplayer = null;
	this.change_module = new Array();
	this.player_info = new Game_Player_Info();
	this.hero = new Hero();
	this.bag = new Bag();
	this.mail = new Mail();
	this.shop = new Shop();
}

	//玩家上线，加载数据
Game_Player.prototype.load_player_data = function(buffer) {
	if (buffer == null) {
		return;
	}
	
	var gate_cid = buffer.read_int32();
	var player_cid = buffer.read_int32();
	var status = buffer.read_int8();					//status
	var change_len = buffer.read_uint16();		//change_module len
	for(var i = 0; i < change_len; ++i) {
		var change_id = buffer.read_int32();
		this.change_module.push(change_id);
	}
	this.player_info.deserialize(buffer);
	this.hero.load_data(this, buffer);
	this.bag.load_data(this, buffer);
	this.mail.load_data(this, buffer);
	this.shop.load_data(this, buffer);
	
	print('------load_data,status:', status, ' role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
	this.cid = gate_cid * 10000 + player_cid;
	this.cplayer = get_player_by_cid(gate_cid, player_cid);
	
	game_player_cid_map.insert(this.cid, this);
	game_player_role_id_map.insert(this.player_info.role_id, this);
}
	
//玩家离线，保存数据
Game_Player.prototype.save_player_data = function() {
	print('------save_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
	
	this.sync_player_data();
	game_player_cid_map.remove(this.cid);
	game_player_role_id_map.remove(this.player_info.role_id);
}

Game_Player.prototype.sync_player_data = function() {
	var buffer = this.cplayer.get_player_data_buffer();
	if (buffer == null) {
		return;
	}
	
	buffer.write_int8(0);			//status
	var change_len = this.change_module.length;
	buffer.write_uint16(change_len);
	for(var i = 0; i < change_len; ++i) {
		buffer.write_int32(this.change_module[i]);
	}
	this.player_info.serialize(buffer);
	this.hero.save_data(buffer);
	this.bag.save_data(buffer);
	this.mail.save_data(buffer);
	this.shop.save_data(buffer);
}
	
Game_Player.prototype.set_data_change = function(change_id) {
	var exist = false;
	for (var i = 0; i < this.change_module.length; ++i) {
		if (this.change_module[i] == change_id) {
			exist = true;
			break;
		}
	}
	if (!exist) {
		this.change_module.push(change_id);
	}
}
	
Game_Player.prototype.tick = function(now) {
	//技能点回复，五分钟回复一点
	if (this.player_info.skill_point < 10) {
		if (now - this.player_info.recover_skill_time >= 300) {
			this.player_info.skill_point++;
			this.player_info.recover_skill_time = now;
		}
	}
	
	//同步玩家数据到C++,15s一次
	if (now - this.sync_player_data_tick >= 15) {
		this.sync_player_data();
		this.sync_player_data_tick = now;
	}
}

Game_Player.prototype.daily_refresh = function() {
	this.player_info.buy_vitality_times = 0;
	this.player_info.exchange_count = 0;
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
	var buf = pop_buffer();
	msg_active.serialize(buf);
	this.cplayer.respond_success_result(Msg_Active.ACTIVE_PLAYER_INFO, buf);
	push_buffer(buf);
	this.set_data_change(Data_Change.PLAYER_CHANGE);
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
	var buf = pop_buffer();
	msg_active.serialize(buf);
	this.cplayer.respond_success_result(Msg_Active.ACTIVE_VIP_INFO, buf);
	push_buffer(buf);
	this.set_data_change(Data_Change.PLAYER_CHANGE);
}
	
Game_Player.prototype.buy_vitality = function() {
	print('buy_vitality, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());

	//1.检查可以购买体力次数
	var max_buy_times = config.vip_json[this.player_info.vip_level].max_buy_vitality;
	if (this.player_info.buy_vitality_times >= max_buy_times){
		return this.cplayer.respond_error_result(Msg_Res.RES_BUY_VITALITY, Error_Code.ERROR_VITALITY_TIMES_NOT_ENOUGH);
	}

	//2.更新元宝
	var buy_vitality_gold = config.util_json.buy_vitality_gold;
	if (buy_vitality_gold == null || this.player_info.buy_vitality_times >= buy_vitality_gold.length) {
		return this.cplayer.respond_error_result(Msg_Res.RES_BUY_VITALITY, Error_Code.ERROR_CONFIG_NOT_EXIST);
	}	
	var cost_gold = buy_vitality_gold[this.player_info.buy_vitality_times];
	var result = this.bag.bag_sub_money(0, cost_gold);
	if (result != 0) {
		return this.cplayer.respond_error_result(Msg_Res.RES_BUY_VITALITY, Error_Code.ERROR_GOLD_NOT_ENOUGH);
	}
	
	//3.更新体力(120应该为配置)
	this.player_info.buy_vitality_times++;
	var maxVit = config.level_json[this.player_info.level].max_vitality;
	this.player_info.vitality = Math.min(Math.max(0, (this.player_info.vitality + 120)), maxVit);
	
	//4.返回消息给客户端
	var buf = pop_buffer();
	buf.write_int32(this.player_info.vitality);
	this.cplayer.respond_success_result(Msg_Res.RES_BUY_VITALITY, buf);
	push_buffer(buf);
	this.set_data_change(Data_Change.PLAYER_CHANGE);
}

Game_Player.prototype.exchange_money = function(buffer){
	print('exchange_money, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());
	
	//检查是否还有剩余次数
	if(this.player_info.exchange_count >= config.vip_json[this.player_info.vip_level].exchange_count)
		return this.cplayer.respond_error_result(Msg_Res.RES_EXCHANGE_MONEY, Error_Code.ERROR_EXCHANGE_COUNT_NOT_ENOUGH);
	
	var cost = config.util_json.exchange_array[this.player_info.exchange_count].gold;
	//元宝是否足够
	var error = this.bag.bag_sub_money(0, cost);
	if(error != 0)
		return this.cplayer.respond_error_result(Msg_Res.RES_EXCHANGE_MONEY, error);
	
	var add_copper = config.util_json.exchange_array[this.player_info.exchange_count].copper;
	var msg_res = new MSG_520004();
	msg_res.copper = 0;

	var rate = Math.floor(Math.random() * 100);
	if(rate < config.util_json['exchange_rate']){
		add_copper *= 2;
		msg_res.copper = add_copper;
	}
	this.bag.bag_add_money(add_copper, 0);
	this.player_info.exchange_count++;

	//返回消息给客户端
	var buf = pop_buffer();
	msg_res.serialize(buf);
	this.cplayer.respond_success_result(Msg_Res.RES_EXCHANGE_MONEY, buf);
	push_buffer(buf);
	
	this.set_data_change(Data_Change.PLAYER_CHANGE);
}
