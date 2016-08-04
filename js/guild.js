/*
*	描述：公会实现类
*	作者：李俊良
*	时间：2016/05/10
*/

function Guild() {
	this.guild_map = new Map();
	this.save_list = new Array();
	this.drop_list = new Array();
}

Guild.prototype.load_data = function(obj){
	print('load guild data, util.now_msec:', util.now_msec());
	for(var i = 0; i < obj.guild_list.length; i++){
		var guild = obj.guild_list[i];
		print("guild[", i, "].guild_id is ", guild.guild_id);
		this.guild_map.insert(guild.guild_id, guild);
	}
}

Guild.prototype.save_data = function(){
	var msg = new MSG_150103();
	this.guild_map.each(function(key,value,index) {
		if (value.is_change) {
			msg.guild_list.push(value);
			value.is_change = false;
		}
    });
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_SAVE_GUILD);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Guild.prototype.drop_guild = function(){
	if (this.drop_list.length <= 0) return;
		
	var msg = new MSG_150102();
	msg.struct_name = "Guild_Info";
	msg.index_list = this.drop_list;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_DELETE_DATA);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
	this.drop_list = [];
}

Guild.prototype.sync_guild_info_to_game = function(player, guild_id, guild_name){
	var msg = new MSG_160100();
	msg.guild_id = guild_id;
	msg.guild_name = guild_name;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_GM.SYNC_MASTER_GAME_GUILD_INFO);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_game(player.cid, buf);
	push_master_buffer(buf);
}

Guild.prototype.save_data_handler = function() {
	this.save_data();
	this.drop_guild();
}

Guild.prototype.member_join_guild = function(player, guild_detail, apply = false) {
	var member_detail = new Guild_Member_Detail();
	member_detail.role_id = player.player_info.role_id;
	member_detail.role_name = player.player_info.role_name;
	member_detail.level = player.player_info.level;
	member_detail.career = player.player_info.career;
	if(!apply)
		guild_detail.member_list.push(member_detail);
	else
		guild_detail.apply_list.push(member_detail);
}

Guild.prototype.create_guild = function(player, obj) {
	print('create_guild, guild_name:', obj.guild_name, ' util.now_msec:', util.now_msec());

	var db_msg = new MSG_150100();
	db_msg.guild_info.guild_name = obj.guild_name;
	db_msg.guild_info.chief_id = player.player_info.role_id;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_CREATE_GUILD);
	db_msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Guild.prototype.create_guild_res = function(obj) {
	print('create_guild_res, guild_id:', obj.guild_info.guild_id, ' guild_name:', obj.guild_info.guild_name, ' chief_id:', obj.guild_info.chief_id, ' util.now_msec:', util.now_msec());
	var player = master_player_role_id_map.get(obj.guild_info.chief_id);
	if (obj.guild_info.guild_id < 0) {
		player.cplayer.respond_error_result(Msg_MC.RES_CREATE_GUILD, Error_Code.ERROR_GUILD_EXIST);
		return;
	}
	
	var guild_detail = new Guild_Info();
	guild_detail.guild_id = obj.guild_info.guild_id;
	guild_detail.guild_name = obj.guild_info.guild_name;
	guild_detail.chief_id = obj.guild_info.chief_id;
	guild_detail.is_change = true;

	this.member_join_guild(player, guild_detail);
	this.guild_map.insert(guild_detail.guild_id, guild_detail);
	this.sync_guild_info_to_game(player, guild_detail.guild_id, guild_detail.guild_name);
	
	var msg_res = new MSG_510101();
	msg_res.guild_id = guild_detail.guild_id;
	var buf = pop_master_buffer();
	msg_res.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_CREATE_GUILD, buf);
	push_master_buffer(buf);
}

Guild.prototype.dissove_guild = function(player, obj) {
	print('dissove_guild, util.now_msec:', util.now_msec());
	var guild_detail = this.guild_map.get(obj.guild_id);
	if(guild_detail == null){
		print('guild ', obj.guild_id, " not exist!");
		return;
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
	this.guild_map.remove(obj.guild_id);
	this.drop_list.push(obj.guild_id);
	
	var msg_res = new MSG_510102();
	msg_res.guild_id = obj.guild_id;
	var buf = pop_master_buffer();
	msg_res.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_DISSOVE_GUILD, buf);
	push_master_buffer(buf);
}

Guild.prototype.join_guild = function(player, obj) {
	print('join_guild, util.now_msec:', util.now_msec());
	var guild_detail = this.guild_map.get(obj.guild_id);
	if(guild_detail == null){
		print('guild ', obj.guild_id, " not exist!");
		return;
	}
	this.member_join_guild(player, guild_detail, true);
	guild_detail.is_change = true;
	
	var msg_res = new MSG_510103();
	var buf = pop_master_buffer();
	msg_res.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_JOIN_GUILD, buf);
	push_master_buffer(buf);
}

Guild.prototype.allow_join_player = function(player, obj) {
	print('allow join guild, util.now_msec:', util.now_msec());
	var guild_detail = this.guild_map.get(obj.guild_id);
	if(guild_detail == null){
		print('guild ', obj.guild_id, " not exist!");
		return;
	}

	var member;
	for(var i = 0; i < guild_detail.apply_list.length; i++){
		if(obj.role_id == guild_detail.apply_list[i].role_id){
			if(obj.allow){
				member = guild_detail.apply_list[i];
				guild_detail.member_list.push(member);
				
				var mem_player = master_player_role_id_map.get(member.role_id);
				if(mem_player == null){
					offline_manager.set_offline_detail(member.role_id, guild_detail.guild_id, guild_detail.guild_name);
				} else {
					this.sync_guild_info_to_game(mem_player, guild_detail.guild_id, guild_detail.guild_name);
				}
			}
			guild_detail.apply_list.splice(i, 1);
			guild_detail.is_change = true;
			
			var msg_res = new MSG_510104();
			var buf = pop_master_buffer();
			msg_res.serialize(buf);
			player.cplayer.respond_success_result(Msg_MC.RES_GUILD_ALLOW_JOIN, buf);
			push_master_buffer(buf);
			return;
		}
	}
	player.cplayer.respond_error_result(Msg_MC.RES_GUILD_ALLOW_JOIN, Error_Code.ERROR_ROLE_ID_NOT_EXIST);
}

Guild.prototype.kick_out_player = function(player, obj) {
	print('kick out player, util.now_msec:', util.now_msec());
	var guild_detail = this.guild_map.get(obj.guild_id);
	if(guild_detail == null){
		print('guild ', obj.guild_id, " not exist!");
		return;
	}
	var member;
	for(var i = 0; i < guild_detail.member_list.length; i++){
		if(obj.role_id == guild_detail.member_list[i].role_id){
			guild_detail.member_list.splice(i, 1);
			guild_detail.is_change = true;
				
			var mem_player = master_player_role_id_map.get(obj.role_id);
			if(mem_player == null){
				offline_manager.set_offline_detail(obj.role_id, 0, "");
			} else {
				this.sync_guild_info_to_game(mem_player, 0, "");
			}

			var msg_res = new MSG_510105();
			var buf = pop_master_buffer();
			msg_res.serialize(buf);
			player.cplayer.respond_success_result(Msg_MC.RES_GUILD_KICK_OUT, buf);
			push_master_buffer(buf);
			return;
		}
	}
	player.cplayer.respond_error_result(Msg_MC.RES_GUILD_KICK_OUT, Error_Code.ERROR_ROLE_ID_NOT_EXIST);
}