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
		
		var buf = pop_buffer();
		buf.write_int32(1);
		this.player.cplayer.respond_success_result(msg_res.RES_ADD_HERO_STAR, buf);
		push_buffer(buf);
	}
	
	this.add_hero_quality = function(buffer) {
		print('add_hero_quality, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var buf = pop_buffer();
		buf.write_int32(1);
		this.player.cplayer.respond_success_result(msg_res.RES_ADD_HERO_QUALITY, buf);
		push_buffer(buf);
	}
}