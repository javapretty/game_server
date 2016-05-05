/*
*	描述：商店系统
*	作者：李俊良
*	时间：2016/03/15
*/

function Shop() {
	this.game_player = null;
	this.shop_info = new Shop_Info();
}

Shop.prototype.init = function(){
	var copper_shop = this.get_rand_product(config.shop_json[Shop_Type.COPPER_SHOP].product, 2);
	copper_shop.shop_type = Shop_Type.COPPER_SHOP;
	this.shop_info.shop_detail.insert(Shop_Type.COPPER_SHOP, copper_shop);
}
	
Shop.prototype.load_data = function(game_player, buffer) {
	this.game_player = game_player;
	this.shop_info.deserialize(buffer);
	if(this.shop_info.shop_detail.empty()){
		this.init();
	}
}

Shop.prototype.save_data = function(buffer) {
	this.shop_info.serialize(buffer);
}

Shop.prototype.daily_refresh = function() {
	this.refresh_shop(Shop_Type.COPPER_SHOP);
}
	
Shop.prototype.fetch_shop_info = function(buffer){
	print('fetch_shop_info, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120400();
	msg_req.deserialize(buffer);
	var msg_res = new MSG_520400();
	msg_res.shop = this.shop_info.shop_detail.get(msg_req.shop_type);
	
	var buf = pop_game_buffer();
	msg_res.serialize(buf);
	this.game_player.cplayer.respond_success_result(Msg_GC.RES_FETCH_SHOP_INFO, buf);
	push_game_buffer(buf);
}
	
Shop.prototype.refresh_by_player = function(buffer){
	print('refresh_by_player, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120402();
	msg_req.deserialize(buffer);
	var count = this.shop_info.shop_detail.get(msg_req.shop_type).refresh_count;
	var max_count = config.vip_json[this.game_player.player_info.vip_level]['refresh_shop_count'];
	if(count >= max_count){
		this.game_player.cplayer.respond_error_result(Msg_GC.RES_REFRESH_SHOP, Error_Code.ERROR_REFRESH_NOT_ENOUGH);
		return;
	}
	var cost = config.shop_json[msg_req.shop_type].refresh_cost[count];
	var error = 0;
	if(msg_req.shop_type == Shop_Type.COPPER_SHOP)
		error = this.game_player.bag.bag_sub_money(cost, 0);
	if(error != 0){
		this.game_player.cplayer.respond_error_result(Msg_GC.RES_REFRESH_SHOP, error);
		return;
	}
	this.refresh_shop(msg_req.shop_type, count + 1);
	
	this.game_player.set_data_change(Data_Change.SHOP_CHANGE);
}

Shop.prototype.refresh_shop = function(shop_type, refresh_count = -1){
	var shop = this.get_rand_product(config.shop_json[shop_type].product, 2);
	shop.shop_type = shop_type;
	shop.refresh_count = ( refresh_count < 0 ? 0 : refresh_count);
	this.shop_info.shop_detail.remove(shop_type);
	this.shop_info.shop_detail.insert(shop_type, shop);
}

Shop.prototype.buy_product = function(buffer){
	print('buy_product, role_id:', this.game_player.player_info.role_id, " role_name:", this.game_player.player_info.role_name, " util.now_msec:", util.now_msec());
	
	var msg_req = new MSG_120401();
	msg_req.deserialize(buffer);
	var product = this.get_product(msg_req.shop_type, msg_req.product_id);
	if(product == null){
		this.game_player.cplayer.respond_error_result(Msg_GC.RES_BUY_PRODUCT, Error_Code.ERROR_PRODUCT_NOT_EXIST);
		return;
	}
	if((product.count - msg_req.amount) < 0){
		this.game_player.cplayer.respond_error_result(Msg_GC.RES_BUY_PRODUCT, Error_Code.ERROR_PRODUCT_NOT_ENOUGH);
		return;
	}
	var error = 0;
	if(msg_req.shop_type == Shop_Type.COPPER_SHOP)
		error = this.game_player.bag.bag_sub_money(product.price * msg_req.amount, 0);
	if(error != 0){
		this.game_player.cplayer.respond_error_result(Msg_GC.RES_BUY_PRODUCT, error);
		return;
	}
	var item_arr = new Array();
	var item = new Item_Info();
	item.item_id = product.item_id;
	item.amount = msg_req.amount;
	item_arr.push(item);
	this.game_player.bag.bag_insert_item(item_arr);
	product.count -= msg_req.amount;
	
	this.game_player.set_data_change(Data_Change.SHOP_CHANGE);
}
	
Shop.prototype.get_product = function(shop_type, product_id){
	var shop = this.shop_info.shop_detail.get(shop_type);
	for(var i = 0; i < shop.products.length; i++){
		if(product_id == shop.products[i].product_id){
			return shop.products[i];
		}
	}
	return null;
}	

Shop.prototype.get_rand_product = function(base, num){
	if(num > base.length){
		print("out of product array");
		return null;
	}
	var temp = new Array();
	for(var i = 0; i < base.length; i++){
		temp.push(base[i]);
	}
	var shop = new Shop_Detail();
	for(var i = 0; i < num; i++){
		var idx = Math.floor(Math.random()*temp.length);
		var product = new Product_Info();
		product.product_id = temp[idx].product_id;
		product.price = temp[idx].price;
		product.item_id = temp[idx].item_id;
		product.count = temp[idx].count;
		shop.products.push(product);
		temp.splice(idx, 1);
	}
	return shop;
}
