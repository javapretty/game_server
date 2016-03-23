/*
*	描述：商店系统
*	作者：李俊良
*	时间：2016/03/15
*	qq:975338436
*	tel:15618920470
*/

function Shop() {
	this.player = null;
	this.shop_info = new Shop_Info();

	this.init = function(){
		var copper_shop = this.get_rand_product(shop_base['COPPER_SHOP'].PRODUCTS, 2);
		copper_shop.shop_type = Shop_Type.COPPER_SHOP;
		copper_shop.fresh_count = 3;
		this.shop_info.shop_detail.put(Shop_Type.COPPER_SHOP, copper_shop);
	}
	
	this.load_data = function(player, buffer) {
		this.player = player;
		this.shop_info.deserialize(buffer);
		if(this.shop_info.shop_detail.isEmpty()){
			this.init();
		}
	}

	this.save_data = function(buffer) {
		this.shop_info.serialize(buffer);
	}

	this.set_data_change = function() {
		this.player.cplayer.set_player_data_change(Data_Change.SHOP_CHANGE);
	}

	this.fetch_shop_info = function(buffer){
		print('fetch_shop_info, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
		var msg_req = new MSG_120400();
		msg_req.deserialize(buffer);
		var msg_res = new MSG_520400();
		msg_res.shop = this.shop_info.shop_detail.get(msg_req.shop_type);
		
		var buf = pop_buffer();
		msg_res.serialize(buf);
		this.player.cplayer.respond_success_result(Msg_Res.RES_FETCH_SHOP_INFO, buf);
		push_buffer(buf);
	}

	this.refresh_shop = function(shop_type, fresh_count = -1){
		var shop;
		if(shop_type == Shop_Type.COPPER_SHOP){
			shop = this.get_rand_product(shop_base['COPPER_SHOP'].PRODUCTS, 2);
		}
		shop.shop_type = Shop_Type.COPPER_SHOP;
		shop.fresh_count = ( fresh_count < 0 ? 3 : fresh_count);
		this.shop_info.shop_detail.remove(shop_type);
		this.shop_info.shop_detail.put(shop_type, shop);
	}
	
	this.refresh_by_timer = function(){
		this.refresh_shop(Shop_Type.COPPER_SHOP);
	}

	this.refresh_by_player = function(buffer){
		print('refresh_by_player, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
		var msg_req = new MSG_120402();
		msg_req.deserialize(buffer);
		var count = this.shop_info.shop_detail.get(msg_req.shop_type).fresh_count;
		if(count <= 0){
			this.player.cplayer.respond_error_result(Msg_Res.RES_REFRESH_SHOP, Error_Code.ERROR_REFRESH_NOT_ENOUGH);
			return;
		}
		var cost;
		switch(msg_req.shop_type){
			case Shop_Type.COPPER_SHOP:
				cost = shop_base['COPPER_SHOP'].REFRESH_COST[3 - count];
				if(cost > this.player.bag.bag_info.copper){
					this.player.cplayer.respond_error_result(Msg_Res.RES_REFRESH_SHOP, Error_Code.ERROR_COPPER_NOT_ENOUGH);
					return;
				}
				this.player.bag.bag_sub_money(cost, 0);
				this.refresh_shop(Shop_Type.COPPER_SHOP, count - 1);
				this.player.bag.bag_sub_money(cost, 0);
				break;
			default:
				print("these's no shop type ", msg_req.shop_type);
				break;
		}
	}

	this.get_product = function(shop_type, product_id){
		var shop = this.shop_info.shop_detail.get(shop_type);
		for(var i = 0; i < shop.products.length; i++){
			if(product_id == shop.products[i].product_id){
				return shop.products[i];
			}
		}
		return null;
	}

	this.buy_product = function(buffer){
		print('buy_product, role_id:', this.player.player_info.role_id, " role_name:", this.player.player_info.role_name, " util.now_msec:", util.now_msec());
	
		var msg_req = new MSG_120401();
		msg_req.deserialize(buffer);
		var product = this.get_product(msg_req.shop_type, msg_req.product_id);
		if(product == null){
			this.player.cplayer.respond_error_result(Msg_Res.RES_BUY_PRODUCT, Error_Code.ERROR_PRODUCT_NOT_EXIST);
			return;
		}
		switch(msg_req.shop_type){
			case Shop_Type.COPPER_SHOP:
				this.buy_copper_product(product, msg_req.amount);
				break;
			default:
				print("these's no shop type ", msg_req.shop_type);
				break;
		}
	}

	this.buy_copper_product = function(product, amount){
		if(this.player.bag.bag_info.copper < product.price * amount){
			this.player.cplayer.respond_error_result(Msg_Res.RES_BUY_PRODUCT, Error_Code.ERROR_COPPER_NOT_ENOUGH);
			return;
		}
		if((product.count - amount) < 0){
			this.player.cplayer.respond_error_result(Msg_Res.RES_BUY_PRODUCT, Error_Code.ERROR_PRODUCT_NOT_ENOUGH);
			return;
		}
		this.player.bag.bag_sub_money(product.price, 0);
		var item_arr = new Array();
		var item = new Item_Info();
		item.id = product.item_id;
		item.amount = amount;
		item_arr.push(item);
		this.player.bag.bag_insert_item(item_arr);
		product.count -= amount;
	}

	this.get_rand_product = function(base, num){
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
}
