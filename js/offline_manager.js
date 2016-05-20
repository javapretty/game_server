/*
*	描述：离线数据管理类
*	作者：李俊良
*	时间：2016/05/17
*/

function Offline_Manager() {
	this.drop_list = new Array();
	this.is_change = false;
	//公会变更消息通知
	this.offline_msg = new Offline_Msg();
}

Offline_Manager.prototype.load_data = function(buffer){
	print('LOAD OFFLINE_MSG DATA, util.now_msec:', util.now_msec());
	this.offline_msg.deserialize(buffer);
}

Offline_Manager.prototype.handle_offline_msg = function(player){
	print('fetch offline msg, util.now_msec:', util.now_msec());
	var role_id = player.player_info.role_id;
	var offline_msg_detail = this.offline_msg.msg_map.get(role_id);
	if(offline_msg_detail == null)
		return;
	this.handle_guild_msg(offline_msg_detail.guild_msg);

	this.offline_msg.msg_map.remove(role_id);
	this.drop_list.push(role_id);
	this.set_change();
}

Offline_Manager.prototype.store_guild_msg = function(role_id, guild_id) {
	var offline_msg_detail = this.get_offline_msg_detail(role_id);
	offline_msg_detail.guild_msg.push(guild_id);
	this.set_change();
}

Offline_Manager.prototype.get_offline_msg_detail = function(role_id) {
	var offline_msg_detail = this.offline_msg.msg_map.get(role_id);
	if(offline_msg_detail == null){
		offline_msg_detail = new Offline_Msg_Detail();
		offline_msg_detail.role_id = role_id;
		this.offline_msg.msg_map.insert(role_id, offline_msg_detail);
	}
	return offline_msg_detail;
}

Offline_Manager.prototype.handle_guild_msg = function(guild_msg) {
	for(var i = 0; i < guild_msg.length; i++){
		guild.sync_data_to_game(player, guild_msg[i]);
	}
}

Offline_Manager.prototype.save_data_handler = function() {
	if(!this.is_change)
		return;
	if(this.drop_list.length > 0)
		this.delete_data();
	this.save_data();
	this.set_change(false);
}

Offline_Manager.prototype.save_data = function(){
	var msg = new MSG_150104();
	msg.offline_msg = this.offline_msg;
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_SAVE_OFFLINE_MSG);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Offline_Manager.prototype.delete_data = function(){
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

Offline_Manager.prototype.set_change = function(change = true){
	this.is_change = change;
}
