/*
*	描述：英雄系统
*	作者：张亚磊
*	时间：2016/03/02
*/

function Hero() {
	this.game_player = null;
	this.hero_info = new Hero_Info();
}
	
Hero.prototype.load_data = function(game_player, obj) {
	this.game_player = game_player;
	this.hero_info = obj.player_data.hero_info;
}
	
Hero.prototype.save_data = function(obj) {
	obj.player_data.hero_info = this.hero_info;
}
	
Hero.prototype.fetch_hero_info = function() {
	print('fetch_hero_info, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var msg = new MSG_520300();
	for (var value of this.hero_info.hero_map.values()) {
  		msg.hero_info.push(value);
	}
  this.game_player.send_success_msg(Msg.RES_FETCH_HERO_INFO, msg);
}
	
Hero.prototype.add_hero_star = function(obj) {
	print('add_hero_star, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var hero_detail = this.hero_info.hero_map.get(obj.hero_id);
	if (hero_detail == null) {
		return this.game_player.send_error_msg(Msg.RES_ADD_HERO_STAR, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var hero_obj = config.hero_json[obj.hero_id];
  if (hero_obj == null || hero_detail.level >= hero_obj.star_item_amount.length) {
  		return this.game_player.send_error_msg(Msg.RES_ADD_HERO_STAR, Error_Code.ERROR_CONFIG_NOT_EXIST);
    }
    	
  if (hero_detail.level == hero_obj.star_item_amount.length) {
  		return this.game_player.send_error_msg(Msg.RES_ADD_HERO_STAR, Error_Code.ERROR_CLIENT_OPERATE);
    }
    	
	var item = new Item_Detail();
	item.id = hero_obj.star_item_id;
	item.amount = hero_obj.star_item_amount[hero_detail.level];
	var item_array = new Array();
	item_array.push(item);
  	var result = this.game_player.bag.bag_erase_item(item_array);
	if (result != 0) {
		return this.game_player.send_error_msg(Msg.RES_ADD_HERO_STAR, result);
	}
	hero_detail.star++;
	this.refresh_hero_property(hero_detail);
	this.game_player.set_data_change();
    	
	var msg = new MSG_520301();
	msg.hero_id = obj.hero_id;
	msg.star = hero_detail.star;
	this.game_player.send_success_msg(Msg.RES_ADD_HERO_STAR, msg);
}
	
Hero.prototype.add_hero_quality = function(obj) {
	print('add_hero_quality, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var hero_detail = this.hero_info.hero_map.get(obj.hero_id);
	if (hero_detail == null) {
		return this.game_player.send_error_msg(Msg.RES_ADD_HERO_QUALITY, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var can_add = true;
	for (var i = 0; i < hero_detail.equip_info.length; ++i) {
		if (hero_detail.equip_info[i].equip_id == 0) {
			can_add = false;
			break;
		}
	}
	if (!can_add) {
		return this.game_player.send_error_msg(Msg.RES_ADD_HERO_QUALITY, Error_Code.ERROR_CLIENT_OPERATE);
	}
	//英雄突破，装备清零
	for (var i = 0; i < hero_detail.equip_info.length; ++i) {
		hero_detail.equip_info[i] = new Equip_Info();
	}
	hero_detail.quality++;
	this.refresh_hero_property(hero_detail);

	var msg = new MSG_520302();
	msg.hero_id = obj.hero_id;
	msg.quality = hero_detail.quality;
	this.game_player.send_success_msg(Msg.RES_ADD_HERO_QUALITY, msg);
}
	
Hero.prototype.add_equip_level = function(obj) {
	print('add_equip_level, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var hero_detail = this.hero_info.hero_map.get(obj.hero_id);
	if (hero_detail == null || obj.equip_index < 0 || obj.equip_index >= hero_detail.equip_info.length) {
		return this.game_player.send_error_msg(Msg.RES_ADD_EQUIP_LEVEL, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	var equip = hero_detail.equip_info[obj.equip_index];
	var json_str = read_json('config/bag/item.json');
	var json_obj = JSON.parse(json_str);
  	var equip_obj = json_obj[equip.item_id];
  if (equip_obj == null || equip.level >= equip_obj.level_exp.length) {
  		return this.game_player.send_error_msg(Msg.RES_ADD_EQUIP_LEVEL, Error_Code.ERROR_CONFIG_NOT_EXIST);
    }
    	
  var result = this.game_player.bag.bag_erase_item(obj.item_info);
	if (result != 0) {
		return this.game_player.send_error_msg(Msg.RES_ADD_EQUIP_LEVEL, result);
	}
    	
    //增加经验升级
  for (var i = 0; i < obj.item_info.length; ++i) {
    	equip.exp += json_obj[obj.item_info[i].item_id].exp * obj.item_info[i].amount;
    }
  if (equip.exp >= equip_obj.level_exp[equip.level]) {
    	equip.exp -= equip_obj.level_exp[equip.level];
    	equip.level++;
    }
  this.refresh_hero_property(hero_detail);

	var msg = new MSG_520303();
	msg.hero_id = obj.hero_id;
	msg.equip_index = obj.equip_index;
	msg.equip_level = equip.level;
	msg.equip_exp = equip.exp;
	this.game_player.send_success_msg(Msg.RES_ADD_EQUIP_LEVEL, msg);
}
	
Hero.prototype.equip_on_off = function(obj) {
	print('equip_on_off, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var hero_detail = this.hero_info.hero_map.get(obj.hero_id);
	if (hero_detail == null || obj.equip_index < 0 || obj.equip_index >= hero_detail.equip_info.length) {
		return this.game_player.send_error_msg(Msg.RES_EQUIP_ON_OFF, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	if (obj.on) {
		//穿装备
		var equip_obj = config.item_json[obj.equip_info.item_id];
		if (equip_obj == null) {
			return this.game_player.send_error_msg(Msg.RES_EQUIP_ON_OFF, Error_Code.ERROR_CONFIG_NOT_EXIST);
		}
		//判断装备等级，部位
		if (equip_obj.level > hero_detail.level || equip_obj.part != obj.equip_index) {
			return this.game_player.send_error_msg(Msg.RES_EQUIP_ON_OFF, Error_Code.ERROR_CLIENT_OPERATE);
		}
		
		var item_array = new Array();
		item_array.push(obj.equip_info);
		var result = this.game_player.bag.bag_erase_item(item_array);
		if (result != 0) {
			return this.game_player.send_error_msg(Msg.RES_EQUIP_ON_OFF, result);
		}
		
		hero_detail.equip_info[obj.equip_index] = obj.equip_info;
	} else {
		//脱装备
		var item_array = new Array();
		item_array.push(hero_detail.equip_info[obj.equip_index]);
		var result = this.game_player.bag.bag_insert_item(item_array);
		if (result != 0) {
			return this.game_player.send_error_msg(Msg.RES_EQUIP_ON_OFF, result);
		}
		hero_detail.equip_info[obj.equip_index] = new Item_Detail();
	}
	this.refresh_hero_property(hero_detail);
	this.game_player.set_data_change();

	var msg = new MSG_520304();
	msg.hero_id = obj.hero_id;
	msg.on = obj.on;
	msg.item_info = obj.item_info;
	this.game_player.send_success_msg(Msg.RES_EQUIP_ON_OFF, msg);
}
	
Hero.prototype.add_skill_level = function(obj) {
	print('add_skill_level, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var hero_detail = this.hero_info.hero_map.get(obj.hero_id);
	if (hero_detail == null) {
		return this.game_player.send_error_msg(Msg.RES_ADD_SKILL_LEVEL, Error_Code.ERROR_CLIENT_PARAM);
	}
	
	for (var i = 0; i < hero_detail.skill_info.length; ++i) {
		if (obj.skill_id == hero_detail.skill_info[i]) {
			hero_detail.skill_info[i]++;
			break;
		}
	}

	var msg = new MSG_520305();
	msg.hero_id = obj.hero_id;
	msg.skill_id = obj.skill_id + 1;
	this.game_player.send_success_msg(Msg.RES_ADD_SKILL_LEVEL, msg);
}
	
Hero.prototype.refresh_hero_property = function(hero_detail) {
	//根据公式计算各模块对英雄属性的英雄，包括装备，星级，品质
	var msg = new MSG_300300();
	msg.property_info = hero_detail.property;
	this.game_player.send_success_msg(Msg.ACTIVE_PROPERTY_INFO, msg);
}
