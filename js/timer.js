/*
*	描述：定时器系统
*	作者：李俊良
*	时间：2016/03/15
*/

function Timer() {
	var game_timer_map = new Map(); 
	var game_timer_id = 1;
	
	var master_timer_map = new Map();
	var master_timer_id = 1;
	
	this.init = function(server_type) {
		if (server_type == Server_Type.GAME_SERVER) {
			//注册玩家定时器，时间间隔500ms，每次到期遍历在线玩家进行处理
			this.register_game_timer(500, 0, this.game_player_handler);
			//注册每日刷新定时器,时间间隔24h
			this.register_game_timer(util.whole_day_msec, util.get_next_day_tick(config.util_json['daily_refresh_time']), this.game_daily_refresh_handler);
			//AI定时器
			this.register_game_timer(config.ai_json.thinking_interval, 0, this.npc_ai_handler);
		} else if (server_type == Server_Type.MASTER_SERVER) {
			//注册玩家定时器，时间间隔500ms，每次到期遍历在线玩家进行处理
			this.register_master_timer(500, 0, this.master_player_handler);
			//注册每日刷新定时器,时间间隔24h
			this.register_master_timer(util.whole_day_msec, util.get_next_day_tick(config.util_json['daily_refresh_time']), this.master_daily_refresh_handler);
			//注册公共信息保存定时器
			this.register_master_timer(15000, 0, this.master_data_save_handler);
		}
	}
	
	//注册game_server定时器
	this.register_game_timer = function(interval, next_tick, handler) {
		register_game_timer(game_timer_id, interval, next_tick);
		game_timer_map.set(game_timer_id, handler);
		game_timer_id++;
	}

	//获取game_server定时器
	this.get_game_timer_handler = function(timer_id) {
		return game_timer_map.get(timer_id);
	}

	//注册master_server定时器
	this.register_master_timer = function(interval, next_tick, handler) {
		register_master_timer(master_timer_id, interval, next_tick);
		master_timer_map.set(master_timer_id, handler);
		master_timer_id++;
	}

	//获取master_server定时器
	this.get_master_timer_handler = function(timer_id) {
		return master_timer_map.get(timer_id);
	}
	
	/////////////////////////////////定时器处理函数//////////////////////////////////
	this.game_player_handler = function() {
		var now = util.now_sec();
		for (var value of game_player_role_id_map.values()) {
  			value.tick(now);
		}
	}

	this.game_daily_refresh_handler = function() {
		for (var value of game_player_role_id_map.values()) {
  			value.daily_refresh();
		}
	}
	
	this.master_player_handler = function() {
		var now = util.now_sec();
		for (var value of master_player_role_id_map.values()) {
  			value.tick(now);
		}
	}

	this.master_daily_refresh_handler = function() {
		for (var value of master_player_role_id_map.values()) {
  			value.daily_refresh();
		}
	}

	this.master_data_save_handler = function() {
		guild_manager.save_data_handler();
		rank_manager.save_data();
		offline_manager.save_data_handler();
	}

	this.npc_ai_handler = function(){
		for(var npc of npc_map.values()) {
			thinking_in_ai(npc);
		}
	}

}
