/*
*	描述：英雄系统
*	作者：张亚磊
*	时间：2016/03/02
*/

function Hero() {
	this.player = null;
	this.hero_info = new Hero_Info();
}
	
Hero.prototype.load_data = function(player, buffer) {
	this.player = player;
	this.hero_info.deserialize(buffer);
	
	//初始化英雄装备信息
	this.hero_info.hero_map.each(function(key,value,index) {
		if (value.equip_info.length != 6) {
			value.equip_info = [];
			for (var i = 0; i < 6; ++i) {
				var item_info = new Item_Info();
				value.equip_info.push(item_info);
			}
		}
    });
}
	
Hero.prototype.save_data = function(buffer) {
	this.hero_info.serialize(buffer);
}
	
Hero.prototype.tick = function(now) {
	
}
	
Hero.prototype.fetch_hero_info = function() {
	print('fetch_hero_info, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_res = new MSG_520300();
	this.hero_info.hero_map.each(function(key,value,index) {
		msg_res.hero_info.push(value);
    });
    var buf = pop_buffer();
    msg_res.serialize(buf);
	this.player.cplayer.respond_success_result(Msg_Res.RES_FETCH_HERO_INFO, buf);
	push_buffer(buf);
}
	
Hero.prototype.add_hero_star = function(buffer) {
	print('add_hero_star, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120302();
	msg_req.deserialize(buffer); 
	var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
	if (hero_detail == null) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var hero_obj = config.hero_json[msg_req.hero_id];
    if (hero_obj == null || hero_detail.level >= hero_obj.star_item_amount.length) {
    	return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, Error_Code.ERROR_CONFIG_NOT_EXIST);
    }
    	
    if (hero_detail.level == hero_obj.star_item_amount.length) {
    	return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, Error_Code.ERROR_CLIENT_OPERATE);
    }
    	
	var item = new Item_Info();
	item.id = hero_obj.star_item_id;
	item.amount = hero_obj.star_item_amount[hero_detail.level];
	var item_array = new Array();
	item_array.push(item);
  	var result = this.player.bag.bag_erase_item(item_array);
	if (result != 0) {
    	return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, result);
	}
	hero_detail.star++;
	this.refresh_hero_property(hero_detail);
	this.player.set_data_change(Data_Change.HERO_CHANGE);
    	
	var msg_res = new MSG_520301();
	msg_res.hero_id = msg_req.hero_id;
	msg_res.star = hero_detail.star;
	var buf = pop_buffer();
	msg_res.serialize(buf);
	this.player.cplayer.respond_success_result(Msg_Res.RES_ADD_HERO_STAR, buf);
	push_buffer(buf);
}
	
Hero.prototype.add_hero_quality = function(buffer) {
	print('add_hero_quality, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120302();
	msg_req.deserialize(buffer); 
	var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
	if (hero_detail == null) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_QUALITY, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var can_add = true;
	for (var i = 0; i < hero_detail.equip_info.length; ++i) {
		if (hero_detail.equip_info[i].equip_id == 0) {
			can_add = false;
			break;
		}
	}
	if (!can_add) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_QUALITY, Error_Code.ERROR_CLIENT_OPERATE);
	}
	//英雄突破，装备清零
	for (var i = 0; i < hero_detail.equip_info.length; ++i) {
		hero_detail.equip_info[i] = new Equip_Info();
	}
	hero_detail.quality++;
	this.refresh_hero_property(hero_detail);
	this.player.set_data_change(Data_Change.HERO_CHANGE);

	var msg_res = new MSG_520302();
	msg_res.hero_id = msg_req.hero_id;
	msg_res.quality = hero_detail.quality;
	var buf = pop_buffer();
	msg_res.serialize(buf);
	this.player.cplayer.respond_success_result(Msg_Res.RES_ADD_HERO_QUALITY, buf);
	push_buffer(buf);
}
	
Hero.prototype.add_equip_level = function(buffer) {
	print('add_equip_level, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120303();
	msg_req.deserialize(buffer); 
	var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
	if (hero_detail == null || msg_req.equip_index < 0 || msg_req.equip_index >= hero_detail.equip_info.length) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_EQUIP_LEVEL, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var equip = hero_detail.equip_info[msg_req.equip_index];
	var json_str = read_json('config/bag/item.json');
	var json_obj = JSON.parse(json_str);
  	var equip_obj = json_obj[equip.item_id];
    if (equip_obj == null || equip.level >= equip_obj.level_exp.length) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_EQUIP_LEVEL, Error_Code.ERROR_CONFIG_NOT_EXIST);
    }
    	
    var result = this.player.bag.bag_erase_item(msg_req.item_info);
	if (result != 0) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_EQUIP_LEVEL, result);
	}
    	
    //增加经验升级
    for (var i = 0; i < msg_req.item_info.length; ++i) {
    	equip.exp += json_obj[msg_req.item_info[i].item_id].exp * msg_req.item_info[i].amount;
    }
    if (equip.exp >= equip_obj.level_exp[equip.level]) {
    	equip.exp -= equip_obj.level_exp[equip.level];
    	equip.level++;
    }
    this.refresh_hero_property(hero_detail);
	this.player.set_data_change(Data_Change.HERO_CHANGE);

	var msg_res = new MSG_520303();
	msg_res.hero_id = msg_req.hero_id;
	msg_res.equip_index = msg_req.equip_index;
	msg_res.equip_level = equip.level;
	msg_res.equip_exp = equip.exp;
	var buf = pop_buffer();
	msg_res.serialize(buf);
	this.player.cplayer.respond_success_result(Msg_Res.RES_ADD_EQUIP_LEVEL, buf);
	push_buffer(buf);
}
	
Hero.prototype.equip_on_off = function(buffer) {
	print('equip_on_off, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120304();
	msg_req.deserialize(buffer); 
	var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
	if (hero_detail == null || msg_req.equip_index < 0 || msg_req.equip_index >= hero_detail.equip_info.length) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_EQUIP_ON_OFF, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	if (msg_req.on) {
		//穿装备
		var equip_obj = config.item_json[msg_req.equip_info.item_id];
		if (equip_obj == null) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_EQUIP_ON_OFF, Error_Code.ERROR_CONFIG_NOT_EXIST);
		}
		//判断装备等级，部位
		if (equip_obj.level > hero_detail.level || equip_obj.part != msg_req.equip_index) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_EQUIP_ON_OFF, Error_Code.ERROR_CLIENT_OPERATE);
		}
		
		var item_array = new Array();
		item_array.push(msg_req.equip_info);
		var result = this.player.bag.bag_erase_item(item_array);
		if (result != 0) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_EQUIP_ON_OFF, result);
		}
		
		hero_detail.equip_info[msg_req.equip_index] = msg_req.equip_info;
	} else {
		//脱装备
		var item_array = new Array();
		item_array.push(hero_detail.equip_info[msg_req.equip_index]);
		var result = this.player.bag.bag_insert_item(item_array);
		if (result != 0) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_EQUIP_ON_OFF, result);
		}
		hero_detail.equip_info[msg_req.equip_index] = new Item_Info();
	}
	this.refresh_hero_property(hero_detail);
	this.player.set_data_change(Data_Change.HERO_CHANGE);

	var msg_res = new MSG_520304();
	msg_res.hero_id = msg_req.hero_id;
	msg_res.on = msg_req.on;
	msg_res.item_info = msg_req.item_info;
	var buf = pop_buffer();
	msg_res.serialize(buf);
	this.player.cplayer.respond_success_result(Msg_Res.RES_EQUIP_ON_OFF, buf);
	push_buffer(buf);
}
	
Hero.prototype.add_skill_level = function(buffer) {
	print('add_skill_level, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	if (this.player.player_info.skill_point <= 0) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_SKILL_LEVEL, Error_Code.ERROR_SKILL_POINT_NOT_ENOUGH);
	}
	
	var msg_req = new MSG_120305();
	msg_req.deserialize(buffer); 
	var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
	if (hero_detail == null) {
		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_SKILL_LEVEL, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	for (var i = 0; i < hero_detail.skill_info.length; ++i) {
		if (msg_req.skill_id == hero_detail.skill_info[i]) {
			hero_detail.skill_info[i]++;
			this.player.player_info.skill_point--;
			break;
		}
	}
	this.player.set_data_change(Data_Change.HERO_CHANGE);

	var msg_res = new MSG_520305();
	msg_res.hero_id = msg_req.hero_id;
	msg_res.skill_id = msg_req.skill_id + 1;
	var buf = pop_buffer();
	msg_res.serialize(buf);
	this.player.cplayer.respond_success_result(Msg_Res.RES_ADD_SKILL_LEVEL, buf);
	push_buffer(buf);
}
	
Hero.prototype.refresh_hero_property = function(hero_detail) {
	//根据公式计算各模块对英雄属性的英雄，包括装备，星级，品质
	
	var msg_active = new MSG_300300();
	msg_active.property_info = hero_detail.property;
	var buf = pop_buffer();
	msg_active.serialize(buf);
	this.player.cplayer.respond_success_result(Msg_Active.ACTIVE_PROPERTY_INFO, buf);
	push_buffer(buf);
}