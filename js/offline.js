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
}

Offline.prototype.save_data = function(){
	var msg = new MSG_150105();
	msg.offline_info = this.offline_info;
	send_master_msg_to_db(Msg_MD.SYNC_MASTER_DB_SAVE_OFFLINE, msg);
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
	
	if(offline_detail.flag & 0x01){
		guild_manager.sync_guild_info_to_game(player, offline_detail.guild_id, offline_detail.guild_name);
		offline_detail.flag &= 0xFE;
	}
	
	this.offline_info.offline_map.delete(role_id);
	this.drop_list.push(role_id);
	this.set_change(true);
}

Offline.prototype.set_offline_detail = function(role_id, guild_id, guild_name) {
	var offline_detail = this.offline_info.offline_map.get(role_id);
	if (offline_detail)
	{
		offline_detail.guild_id = guild_id;
		offline_detail.guild_name = guild_name;
	} else {
		offline_detail = new Offline_Detail();
		offline_detail.role_id = role_id;
		offline_detail.guild_id = guild_id;
		offline_detail.guild_name = guild_name;
		this.offline_info.offline_map.set(role_id, offline_detail);
	}
	offline_detail.flag |= 0x01;
	this.set_change();
}

Offline.prototype.drop_offline = function(){
	if (this.drop_list.length <= 0) return;
	
	var msg = new MSG_150102();
	msg.struct_name = "Offline_Info";
	msg.index_list = this.drop_list;
	send_master_msg_to_db(Msg_MD.SYNC_MASTER_DB_DELETE_DATA, msg);
	this.drop_list = [];
}

Offline.prototype.set_change = function(is_change){
	this.is_change = is_change;
}
