/*
*	描述：排行榜实现类
*	作者：李俊良
*	时间：2016/05/25
*/

function Rank() {
	this.rank_map = new Map();
}

Rank.prototype.load_data = function(buffer){
	print('load rank_manager data, util.now_msec:', util.now_msec());
	var msg = new MSG_550105();
	msg.deserialize(buffer);
	print('rank list size is ', msg.rank_list.length);
	for(var i = 0; i < msg.rank_list.length; i++){
		var rank = msg.rank_list[i];
		this.rank_map.insert(rank.rank_type, rank);
	}
}

Rank.prototype.save_data = function(){
	var msg = new MSG_150105();
	this.rank_map.each(function(key,value,index) {
		msg.rank_list.push(value);
    });
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_SAVE_RANK_INFO);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Rank.prototype.fetch_rank_info = function(player, buffer) {
	print('fetch rank info, util.now_msec:', util.now_msec());
	var msg = new MSG_110201();
	msg.deserialize(buffer);
	var rank_info = this.rank_map.get(msg.type);
	
	var msg_res = new MSG_510201();
	msg_res.rank_type = msg.type;
	rank_info.member_map.each(function(key,value,index) {
		msg_res.rank_list.push(value);
	});
	
	msg_res.rank_list.sort(function(a,b){
		return b.value - a.value;
	});
	for(var i = 0; i < msg_res.rank_list.length; i++){
		if(player.player_info.role_id == msg_res.rank_list[i].role_id){
			msg_res.self_rank = i + 1;
			break;
		}
	}

	var buf = pop_master_buffer();
	msg_res.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_FETCH_RANK, buf);
	push_master_buffer(buf);
}

Rank.prototype.update_rank_level = function(player) {
	this.update_rank(Rank_Type.LEVEL_RANK, player);
}

Rank.prototype.get_player_value = function(type, player){
	//按照类型获取player对应数值
	return (type == Rank_Type.LEVEL_RANK) ? player.player_info.level :
			(type == Rank_Type.BATTLE_RANK) ? -1 :
			-1;
}

Rank.prototype.update_rank = function(type, player){
	var rank_info = this.rank_map.get(type);
	if(rank_info == null){
		rank_info = new Rank_Info();
		rank_info.rank_type = type;
		rank_info.min_value = 0x7fffffff;
		this.rank_map.insert(type, rank_info);
	}

	var rank_member = rank_info.member_map.get(player.player_info.role_id);
	var value = this.get_player_value(type, player);

	if(rank_member == null){
		rank_member = new Rank_Member();
		rank_member.role_id = player.player_info.role_id;
		rank_member.role_name = player.player_info.role_name;
		rank_member.value = value;
		if(rank_info.member_map.size() < 100){
			rank_info.member_map.insert(rank_member.role_id, rank_member);
			if(rank_member.value < rank_info.min_value){
				rank_info.min_value = rank_member.value;
				rank_info.min_role_id = rank_member.role_id;
			}
		}
		else{
			if(rank_member.value > rank_info.min_value){
				rank_info.member_map.remove(rank_info.min_role_id);
				rank_info.member_map.insert(rank_member.role_id, rank_member);
				this.refresh_min_value(rank_info);
			}
		}
	}
	else {
		rank_member.value = value;
		if(rank_member.value < rank_info.min_value){
			rank_info.min_value = rank_member.value;
			rank_info.min_role_id = rank_member.role_id;
		}
		else
			this.refresh_min_value(rank_info);
	}
	
}

Rank.prototype.refresh_min_value = function(rank_info){
	rank_info.min_value = 0x7fffffff;
	rank_info.member_map.each(function(key,value,index) {
		if(value.value < rank_info.min_value){
			rank_info.min_value = value.value;
			rank_info.min_role_id = value.role_id;
		}
	});
}

