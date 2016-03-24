/*
*	描述：定时器系统
*	作者：李俊良
*	时间：2016/03/15
*/

function Timer() {
	var timer_map = new Map(); 
	var timer_id = 1;
	
	this.register_timer = function(interval, next_tick, handler) {
		register_timer(timer_id, interval, next_tick);
		timer_map.insert(timer_id, handler);
		timer_id++;
	}

	this.get_timer_handler = function(timer_id) {
		return timer_map.get(timer_id);
	}
	
	this.init = function() {
		//注册玩家定时器，时间间隔500ms，每次到期遍历在线玩家进行处理
		this.register_timer(500, 0, this.player_handler);
		this.register_timer(1000 * 60 * 60 * 24, this.get_next_tick(8), this.refresh_shop_handler);
	}

	//获取当前时间到指定定时时间的间隔(每日)
	this.get_next_tick = function(hour, min = 0, sec = 0) {
		var date = new Date();
		date.setHours(hour);
		date.setMinutes(min);
		date.setSeconds(sec);
		date.setMilliseconds(0);
		var time = date.getTime() / 1000;
		var now = util.now_sec();
		var next_tick = time - now;
		return next_tick > 0 ? next_tick : (next_tick + 60 * 60 * 24);
	}
	
	this.player_handler = function() {
		var now = util.now_sec();
		player_role_id_map.each(function(key,value,index) {
			value.tick(now);
			value.bag.tick(now);
			value.hero.tick(now);
			value.mail.tick(now);
    	});
	}

	this.refresh_shop_handler = function() {
		player_role_id_map.each(function(key,value,index) {
			value.shop.tick();
    	});
	}
}
