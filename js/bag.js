/*
*	描述：背包系统
*	作者：张亚磊
*	时间：2016/02/26
*	qq:784599938
*	tel:18268193219
*/

function Bag() {
	this.player = null;
	this.bag_info = new Bag_Info();
	
	this.load_data = function(player, buffer) {
		this.player = player;
		this.bag_info.deserialize(buffer);
	}
	
	this.save_data = function(buffer) {
		this.bag_info.serialize(buffer);
	}
	
	this.set_data_change = function() {
		this.player.cplayer.set_player_data_change(DATA_CHANGE.BAG_CHANGE);
	}
	
	this.fetch_bag_info = function() {
		print('fetch_bag_info, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
	
		var msg_res = new MSG_520100();
		this.bag_info.item_map.each(function(key,value,index) {
			msg_res.item_info.push(value);
    	});
    	
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(MSG_RES.RES_FETCH_BAG_INFO, buf);
		push_buffer(buf);
	}
	
	this.use_item = function(buffer) {
		print('use_item, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var msg_req = new MSG_120101();
		msg_req.deserialize(buffer);
		var result = this.bag_erase_item(msg_req.item);
		
		this.player.cplayer.respond_error_result(MSG_RES.RES_USE_ITEM, result);
	}
	
	this.sell_item = function(buffer) {
		print('sell_item, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " msec:", msec());
		
		var msg_req = new MSG_120102();
		msg_req.deserialize(buffer);
		var result = this.bag_erase_item(msg_req.item);
		
		this.player.cplayer.respond_error_result(MSG_RES.RES_SELL_ITEM, result);
	}
	
	this.bag_add_money = function(copper, gold) {
		this.bag_info.copper += copper;
		this.bag_info.gold += gold;
		this.bag_active_money();
	}
	
	this.bag_sub_money = function(copper, gold) {
		this.bag_info.copper -= copper;
		this.bag_info.gold -= gold;
		this.bag_active_money();
	}
	
	this.bag_insert_item = function(item) {
		var item_info = this.bag_info.item_map.get(item.id);
		if (item_info == null) {
			if (this.bag_info.item_map.size() >= 2000)
				return ERROR_CODE.ERROR_BAG_FULL;
			this.bag_info.item_map = put(item.id, item);	
		} else {
			item_info.amount += item.amount;
		}
		
		this.bag_active_item();
		return 0;
	}
	
	this.bag_erase_item = function(item) {
		var item_info = this.bag_info.item_map.get(item.id);
		if (item_info != null) {
			item_info.amount -= item.amount;
			if (item_info.amount < 0) {
				this.bag_info.item_map.remove(item.id);
			}
			this.bag_active_item();
			return 0;		
		} else {
			print('item_info is null');
			return ERROR_CODE.ERROR_ITEM_NOT_ENOUGH;
		}
	}
	
	this.bag_active_money = function() {
		var msg_active = new MSG_300102();
		msg_active.copper = this.bag_info.copper;
		msg_active.gold = this.bag_info.gold;
		
		var buf = pop_buffer();
		msg_active.serialize(buf);
		this.player.cplayer.respond_success_result(MSG_ACTIVE.ACTIVE_MONEY_INFO, buf);
		push_buffer(buf);
		this.set_data_change();
	}
	
	this.bag_active_item = function() {
		var msg_active = new MSG_300100();
		this.bag_info.item_map.each(function(key,value,index) {
			msg_active.item_info.push(value);
    	});
    	
		var buf = pop_buffer();
		msg_active.serialize(buf);
		this.player.cplayer.respond_success_result(MSG_ACTIVE.ACTIVE_ITEM_INFO, buf);
		push_buffer(buf);
		this.set_data_change();
	}
}