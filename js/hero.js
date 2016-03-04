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
	}
	
	this.save_data = function(buffer) {
		this.hero_info.serialize(buffer);
	}
	
	this.set_data_change = function() {
		this.player.cplayer.set_player_data_change(data_change.HERO_CHANGE);
	}
	
	this.fetch_hero_info = function() {
		print('fetch_hero_info, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var buf = pop_buffer();
		buf.write_uint16(this.hero_info.hero_map.size());
		this.hero_info.hero_map.each(function(key,value,index) {
			value.serialize(buf);
    	});
		this.player.cplayer.respond_success_result(msg_res.RES_FETCH_HERO_INFO, buf);
		push_buffer(buf);
	}
	
	this.add_hero_star = function(buffer) {
		print('add_hero_star, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var hero_id = buffer.read_int32();
		var hero_detail = this.hero_info.hero_map.get(hero_id);
		if (hero_detail == null) {
			return this.player.cplayer.respond_error_result(msg_res.RES_ADD_HERO_STAR, error.ERROR_CLIENT_PARAM);
		}
		
		var json_str = read_json('config/hero/hero.json');
  		var hero_obj = JSON.parse(json_str)[hero_id];
    	if (hero_obj == null) {
    		return this.player.cplayer.respond_error_result(msg_res.RES_ADD_HERO_STAR, error.ERROR_CONFIG_NOT_EXIST);
    	}
    	
    	if (hero_detail.level == hero_obj.star_item_amount.length) {
    		return this.player.cplayer.respond_error_result(msg_res.RES_ADD_HERO_STAR, error.ERROR_CLIENT_OPERATE);
    	}
    		
    	var item = new Item_Info();
    	item.id = hero_obj.star_item_id;
    	item.amount = hero_obj.star_item_amount[hero_detail.level];
    	print(item.id, item.amount);
    	var result = this.player.bag.bag_erase_item(item);
    	if (result != 0) {
    		return this.player.cplayer.respond_error_result(msg_res.RES_ADD_HERO_STAR, result);
    	}
    	hero_detail.star++;
    	this.set_data_change();
    	
		var buf = pop_buffer();
		buf.write_int32(hero_id);
		buf.write_int32(hero_detail.star);
		this.player.cplayer.respond_success_result(msg_res.RES_ADD_HERO_STAR, buf);
		push_buffer(buf);
	}
	
	this.add_hero_quality = function(buffer) {
		print('add_hero_quality, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var hero_id = buffer.read_int32();
		this.set_data_change();

		var buf = pop_buffer();
		buf.write_int32(hero_id);
		buf.write_int32(1);
		this.player.cplayer.respond_success_result(msg_res.RES_ADD_HERO_QUALITY, buf);
		push_buffer(buf);
	}
}