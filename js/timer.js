/*
*	描述：定时器系统
*	作者：李俊良
*	时间：2016/03/15
*/

function Timer() {
	var timer_map = new Map(); 
	var timer_id = 1;
	
	this.register_timer = function(interval, next_tick, handler) {
		register_game_timer(timer_id, interval, next_tick);
		timer_map.insert(timer_id, handler);
		timer_id++;
	}

	this.get_timer_handler = function(timer_id) {
		return timer_map.get(timer_id);
	}
	
	this.init = function() {
		//注册玩家定时器，时间间隔500ms，每次到期遍历在线玩家进行处理
		this.register_timer(500, 0, this.player_handler);
		this.register_timer(util.whole_day_msec, util.get_next_day_tick(config.util_json['daily_refresh_time']), this.daily_refresh_handler);
	}

	this.player_handler = function() {
		var now = util.now_sec();
		game_player_role_id_map.each(function(key,value,index) {
			value.tick(now);
			value.hero.tick(now);
    	});
	}

	this.daily_refresh_handler = function() {
		game_player_role_id_map.each(function(key,value,index) {
			value.daily_refresh();
			value.shop.daily_refresh();
    	});
	}
}
