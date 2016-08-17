/*
*	描述：背包系统
*	作者：张亚磊
*	时间：2016/02/26
*/

function Bag() {
	this.game_player = null;
	this.bag_info = new Bag_Info();
}
	
Bag.prototype.load_data = function(game_player, obj) {
	this.game_player = game_player;
	this.bag_info = obj.player_data.bag_info;	
}

Bag.prototype.save_data = function(obj) {
	obj.player_data.bag_info = this.bag_info;
}

Bag.prototype.fetch_bag_info = function() {
	print('fetch_bag_info, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var msg = new MSG_520100();
	for (var value of this.bag_info.item_map.values()) {
  		msg.item_info.push(value);
	}
  this.game_player.send_success_msg(Msg.RES_FETCH_BAG_INFO, msg);
}
	
Bag.prototype.use_item = function(obj) {
	print('use_item, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var item_array = new Array();
	item_array.push(obj.item);
	var result = this.bag_erase_item(item_array);
	this.game_player.send_error_msg(Msg.RES_USE_ITEM, result);
}
	
Bag.prototype.sell_item = function(obj) {
	print('sell_item, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	var item_array = new Array();
	item_array.push(obj.item);
	var result = this.bag_erase_item(item_array);
	this.game_player.send_error_msg(Msg.RES_SELL_ITEM, result);
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
			this.bag_info.item_map.set(item_array[i].item_id, item_array[i]);	
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
			this.bag_info.item_map.delete(item.id);
		}
	}
	
	this.bag_active_item();
	return 0;
}
	
Bag.prototype.bag_active_money = function() {
	var msg = new MSG_300100();
	msg.copper = this.bag_info.copper;
	msg.gold = this.bag_info.gold;
	this.game_player.send_success_msg(Msg.ACTIVE_MONEY_INFO, msg);
}
	
Bag.prototype.bag_active_item = function() {
	var msg = new MSG_300101();
	for (var value of this.bag_info.item_map.values()) {
  		msg.item_info.push(value);
	}
  this.game_player.send_success_msg(Msg.ACTIVE_ITEM_INFO, msg);
}
