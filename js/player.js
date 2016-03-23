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
	this.hero = new Hero();
	this.bag = new Bag();
	this.mail = new Mail();

	//玩家上线，加载数据
	this.load_player_data = function(buffer) {
		if (buffer == null) {
			return;
		}
	
		var gate_cid = buffer.read_int32();
		var player_cid = buffer.read_int32();
		var status = buffer.read_int8();
		var size = buffer.read_uint16();
		this.player_info.deserialize(buffer);
		this.hero.load_data(this, buffer);
		this.bag.load_data(this, buffer);
		this.mail.load_data(this, buffer);
		
		print('------load_data,status:', status, ' role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
		this.cid = gate_cid * 10000 + player_cid;
		this.cplayer = get_player_by_cid(gate_cid, player_cid);
		
		player_cid_map.put(this.cid, this);
		player_role_id_map.put(this.player_info.role_id, this);
	}
	
	//玩家离线，保存数据
	this.save_player_data = function() {
		var buffer = this.cplayer.player_data_buffer();
		if (buffer == null) {
			return;
		}
		
		buffer.write_int8(0);				//status
		buffer.write_uint16(0);			//change_set size
		this.player_info.serialize(buffer);
		this.hero.save_data(buffer);
		this.bag.save_data(buffer);
		this.mail.save_data(buffer);
		print('------save_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
		
		player_cid_map.remove(this.cid);
		player_role_id_map.remove(this.player_info.role_id);
	}
	
	this.set_data_change = function() {
		this.cplayer.set_player_data_change(Data_Change.PLAYER_CHANGE);
	}
	
	this.tick = function(now) {
		//技能点回复，五分钟回复一点
		if (this.player_info.skill_point < 10) {
			if (now > this.player_info.recover_skill_time) {
				this.player_info.skill_point++;
				this.player_info.recover_skill_time = now + 300;
			}
		}
	}
	
	this.add_exp = function(exp) {
		print('add_exp, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " exp:", exp);
		
		if (exp <= 0) {
			return Error_Code.ERROR_CLIENT_PARAM;
		}
		
		//经验增加升级
		this.player_info.exp += exp;
		var max_player_level = util.get_json_config('config/util/param.json', max_player_level);
		for (var i = this.player_info.level; i < max_player_level; ++i) {
			var level_exp = util.get_json_config('config/player/level.json', i).level_exp;
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
		this.set_data_change();
	}
	
	this.update_vip = function(charge_id) {
		var charge_exp = util.get_json_config('config/vip/charge.json', charge_id).vip_exp;
		this.player_info.vip_exp += charge_exp;
		var max_vip_level = util.get_json_config('config/util/param.json', max_vip_level);
		for (var i = this.player_info.vip_level; i < max_vip_level; ++i) {
			var level_exp = util.get_json_config('config/vip/vip.json', i).level_exp;
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
		this.set_data_change();
	}
	
	this.buy_vitality = function() {
		print('buy_vitality, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " util.now_msec:", util.now_msec());

		//1.检查可以购买体力次数
		var max_buy_times = util.get_json_config('config/vip/vip.json', this.player_info.vip_level).buy_vitality_max;
		if (this.player_info.buy_vitality_times >= max_buy_times){
			return this.cplayer.respond_error_result(Msg_Res.RES_BUY_VITALITY, Error_Code.ERROR_VITALITY_TIMES_NOT_ENOUGH);
		}

		//2.更新元宝
		var cost_gold = util.get_json_config('config/util/price.json', this.player_info.buy_vitality_times).vitality;
		var result = this.bag.bag_sub_money(0, cost_gold);
		if (result != 0) {
			return this.cplayer.respond_error_result(Msg_Res.RES_BUY_VITALITY, Error_Code.ERROR_GOLD_NOT_ENOUGH);
		}
		
		//3.更新体力(120应该为配置)
		this.player_info.buy_vitality_times++;
		var maxVit = util.get_json_config('config/player/level.json', this.player_info.level).max_vitality;
		this.player_info.vitality = Math.min(Math.max(0, (this.player_info.vitality + 120)), maxVit);
		
		//4.返回消息给客户端
		var buf = pop_buffer();
		buf.write_int32(this.player_info.vitality);
		this.cplayer.respond_success_result(Msg_Res.RES_BUY_VITALITY, buf);
		push_buffer(buf);
		this.set_data_change();
	}
}