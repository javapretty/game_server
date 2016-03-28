/*
*	描述：背包系统
*	作者：张亚磊
*	时间：2016/02/26
*/

function Bag() {
	this.game_player = null;
	this.bag_info = new Bag_Info();
}
	
Bag.prototype.load_data = function(game_player, buffer) {
	this.game_player = game_player;
	this.bag_info.deserialize(buffer);
}
	
Bag.prototype.save_data = function(buffer) {
	this.bag_info.serialize(buffer);
}

Bag.prototype.fetch_bag_info = function() {
	print('fetch_bag_info, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_res = new MSG_520100();
	this.bag_info.item_map.each(function(key,value,index) {
		msg_res.item_info.push(value);
    });
    	
	var buf = pop_buffer();
	msg_res.serialize(buf);
	this.game_player.cplayer.respond_success_result(Msg_Res.RES_FETCH_BAG_INFO, buf);
	push_buffer(buf);
}
	
Bag.prototype.use_item = function(buffer) {
	print('use_item, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120101();
	msg_req.deserialize(buffer);
	var item_array = new Array();
	item_array.push(msg_req.item);
	var result = this.bag_erase_item(item_array);
	this.game_player.cplayer.respond_error_result(Msg_Res.RES_USE_ITEM, result);
}
	
Bag.prototype.sell_item = function(buffer) {
	print('sell_item, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120102();
	msg_req.deserialize(buffer);
	var item_array = new Array();
	item_array.push(msg_req.item);
	var result = this.bag_erase_item(item_array);
	this.game_player.cplayer.respond_error_result(Msg_Res.RES_SELL_ITEM, result);
}
	
Bag.prototype.bag_add_money = function(copper, gold) {
	if(!util.is_number(copper) || !util.is_number(gold)){
		return Error_Code.ERROR_CLIENT_PARAM;
	}
	this.bag_info.copper += copper;
	this.bag_info.gold += gold;
	this.bag_active_money();
	return 0;
}
	
Bag.prototype.bag_sub_money = function(copper, gold) {
	if(!util.is_number(copper) || !util.is_number(gold)){
		return Error_Code.ERROR_CLIENT_PARAM;
	}
	if (this.bag_info.copper < copper) {
		return Error_Code.ERROR_COPPER_NOT_ENOUGH;
	}
	if (this.bag_info.gold < gold) {
		return Error_Code.ERROR_GOLD_NOT_ENOUGH;
	}
	this.bag_info.copper -= copper;
	this.bag_info.gold -= gold;
	this.bag_active_money();
	return 0;
}
	
Bag.prototype.bag_insert_item = function(item_array) {
	if (this.bag_info.item_map.size + item_array.length > 2000){
		return Error_Code.ERROR_BAG_FULL;
	}
	for (var i = 0; i < item_array.length; ++i) {
		var item_info = this.bag_info.item_map.get(item_array[i].item_id);
		if (item_info == null) {
			this.bag_info.item_map.insert(item_array[i].item_id, item_array[i]);	
		} else {
			item_info.amount += item_array[i].amount;
		}	
	}
	
	this.bag_active_item();
	return 0;
}
	
Bag.prototype.bag_erase_item = function(item_array) {
	for (var i = 0; i < item_array.length; ++i) {
		var item_info = this.bag_info.item_map.get(item_array[i].id);
		if (item_info == null) {
			return Error_Code.ERROR_ITEM_NOT_ENOUGH;
		} else {
			if (item_info.amount < item_array[i].amount) {
				return Error_Code.ERROR_ITEM_NOT_ENOUGH;
			}
		}
	}
	
	for (var i = 0; i < item_array.length; ++i) {
		var item_info = this.bag_info.item_map.get(item_array[i].id);
		item_info.amount -= item_array[i].amount;
		if (item_info.amount == 0) {
			this.bag_info.item_map.remove(item.id);
		}
	}
	
	this.bag_active_item();
	return 0;
}
	
Bag.prototype.bag_active_money = function() {
	var msg_active = new MSG_300100();
	msg_active.copper = this.bag_info.copper;
	msg_active.gold = this.bag_info.gold;
	
	var buf = pop_buffer();
	msg_active.serialize(buf);
	this.game_player.cplayer.respond_success_result(Msg_Active.ACTIVE_MONEY_INFO, buf);
	push_buffer(buf);
	this.game_player.set_data_change(Data_Change.BAG_CHANGE);
}
	
Bag.prototype.bag_active_item = function() {
	var msg_active = new MSG_300101();
	this.bag_info.item_map.each(function(key,value,index) {
		msg_active.item_info.push(value);
    });
    	
	var buf = pop_buffer();
	msg_active.serialize(buf);
	this.game_player.cplayer.respond_success_result(Msg_Active.ACTIVE_ITEM_INFO, buf);
	push_buffer(buf);
	this.game_player.set_data_change(Data_Change.BAG_CHANGE);
}
