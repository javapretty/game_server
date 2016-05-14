/*
*	描述：公会实现类
*	作者：李俊良
*	时间：2016/05/10
*/

function Guild() {
	this.guild_info = new Guild_Info();
}

Guild.prototype.load_data = function(buffer){
	print('LOAD GUILD DATA!!, util.now_msec:', util.now_msec());
	this.guild_info.deserialize(buffer);
}

Guild.prototype.create_guild = function(player, buffer) {
	print('create_guild, util.now_msec:', util.now_msec());
	var msg = new MSG_110101();
	msg.deserialize(buffer);

	var guild_data = new Guild_Data();
	
	guild_data.guild_id = this.get_next_id();
	guild_data.guild_name = msg.guild_name;
	guild_data.chief_id = player.player_info.role_id;
	print('chief id is ', guild_data.chief_id);
	guild_data.member_list = new Array();
	this.guild_info.guild_map.insert(guild_data.guild_id, guild_data);
	this.save_data(guild_data);
	this.sync_game_player(player, guild_data.guild_id);
	
	var msg = new MSG_510101();
	msg.guild_id = guild_data.guild_id;
	var buf = pop_master_buffer();
	msg.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_CREATE_GUILD, buf);
	push_master_buffer(buf);
}

Guild.prototype.get_next_id = function(){
	var max_id = 0;;
	for(var i = 0; i < this.guild_info.guild_map.keys.length; i++){
		if(this.guild_info.guild_map.keys[i] > max_id){
			max_id = this.guild_info.guild_map.keys[i];
		}
	}
	return max_id + 1;
}

Guild.prototype.dissove_guild = function(player, buffer) {
	print('dissove_guild, util.now_msec:', util.now_msec());
	var msg = new MSG_110102();
	msg.deserialize(buffer);
	var guild_data = this.guild_info.guild_map.get(msg.guild_id);
	if(guild_data == null){
		print('guild ', msg.guild_id, " not exist!");
		return;
	}
	for(var i = 0; i < guild_data.member_list.length; i++){
		var p = master_player_role_id_map.get(guild_data.member_list[i]);
		if(p == null)
			continue;
		this.sync_game_player(p, 0);
	}
	this.guild_info.guild_map.remove(msg.guild_id);
	this.delete_data(msg.guild_id);
	
	var msg = new MSG_510102();
	var buf = pop_master_buffer();
	msg.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_DISSOVE_GUILD, buf);
	push_master_buffer(buf);
}

Guild.prototype.join_guild = function(player, buffer) {
	print('join_guild, util.now_msec:', util.now_msec());
	var msg = new MSG_110103();
	msg.deserialize(buffer);
	var guild_data = this.guild_info.guild_map.get(msg.guild_id);
	if(guild_data == null){
		print('guild ', msg.guild_id, " not exist!");
		return;
	}
	guild_data.member_list.push(player.player_info.role_id);
	this.save_data(guild_data);
	this.sync_game_player(player, guild_data.guild_id);
	
	var msg = new MSG_510103();
	var buf = pop_master_buffer();
	msg.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_JOIN_GUILD, buf);
	push_master_buffer(buf);
}

Guild.prototype.save_data = function(data){
	var msg = new MSG_150102();
	//msg.reset();
	msg.guild_data = data;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_SAVE_GUILD_INFO);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Guild.prototype.delete_data = function(data){
	var msg = new MSG_150103();
	msg.guild_id = data;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_DELETE_GUILD_INFO);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Guild.prototype.sync_game_player = function(player, guild_id){
	var msg = new MSG_160100();
	msg.guild_id = guild_id;
	var buf = pop_master_buffer();
	msg.serialize(buf);
	player.cplayer.sync_data_to_game(Msg_GM.SYNC_MASTER_GAME_SET_GUILD, buf);
	push_master_buffer(buf);
}
