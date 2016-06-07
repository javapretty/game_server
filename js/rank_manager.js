/*
*	描述：排行榜实现类
*	作者：李俊良
*	时间：2016/05/25
*/

function Rank_Manager() {
	this.rank_info = new Rank_Info();
}

Rank_Manager.prototype.load_data = function(buffer){
	print('load rank_manager data, util.now_msec:', util.now_msec());
	var msg = new MSG_550105();
	msg.deserialize(buffer);
	for(var i = 0; i < msg.rank_list.length; i++){
		var list = new Rank_List();
		list.rank_type = msg.rank_list[i].rank_type;
		print("rank type is ", list.rank_type);
		list.member_list = msg.rank_list[i].member_list;
		print("list size is ", list.member_list.length);
		print("list data 1 role_name is ", list.member_list[0].role_name);
		this.rank_info.rank_map.insert(list.rank_type, list);
	}
}

Rank_Manager.prototype.save_data = function(){
	var msg = new MSG_150105();
	msg.index = 0;
	this.rank_info.rank_map.each(function(key,value,index) {
		msg.rank_list.push(value);
    });
	var buf = pop_master_buffer();
	buf.make_inner_message(Msg_MD.SYNC_MASTER_DB_SAVE_RANK_INFO);
	msg.serialize(buf);
	buf.finish_message();
	send_master_buffer_to_db(buf);
	push_master_buffer(buf);
}

Rank_Manager.prototype.fetch_rank_info = function(player, buffer) {
	print('fetch rank info, util.now_msec:', util.now_msec());
	var msg = new MSG_110104();
	msg.deserialize(buffer);
	var rank_list = this.rank_info.rank_map.get(msg.type);
	
	var min = (rank_list.member_list.length > 100) ? 100 : rank_list.member_list.length;
	var msg_res = new MSG_510104();
	msg_res.rank_type = msg.type;
	for(var i = 0; i < min; i++){
		msg_res.rank_list.member_list.push(rank_list.member_list[i]);
	}
	for(var i = 0; i < rank_list.member_list.length; i++){
		if(player.player_info.role_id == rank_list.member_list[i].role_id){
			msg_res.rank = i + 1;
			break;
		}
	}
	var buf = pop_master_buffer();
	msg_res.serialize(buf);
	player.cplayer.respond_success_result(Msg_MC.RES_FETCH_RANK, buf);
	push_master_buffer(buf);
}

Rank_Manager.prototype.update_rank_level = function(player) {
	this.update_rank(Rank_Type.LEVEL_RANK, player);
}

Rank_Manager.prototype.get_player_value = function(type, player){
	//按照类型获取player对应数值
	return (type == Rank_Type.LEVEL_RANK) ? player.player_info.level :
			(type == Rank_Type.BATTLE_RANK) ? -1 :
			-1;
}

Rank_Manager.prototype.update_rank = function(type, player){
	var rank_list = this.rank_info.rank_map.get(type);
	if(rank_list == null){
		rank_list = new Rank_List();
		rank_list.rank_type = type;
		this.rank_info.rank_map.insert(type, rank_list);
	}

	var rank_member = null;
	var pos = -1;//当前位置
	var flag;//遍历方向
	var value = this.get_player_value(type, player);
	for(var i = 0; i < rank_list.member_list.length; i++){
		if(rank_list.member_list[i].role_id == player.player_info.role_id){
			rank_member = rank_list.member_list[i];
			rank_list.member_list.splice(i, 1);
			pos = i;
			flag = (rank_member.value > value) ? true : false;
			break;
		}
	}
	if(rank_member == null){
		rank_member = new Rank_Member();
		rank_member.role_id = player.player_info.role_id;
		rank_member.role_name = player.player_info.role_name;
		pos = 0;
		flag = true;
	}
	
	rank_member.value = value;
	
	if(flag){
		while(pos != rank_list.member_list.length &&
			rank_member.value <= rank_list.member_list[pos].value)
			pos++;
	}
	else {
		pos--;
		while(pos != -1 &&
			rank_member.value > rank_list.member_list[pos].value)
			pos--;
		pos++;
	}
	rank_list.member_list.splice(pos, 0, rank_member);
}

