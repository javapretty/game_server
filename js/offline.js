/*
*	描述：离线数据管理类
*	作者：李俊良
*	时间：2016/05/17
*/

function Offline() {
	this.offline_info = new Offline_Info();
	this.drop_list = new Array();
	this.is_change = false;
}

Offline.prototype.load_data = function(buffer){
	print('load offline info, util.now_msec:', util.now_msec());
	this.offline_info.deserialize(buffer);
}

Offline.prototype.save_data = function(){
	var msg = new MSG_150104();
	msg.offline_info = this.offline_info;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_SAVE_OFFLINE_MSG);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Offline.prototype.save_data_handler = function() {
	if(!this.is_change) return;
	this.drop_offline();
	this.save_data();
	this.set_change(false);
}

Offline.prototype.handle_offline_info = function(player){
	var role_id = player.player_info.role_id;
	var offline_detail = this.offline_info.offline_map.get(role_id);
	if(offline_detail == null)
		return;
	
	guild_manager.sync_guild_info_to_game(player, offline_detail.guild_id, offline_detail.guild_name);
	
	this.offline_info.offline_map.remove(role_id);
	this.drop_list.push(role_id);
	this.set_change();
}

Offline.prototype.set_offline_detail = function(role_id, guild_id, guild_name) {
	var offline_detail = this.get_offline_msg_detail(role_id);
	if (offline_detail)
	{
		offline_detail.guild_id = guild_id;
		offline_detail.guild_name = guild_name;
	} else {
		offline_detail = new Offline_Detail();
		offline_detail.role_id = role_id;
		offline_detail.guild_id = guild_id;
		offline_detail.guild_name = guild_name;
		this.offline_info.offline_map.insert(role_id, offline_detail);
	}
	this.set_change();
}

Offline.prototype.drop_offline = function(){
	if (this.drop_list.length <= 0) return;
	
	var msg = new MSG_150104();
	msg.offline_msg_list = this.drop_list;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_DROP_OFFLINE_MSG);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
	this.drop_list = [];
}

Offline.prototype.set_change = function(change = true){
	this.is_change = change;
}
