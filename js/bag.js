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
		this.player.cplayer.set_player_data_change(Data_Change.BAG_CHANGE);
	}
	
	this.fetch_bag_info = function() {
		print('fetch_bag_info, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
		var msg_res = new MSG_520100();
		this.bag_info.item_map.each(function(key,value,index) {
			msg_res.item_info.push(value);
    	});
    	
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_FETCH_BAG_INFO, buf);
		push_buffer(buf);
	}
	
	this.use_item = function(buffer) {
		print('use_item, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
		
		var msg_req = new MSG_120101();
		msg_req.deserialize(buffer);
		var item_array = new Array();
		item_array.push(msg_req.item);
		var result = this.bag_erase_item(item_array);
		this.player.cplayer.respond_error_result(Msg_Res.RES_USE_ITEM, result);
	}
	
	this.sell_item = function(buffer) {
		print('sell_item, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
		
		var msg_req = new MSG_120102();
		msg_req.deserialize(buffer);
		var item_array = new Array();
		item_array.push(msg_req.item);
		var result = this.bag_erase_item(item_array);
		this.player.cplayer.respond_error_result(Msg_Res.RES_SELL_ITEM, result);
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
	
	this.bag_insert_item = function(item_array) {
		for (var i = 0; i < item_array.length; ++i) {
			var item_info = this.bag_info.item_map.get(item_array[i].id);
			if (item_info == null) {
				this.bag_info.item_map = put(item_array[i].id, item_array[i]);	
			} else {
				item_info.amount += item_array[i].amount;
			}	
		}
		
		this.bag_active_item();
		return 0;
	}
	
	this.bag_erase_item = function(item_array) {
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
	}
	
	this.bag_active_money = function() {
		var msg_active = new MSG_300100();
		msg_active.copper = this.bag_info.copper;
		msg_active.gold = this.bag_info.gold;
		
		var buf = pop_buffer();
		msg_active.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Active.ACTIVE_MONEY_INFO, buf);
		push_buffer(buf);
		this.set_data_change();
	}
	
	this.bag_active_item = function() {
		var msg_active = new MSG_300101();
		this.bag_info.item_map.each(function(key,value,index) {
			msg_active.item_info.push(value);
    	});
    	
		var buf = pop_buffer();
		msg_active.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Active.ACTIVE_ITEM_INFO, buf);
		push_buffer(buf);
		this.set_data_change();
	}
}