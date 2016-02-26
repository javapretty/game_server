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
	
	this.bag_add_money = function(copper, gold) {
		this.bag_info.copper += copper;
		this.bag_info.gold += gold;
	}
	
	this.bag_sub_money = function(copper, gold) {
		this.bag_info.copper -= copper;
		this.bag_info.gold -= gold;
	}
	
	this.bag_add_item = function(item) {
		this.bag_info.bag_map = put(item.id, item);
	}
	
	this.bag_erase_item = function(item) {
		this.bag_info.bag_map.remove(item.id);
	}
}