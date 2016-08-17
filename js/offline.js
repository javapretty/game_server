/*
*	描述：离线数据管理类
*	作者：李俊良
*	时间：2016/05/17
*/

function Offline() {
	this.offline_map = new Map();
	this.drop_list = new Array();
	this.is_change = false;
}

Offline.prototype.load_data = function(obj) {
	print('load offline data, size:', obj.offline_list.length);	
	for(var i = 0; i < obj.offline_list.length; i++) {
		var offline_info = obj.offline_list[i];
		print('role_id:', offline_info.role_id);
		this.offline_map.set(offline_info.role_id, offline_info);
	}
}

Offline.prototype.save_data = function() {
	var msg = new MSG_150105();
	for (var value of this.offline_map.values()) {
  		msg.offline_list.push(value);
	}
	send_master_msg_to_db(Msg.SYNC_MASTER_DB_SAVE_OFFLINE, msg);
}

Offline.prototype.save_data_handler = function() {
	if(!this.is_change) return;
	this.drop_offline();
	this.save_data();
	this.is_change = false;
}

Offline.prototype.handle_offline_info = function(player) {
	var offline_info = this.offline_map.get(player.player_info.role_id);
	if(offline_info == null)
		return;
	
	guild_manager.sync_guild_info_to_game(player, offline_info.guild_id, offline_info.guild_name);
	this.offline_map.delete(role_id);
	this.drop_list.push(role_id);
	this.is_change = true;
}

Offline.prototype.set_offline_detail = function(role_id, guild_id, guild_name) {
	var offline_info = this.offline_map.get(role_id);
	if (offline_info) {
		offline_info.guild_id = guild_id;
		offline_info.guild_name = guild_name;
	} else {
		offline_info = new Offline_Info();
		offline_info.role_id = role_id;
		offline_info.guild_id = guild_id;
		offline_info.guild_name = guild_name;
		this.offline_info.offline_map.set(role_id, offline_info);
	}
	this.is_change = true;
}

Offline.prototype.drop_offline = function(){
	if (this.drop_list.length <= 0) return;
	
	var msg = new MSG_150102();
	msg.struct_name = "Offline_Info";
	msg.index_list = this.drop_list;
	send_master_msg_to_db(Msg.SYNC_MASTER_DB_DELETE_DATA, msg);
	this.drop_list = [];
}