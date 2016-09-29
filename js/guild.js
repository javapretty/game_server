/*
*	描述：公会实现类
*	作者：李俊良
*	时间：2016/05/10
*/

function Guild() {
	this.guild_map = new Map();
	this.drop_list = new Array();
	this.is_change = false;
}

Guild.prototype.load_data = function(obj) {
	print('load guild data, size:', obj.guild_list.length);	
	for(var i = 0; i < obj.guild_list.length; i++) {
		var guild_info = obj.guild_list[i];
		print('guild_id:', guild_info.guild_id, ' guild_name:', guild_info.guild_name);
		if(guild_info.member_list.length > 0)
			print('member_list[0].role_id is ', guild_info.member_list[0].role_id);
		else
			print('member_list is empty')
		if(guild_info.app_list.length > 0)
			print('app_list[0].role_id is ', guild_info.app_list[0].role_id);
		else
			print('app_list is empty')
		this.guild_map.set(guild_info.guild_id, guild_info);
	}
}

Guild.prototype.save_data = function(){
	var msg = new MSG_150103();
	for (var value of this.guild_map.values()) {
  		msg.guild_list.push(value);
	}
  send_master_msg_to_db(Msg.SYNC_MASTER_DB_SAVE_GUILD, msg);
  this.is_change = false;
}

Guild.prototype.drop_guild = function(){
	if (this.drop_list.length <= 0) return;
		
	var msg = new MSG_150102();
	msg.struct_name = "Guild_Info";
	msg.index_list = this.drop_list;
	send_master_msg_to_db(Msg.SYNC_MASTER_DB_DELETE_DATA, msg);
	this.drop_list = [];
}

Guild.prototype.sync_guild_info_to_game = function(player, guild_id, guild_name){
	var msg = new MSG_160100();
	msg.role_id = player.player_info.role_id;
	msg.guild_id = guild_id;
	msg.guild_name = guild_name;
	send_master_msg_to_game(player.game_cid, player.player_cid, Msg.SYNC_MASTER_GAME_GUILD_INFO, 0, msg);
}

Guild.prototype.save_data_handler = function() {
	if (!this.is_change) return;
	this.save_data();
	this.drop_guild();
	this.is_change = false;
}

Guild.prototype.member_join_guild = function(player, guild_detail) {
	var member_detail = new Guild_Member_Detail();
	member_detail.role_id = player.player_info.role_id;
	member_detail.role_name = player.player_info.role_name;
	member_detail.level = player.player_info.level;
	guild_detail.member_list.push(member_detail);
	guild_detail.app_list.push(member_detail);
	this.is_change = true;
}

Guild.prototype.create_guild = function(player, obj) {
	print('create_guild, guild_name:', obj.guild_name, ' chief_id:', player.player_info.role_id, ' util.now_msec:', util.now_msec());

	var msg = new MSG_150100();
	msg.guild_info.guild_name = obj.guild_name;
	msg.guild_info.chief_id = player.player_info.role_id;
	send_master_msg_to_db(Msg.SYNC_MASTER_DB_CREATE_GUILD, msg);
}

Guild.prototype.create_guild_res = function(obj) {
	print('create_guild_res, guild_id:', obj.guild_info.guild_id, ' guild_name:', obj.guild_info.guild_name, ' chief_id:', obj.guild_info.chief_id, ' util.now_msec:', util.now_msec());
	var player = master_player_role_id_map.get(obj.guild_info.chief_id);
	if (obj.guild_info.guild_id < 0) {
		return player.send_error_msg(Msg.RES_CREATE_GUILD, Error_Code.ERROR_GUILD_EXIST);
	}
	
	var guild_detail = new Guild_Info();
	guild_detail.guild_id = obj.guild_info.guild_id;
	guild_detail.guild_name = obj.guild_info.guild_name;
	guild_detail.chief_id = obj.guild_info.chief_id;
	guild_detail.create_time = util.now_sec();

	this.member_join_guild(player, guild_detail);
	this.guild_map.set(guild_detail.guild_id, guild_detail);
	//公会创建完成保存一次数据，防止服务器异常终止，数据丢失
	this.save_data();
	this.sync_guild_info_to_game(player, guild_detail.guild_id, guild_detail.guild_name);
	
	var msg = new MSG_510101();
	msg.guild_id = guild_detail.guild_id;
	player.send_success_msg(Msg.RES_CREATE_GUILD, msg);
}

Guild.prototype.dissove_guild = function(player, obj) {
	print('dissove_guild, util.now_msec:', util.now_msec());
	var guild_detail = this.guild_map.get(obj.guild_id);
	if(guild_detail == null){
		return player.send_error_msg(Msg.RES_DISSOVE_GUILD, Error_Code.ERROR_GUILD_NOT_EXIST);
	}
	for(var i = 0; i < guild_detail.member_list.length; i++){
		var mem_player = master_player_role_id_map.get(guild_detail.member_list[i].role_id);
		if(mem_player == null){
			//离线数据，保存到离线数据列表
			offline_manager.set_offline_detail(guild_detail.member_list[i].role_id, guild_detail.guild_id, guild_detail.guild_name);
		} else {
			this.sync_guild_info_to_game(mem_player, 0, "");
		}
	}
	this.guild_map.delete(obj.guild_id);
	this.drop_list.push(obj.guild_id);
	
	var msg = new MSG_510102();
	msg.guild_id = obj.guild_id;
	player.send_success_msg(Msg.RES_DISSOVE_GUILD, msg);
	this.is_change = true;
}

Guild.prototype.kick_out_player = function(player, obj) {
	print('kick out player, util.now_msec:', util.now_msec());
	var guild_detail = this.guild_map.get(obj.guild_id);
	if(guild_detail == null){
		return player.send_error_msg(Msg.RES_GUILD_KICK_OUT, Error_Code.ERROR_GUILD_NOT_EXIST);
	}

	for(var i = 0; i < guild_detail.member_list.length; i++){
		if(obj.role_id == guild_detail.member_list[i].role_id){
			guild_detail.member_list.splice(i, 1);
				
			var mem_player = master_player_role_id_map.get(obj.role_id);
			if(mem_player == null){
				offline_manager.set_offline_detail(obj.role_id, 0, "");
			} else {
				this.sync_guild_info_to_game(mem_player, 0, "");
			}
			return player.send_error_msg(Msg.RES_GUILD_KICK_OUT, 0);
		}
	}
	player.send_error_msg(Msg.RES_GUILD_KICK_OUT, Error_Code.ERROR_CLIENT_PARAM);
	this.is_change = true;
}
