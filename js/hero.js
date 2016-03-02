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
	}
	
	this.add_hero_star = function(buffer) {
		print('add_hero_star, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
	}
	
	this.add_hero_quality = function(buffer) {
		print('add_hero_quality, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
	}
}