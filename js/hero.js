/*
*	描述：英雄系统
*	作者：张亚磊
*	时间：2016/03/02
*	qq:784599938
*	tel:18268193219
*/

function Hero() {
	this.player = null;
	this.hero_info = new Hero_Info();
	
	this.load_data = function(player, buffer) {
		this.player = player;
		this.hero_info.deserialize(buffer);
		
		//初始化英雄装备信息
		this.hero_info.hero_map.each(function(key,value,index) {
			if (value.equip_info.length != 6) {
				value.equip_info = [];
				for (var i = 0; i < 6; ++i) {
					var equip_detail = new Equip_Detail();
					value.equip_info.push(equip_detail);
				}
			}
    	});
	}
	
	this.save_data = function(buffer) {
		this.hero_info.serialize(buffer);
	}
	
	this.set_data_change = function() {
		this.player.cplayer.set_player_data_change(Data_Change.HERO_CHANGE);
	}
	
	this.fetch_hero_info = function() {
		print('fetch_hero_info, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var msg_res = new MSG_520300();
		this.hero_info.hero_map.each(function(key,value,index) {
			msg_res.hero_info.push(value);
    	});
    	var buf = pop_buffer();
    	msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_FETCH_HERO_INFO, buf);
		push_buffer(buf);
	}
	
	this.add_hero_star = function(buffer) {
		print('add_hero_star, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var msg_req = new MSG_120302();
		msg_req.deserialize(buffer); 
		var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
		if (hero_detail == null) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, Error_Code.ERROR_CLIENT_PARAM);
		}
		
		var json_str = read_json('config/hero/hero.json');
  		var hero_obj = JSON.parse(json_str)[msg_req.hero_id];
    	if (hero_obj == null) {
    		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, Error_Code.ERROR_CONFIG_NOT_EXIST);
    	}
    	
    	if (hero_detail.level == hero_obj.star_item_amount.length) {
    		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, Error_Code.ERROR_CLIENT_OPERATE);
    	}
    		
    	var item = new Item_Info();
    	item.id = hero_obj.star_item_id;
    	item.amount = hero_obj.star_item_amount[hero_detail.level];
    	print(item.id, item.amount);
    	var result = this.player.bag.bag_erase_item(item);
    	if (result != 0) {
    		return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_HERO_STAR, result);
    	}
    	hero_detail.star++;
    	this.set_data_change();
    	
    	var msg_res = new MSG_520301();
    	msg_res.hero_id = msg_req.hero_id;
    	msg_res.star = hero_detail.star;
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_ADD_HERO_STAR, buf);
		push_buffer(buf);
	}
	
	this.add_hero_quality = function(buffer) {
		print('add_hero_quality, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
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
		this.set_data_change();

		var msg_res = new MSG_520302();
		msg_res.hero_id = msg_req.hero_id;
		msg_res.quality = hero_detail.quality;
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_ADD_HERO_QUALITY, buf);
		push_buffer(buf);
	}
	
	this.add_equip_level = function(buffer) {
		print('add_equip_level, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var msg_req = new MSG_120303();
		msg_req.deserialize(buffer); 
		var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
		if (hero_detail == null || msg_req.equip_index < 0 || msg_req.equip_index >= hero_detail.equip_info.length) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_ADD_EQUIP_LEVEL, Error_Code.ERROR_CLIENT_PARAM);
		}
		
		this.set_data_change();

		var msg_res = new MSG_520303();
		msg_res.hero_id = msg_req.hero_id;
		msg_res.equip_index = msg_req.equip_index;
		msg_res.equip_level = 1;
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_ADD_EQUIP_LEVEL, buf);
		push_buffer(buf);
	}
	
	this.equip_on_off = function(buffer) {
		print('equip_on_off, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var msg_req = new MSG_120304();
		msg_req.deserialize(buffer); 
		var hero_detail = this.hero_info.hero_map.get(msg_req.hero_id);
		if (hero_detail == null || msg_req.equip_index < 0 || msg_req.equip_index >= hero_detail.equip_info.length) {
			return this.player.cplayer.respond_error_result(Msg_Res.RES_EQUIP_ON_OFF, Error_Code.ERROR_CLIENT_PARAM);
		}
		this.set_data_change();

		var msg_res = new MSG_520304();
		msg_res.hero_id = msg_req.hero_id;
		msg_res.on = msg_req.on;
		msg_res.item_info = msg_req.item_info;
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_EQUIP_ON_OFF, buf);
		push_buffer(buf);
	}
}