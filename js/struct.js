/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.4]
*/

function Rank_Member() {
	this.role_id = 0;
	this.role_name = ""
	this.value = 0;
}

Rank_Member.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.value);
}

Rank_Member.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.role_name = buffer.read_string();
	this.value = buffer.read_int32();
}

function Role_Info() {
	this.role_id = 0;
	this.account = ""
	this.role_name = ""
	this.level = 0;
	this.exp = 0;
	this.career = 0;
	this.gender = 0;
	this.vitality = 0;
	this.buy_vitality_times = 0;
	this.vip_level = 0;
	this.vip_exp = 0;
	this.charge_gold = 0;
}

Role_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.account);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.level);
	buffer.write_int32(this.exp);
	buffer.write_int32(this.career);
	buffer.write_int32(this.gender);
	buffer.write_int32(this.vitality);
	buffer.write_int32(this.buy_vitality_times);
	buffer.write_int32(this.vip_level);
	buffer.write_int32(this.vip_exp);
	buffer.write_int32(this.charge_gold);
}

Role_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.account = buffer.read_string();
	this.role_name = buffer.read_string();
	this.level = buffer.read_int32();
	this.exp = buffer.read_int32();
	this.career = buffer.read_int32();
	this.gender = buffer.read_int32();
	this.vitality = buffer.read_int32();
	this.buy_vitality_times = buffer.read_int32();
	this.vip_level = buffer.read_int32();
	this.vip_exp = buffer.read_int32();
	this.charge_gold = buffer.read_int32();
}

function Hero_Detail() {
	this.hero_id = 0;
	this.level = 0;
	this.exp = 0;
	this.star = 0;
	this.quality = 0;
	this.energy = 0;
	this.skill_info = new Array();
	this.equip_info = new Array();
	this.property_info = new Array();
}

Hero_Detail.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.level);
	buffer.write_int32(this.exp);
	buffer.write_int32(this.star);
	buffer.write_int32(this.quality);
	buffer.write_int32(this.energy);
	var len = this.skill_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int32(this.skill_info[i]);
	}
	var len = this.equip_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.equip_info[i].serialize(buffer);
	}
	var len = this.property_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.property_info[i].serialize(buffer);
	}
}

Hero_Detail.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.level = buffer.read_int32();
	this.exp = buffer.read_int32();
	this.star = buffer.read_int32();
	this.quality = buffer.read_int32();
	this.energy = buffer.read_int32();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var skill_info_v;
		skill_info_v = buffer.read_int32();
		this.skill_info.push(skill_info_v);
	}
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var equip_info_v = new Item_Info();
		equip_info_v.deserialize(buffer);
		this.equip_info.push(equip_info_v);
	}
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var property_info_v = new Property_Detail();
		property_info_v.deserialize(buffer);
		this.property_info.push(property_info_v);
	}
}

function Item_Info() {
	this.item_id = 0;
	this.amount = 0;
	this.level = 0;
	this.exp = 0;
}

Item_Info.prototype.serialize = function(buffer) {
	buffer.write_int32(this.item_id);
	buffer.write_int32(this.amount);
	buffer.write_int32(this.level);
	buffer.write_int32(this.exp);
}

Item_Info.prototype.deserialize = function(buffer) {
	this.item_id = buffer.read_int32();
	this.amount = buffer.read_int32();
	this.level = buffer.read_int32();
	this.exp = buffer.read_int32();
}

function Property_Detail() {
	this.type = 0;
	this.name = 0;
}

Property_Detail.prototype.serialize = function(buffer) {
	buffer.write_int32(this.type);
	buffer.write_int32(this.name);
}

Property_Detail.prototype.deserialize = function(buffer) {
	this.type = buffer.read_int32();
	this.name = buffer.read_int32();
}

function Mail_Detail() {
	this.pickup = 0;
	this.mail_id = 0;
	this.send_time = 0;
	this.sender_type = 0;
	this.sender_id = 0;
	this.sender_name = ""
	this.mail_title = ""
	this.mail_content = ""
	this.copper = 0;
	this.gold = 0;
	this.item_info = new Array();
}

Mail_Detail.prototype.serialize = function(buffer) {
	buffer.write_bool(this.pickup);
	buffer.write_int32(this.mail_id);
	buffer.write_int32(this.send_time);
	buffer.write_int32(this.sender_type);
	buffer.write_int64(this.sender_id);
	buffer.write_string(this.sender_name);
	buffer.write_string(this.mail_title);
	buffer.write_string(this.mail_content);
	buffer.write_int32(this.copper);
	buffer.write_int32(this.gold);
	var len = this.item_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.item_info[i].serialize(buffer);
	}
}

Mail_Detail.prototype.deserialize = function(buffer) {
	this.pickup = buffer.read_bool();
	this.mail_id = buffer.read_int32();
	this.send_time = buffer.read_int32();
	this.sender_type = buffer.read_int32();
	this.sender_id = buffer.read_int64();
	this.sender_name = buffer.read_string();
	this.mail_title = buffer.read_string();
	this.mail_content = buffer.read_string();
	this.copper = buffer.read_int32();
	this.gold = buffer.read_int32();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var item_info_v = new Item_Info();
		item_info_v.deserialize(buffer);
		this.item_info.push(item_info_v);
	}
}

function Shop_Detail() {
	this.shop_type = 0;
	this.refresh_count = 0;
	this.products = new Array();
}

Shop_Detail.prototype.serialize = function(buffer) {
	buffer.write_int32(this.shop_type);
	buffer.write_int32(this.refresh_count);
	var len = this.products.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.products[i].serialize(buffer);
	}
}

Shop_Detail.prototype.deserialize = function(buffer) {
	this.shop_type = buffer.read_int32();
	this.refresh_count = buffer.read_int32();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var products_v = new Product_Info();
		products_v.deserialize(buffer);
		this.products.push(products_v);
	}
}

function Product_Info() {
	this.product_id = 0;
	this.item_id = 0;
	this.price = 0;
	this.count = 0;
}

Product_Info.prototype.serialize = function(buffer) {
	buffer.write_int32(this.product_id);
	buffer.write_int32(this.item_id);
	buffer.write_int32(this.price);
	buffer.write_int32(this.count);
}

Product_Info.prototype.deserialize = function(buffer) {
	this.product_id = buffer.read_int32();
	this.item_id = buffer.read_int32();
	this.price = buffer.read_int32();
	this.count = buffer.read_int32();
}

function Guild_Member_Detail() {
	this.role_id = 0;
	this.role_name = ""
	this.level = 0;
	this.career = 0;
}

Guild_Member_Detail.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.level);
	buffer.write_int32(this.career);
}

Guild_Member_Detail.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.role_name = buffer.read_string();
	this.level = buffer.read_int32();
	this.career = buffer.read_int32();
}

function Game_Player_Info() {
	this.role_id = 0;
	this.account = ""
	this.role_name = ""
	this.client_ip = ""
	this.agent_num = 0;
	this.server_num = 0;
	this.level = 0;
	this.exp = 0;
	this.gender = 0;
	this.career = 0;
	this.create_time = 0;
	this.last_sign_in_time = 0;
	this.last_sign_out_time = 0;
	this.vitality = 0;
	this.buy_vitality_times = 0;
	this.vip_level = 0;
	this.vip_exp = 0;
	this.charge_gold = 0;
	this.guild_id = 0;
	this.guild_name = ""
}

Game_Player_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.account);
	buffer.write_string(this.role_name);
	buffer.write_string(this.client_ip);
	buffer.write_int32(this.agent_num);
	buffer.write_int32(this.server_num);
	buffer.write_int32(this.level);
	buffer.write_int32(this.exp);
	buffer.write_int32(this.gender);
	buffer.write_int32(this.career);
	buffer.write_int32(this.create_time);
	buffer.write_int32(this.last_sign_in_time);
	buffer.write_int32(this.last_sign_out_time);
	buffer.write_int32(this.vitality);
	buffer.write_int32(this.buy_vitality_times);
	buffer.write_int32(this.vip_level);
	buffer.write_int32(this.vip_exp);
	buffer.write_int32(this.charge_gold);
	buffer.write_int32(this.guild_id);
	buffer.write_string(this.guild_name);
}

Game_Player_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.account = buffer.read_string();
	this.role_name = buffer.read_string();
	this.client_ip = buffer.read_string();
	this.agent_num = buffer.read_int32();
	this.server_num = buffer.read_int32();
	this.level = buffer.read_int32();
	this.exp = buffer.read_int32();
	this.gender = buffer.read_int32();
	this.career = buffer.read_int32();
	this.create_time = buffer.read_int32();
	this.last_sign_in_time = buffer.read_int32();
	this.last_sign_out_time = buffer.read_int32();
	this.vitality = buffer.read_int32();
	this.buy_vitality_times = buffer.read_int32();
	this.vip_level = buffer.read_int32();
	this.vip_exp = buffer.read_int32();
	this.charge_gold = buffer.read_int32();
	this.guild_id = buffer.read_int32();
	this.guild_name = buffer.read_string();
}

function Hero_Info() {
	this.role_id = 0;
	this.hero_map = new Map();
}

Hero_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_uint16(this.hero_map.size());
	this.hero_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Hero_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Hero_Detail();
		_v.deserialize(buffer);
		this.hero_map.insert(_v.hero_id, _v);
	}
}

function Bag_Info() {
	this.role_id = 0;
	this.copper = 0;
	this.gold = 0;
	this.item_map = new Map();
}

Bag_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_int32(this.copper);
	buffer.write_int32(this.gold);
	buffer.write_uint16(this.item_map.size());
	this.item_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Bag_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.copper = buffer.read_int32();
	this.gold = buffer.read_int32();
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Item_Info();
		_v.deserialize(buffer);
		this.item_map.insert(_v.item_id, _v);
	}
}

function Mail_Info() {
	this.role_id = 0;
	this.total_count = 0;
	this.mail_map = new Map();
}

Mail_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_int32(this.total_count);
	buffer.write_uint16(this.mail_map.size());
	this.mail_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Mail_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.total_count = buffer.read_int32();
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Mail_Detail();
		_v.deserialize(buffer);
		this.mail_map.insert(_v.mail_id, _v);
	}
}

function Shop_Info() {
	this.role_id = 0;
	this.shop_map = new Map();
}

Shop_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_uint16(this.shop_map.size());
	this.shop_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Shop_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Shop_Detail();
		_v.deserialize(buffer);
		this.shop_map.insert(_v.shop_type, _v);
	}
}

function Rank_Info() {
	this.rank_type = 0;
	this.min_value = 0;
	this.min_role_id = 0;
	this.member_map = new Map();
}

Rank_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.rank_type);
	buffer.write_int32(this.min_value);
	buffer.write_int64(this.min_role_id);
	buffer.write_uint16(this.member_map.size());
	this.member_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Rank_Info.prototype.deserialize = function(buffer) {
	this.rank_type = buffer.read_int64();
	this.min_value = buffer.read_int32();
	this.min_role_id = buffer.read_int64();
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Rank_Member();
		_v.deserialize(buffer);
		this.member_map.insert(_v.role_id, _v);
	}
}

function Guild_Info() {
	this.guild_id = 0;
	this.change = 0;
	this.guild_name = ""
	this.chief_id = 0;
	this.applicant_list = new Array();
	this.member_list = new Array();
}

Guild_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
	buffer.write_bool(this.change);
	buffer.write_string(this.guild_name);
	buffer.write_int64(this.chief_id);
	var len = this.applicant_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.applicant_list[i].serialize(buffer);
	}
	var len = this.member_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.member_list[i].serialize(buffer);
	}
}

Guild_Info.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
	this.change = buffer.read_bool();
	this.guild_name = buffer.read_string();
	this.chief_id = buffer.read_int64();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var applicant_list_v = new Guild_Member_Detail();
		applicant_list_v.deserialize(buffer);
		this.applicant_list.push(applicant_list_v);
	}
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var member_list_v = new Guild_Member_Detail();
		member_list_v.deserialize(buffer);
		this.member_list.push(member_list_v);
	}
}

function Offline_Info() {
	this.role_id = 0;
	this.guild_id = 0;
	this.guild_name = ""
	this.flag = 0;
}

Offline_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_int64(this.guild_id);
	buffer.write_string(this.guild_name);
	buffer.write_int16(this.flag);
}

Offline_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.guild_id = buffer.read_int64();
	this.guild_name = buffer.read_string();
	this.flag = buffer.read_int16();
}

function Master_Player_Info() {
	this.role_id = 0;
	this.account = ""
	this.role_name = ""
	this.level = 0;
	this.gender = 0;
	this.career = 0;
	this.vip_level = 0;
}

Master_Player_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.account);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.level);
	buffer.write_int32(this.gender);
	buffer.write_int32(this.career);
	buffer.write_int32(this.vip_level);
}

Master_Player_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.account = buffer.read_string();
	this.role_name = buffer.read_string();
	this.level = buffer.read_int32();
	this.gender = buffer.read_int32();
	this.career = buffer.read_int32();
	this.vip_level = buffer.read_int32();
}

function MSG_150001() {
	this.account = ""
}

MSG_150001.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
}

MSG_150001.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
}

function MSG_550001() {
}

MSG_550001.prototype.serialize = function(buffer) {
}

MSG_550001.prototype.deserialize = function(buffer) {
}

function MSG_150002() {
	this.player_info = new Game_Player_Info();
}

MSG_150002.prototype.serialize = function(buffer) {
	this.player_info.serialize(buffer);
}

MSG_150002.prototype.deserialize = function(buffer) {
	this.player_info.deserialize(buffer);
}

function MSG_550002() {
}

MSG_550002.prototype.serialize = function(buffer) {
}

MSG_550002.prototype.deserialize = function(buffer) {
}

function MSG_150003() {
}

MSG_150003.prototype.serialize = function(buffer) {
}

MSG_150003.prototype.deserialize = function(buffer) {
}

function MSG_550003() {
	this.role_id = 0;
}

MSG_550003.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
}

MSG_550003.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
}

function MSG_150101() {
}

MSG_150101.prototype.serialize = function(buffer) {
}

MSG_150101.prototype.deserialize = function(buffer) {
}

function MSG_150102() {
	this.table_name = ""
	this.delete_list = new Array();
}

MSG_150102.prototype.serialize = function(buffer) {
	buffer.write_string(this.table_name);
	var len = this.delete_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int64(this.delete_list[i]);
	}
}

MSG_150102.prototype.deserialize = function(buffer) {
	this.table_name = buffer.read_string();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var delete_list_v;
		delete_list_v = buffer.read_int64();
		this.delete_list.push(delete_list_v);
	}
}

function MSG_150103() {
	this.guild_list = new Array();
}

MSG_150103.prototype.serialize = function(buffer) {
	var len = this.guild_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.guild_list[i].serialize(buffer);
	}
}

MSG_150103.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var guild_list_v = new Guild_Info();
		guild_list_v.deserialize(buffer);
		this.guild_list.push(guild_list_v);
	}
}

function MSG_550103() {
	this.guild_list = new Array();
}

MSG_550103.prototype.serialize = function(buffer) {
	var len = this.guild_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.guild_list[i].serialize(buffer);
	}
}

MSG_550103.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var guild_list_v = new Guild_Info();
		guild_list_v.deserialize(buffer);
		this.guild_list.push(guild_list_v);
	}
}

function MSG_150104() {
	this.offline_list = new Array();
}

MSG_150104.prototype.serialize = function(buffer) {
	var len = this.offline_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.offline_list[i].serialize(buffer);
	}
}

MSG_150104.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var offline_list_v = new Offline_Info();
		offline_list_v.deserialize(buffer);
		this.offline_list.push(offline_list_v);
	}
}

function MSG_550104() {
	this.offline_list = new Array();
}

MSG_550104.prototype.serialize = function(buffer) {
	var len = this.offline_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.offline_list[i].serialize(buffer);
	}
}

MSG_550104.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var offline_list_v = new Offline_Info();
		offline_list_v.deserialize(buffer);
		this.offline_list.push(offline_list_v);
	}
}

function MSG_150105() {
	this.rank_list = new Array();
}

MSG_150105.prototype.serialize = function(buffer) {
	var len = this.rank_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.rank_list[i].serialize(buffer);
	}
}

MSG_150105.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var rank_list_v = new Rank_Info();
		rank_list_v.deserialize(buffer);
		this.rank_list.push(rank_list_v);
	}
}

function MSG_550105() {
	this.rank_list = new Array();
}

MSG_550105.prototype.serialize = function(buffer) {
	var len = this.rank_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.rank_list[i].serialize(buffer);
	}
}

MSG_550105.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var rank_list_v = new Rank_Info();
		rank_list_v.deserialize(buffer);
		this.rank_list.push(rank_list_v);
	}
}

function MSG_180001() {
	this.role_id = 0;
	this.role_name = ""
	this.account = ""
	this.level = 0;
	this.client_ip = ""
	this.login_time = 0;
	this.logout_time = 0;
}

MSG_180001.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.role_name);
	buffer.write_string(this.account);
	buffer.write_uint16(this.level);
	buffer.write_string(this.client_ip);
	buffer.write_int32(this.login_time);
	buffer.write_int32(this.logout_time);
}

MSG_180001.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.role_name = buffer.read_string();
	this.account = buffer.read_string();
	this.level = buffer.read_uint16();
	this.client_ip = buffer.read_string();
	this.login_time = buffer.read_int32();
	this.logout_time = buffer.read_int32();
}

function MSG_160000() {
	this.player_info = new Master_Player_Info();
}

MSG_160000.prototype.serialize = function(buffer) {
	this.player_info.serialize(buffer);
}

MSG_160000.prototype.deserialize = function(buffer) {
	this.player_info.deserialize(buffer);
}

function MSG_160001() {
	this.role_id = 0;
}

MSG_160001.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
}

MSG_160001.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
}

function MSG_160100() {
	this.guild_id = 0;
	this.guild_name = ""
}

MSG_160100.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
	buffer.write_string(this.guild_name);
}

MSG_160100.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
	this.guild_name = buffer.read_string();
}

function MSG_165000() {
	this.level = 0;
}

MSG_165000.prototype.serialize = function(buffer) {
	buffer.write_int32(this.level);
}

MSG_165000.prototype.deserialize = function(buffer) {
	this.level = buffer.read_int32();
}

function MSG_140000() {
	this.account = ""
	this.session = ""
	this.gate_ip = ""
	this.gate_port = 0;
}

MSG_140000.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_string(this.session);
	buffer.write_string(this.gate_ip);
	buffer.write_int32(this.gate_port);
}

MSG_140000.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.session = buffer.read_string();
	this.gate_ip = buffer.read_string();
	this.gate_port = buffer.read_int32();
}

function MSG_140001() {
	this.account = ""
}

MSG_140001.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
}

MSG_140001.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
}

function MSG_140100() {
}

MSG_140100.prototype.serialize = function(buffer) {
}

MSG_140100.prototype.deserialize = function(buffer) {
}

function MSG_140200() {
	this.player_info = new Master_Player_Info();
}

MSG_140200.prototype.serialize = function(buffer) {
	this.player_info.serialize(buffer);
}

MSG_140200.prototype.deserialize = function(buffer) {
	this.player_info.deserialize(buffer);
}

function MSG_300000() {
}

MSG_300000.prototype.serialize = function(buffer) {
}

MSG_300000.prototype.deserialize = function(buffer) {
}

function MSG_300001() {
	this.player_level = 0;
	this.player_exp = 0;
}

MSG_300001.prototype.serialize = function(buffer) {
	buffer.write_int32(this.player_level);
	buffer.write_int32(this.player_exp);
}

MSG_300001.prototype.deserialize = function(buffer) {
	this.player_level = buffer.read_int32();
	this.player_exp = buffer.read_int32();
}

function MSG_300002() {
	this.vip_level = 0;
	this.vip_exp = 0;
}

MSG_300002.prototype.serialize = function(buffer) {
	buffer.write_int32(this.vip_level);
	buffer.write_int32(this.vip_exp);
}

MSG_300002.prototype.deserialize = function(buffer) {
	this.vip_level = buffer.read_int32();
	this.vip_exp = buffer.read_int32();
}

function MSG_300100() {
	this.copper = 0;
	this.gold = 0;
}

MSG_300100.prototype.serialize = function(buffer) {
	buffer.write_int32(this.copper);
	buffer.write_int32(this.gold);
}

MSG_300100.prototype.deserialize = function(buffer) {
	this.copper = buffer.read_int32();
	this.gold = buffer.read_int32();
}

function MSG_300101() {
	this.item_info = new Array();
}

MSG_300101.prototype.serialize = function(buffer) {
	var len = this.item_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.item_info[i].serialize(buffer);
	}
}

MSG_300101.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var item_info_v = new Item_Info();
		item_info_v.deserialize(buffer);
		this.item_info.push(item_info_v);
	}
}

function MSG_300200() {
	this.mail_info = new Array();
}

MSG_300200.prototype.serialize = function(buffer) {
	var len = this.mail_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.mail_info[i].serialize(buffer);
	}
}

MSG_300200.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var mail_info_v = new Mail_Detail();
		mail_info_v.deserialize(buffer);
		this.mail_info.push(mail_info_v);
	}
}

function MSG_300300() {
	this.property_info = new Array();
}

MSG_300300.prototype.serialize = function(buffer) {
	var len = this.property_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.property_info[i].serialize(buffer);
	}
}

MSG_300300.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var property_info_v = new Property_Detail();
		property_info_v.deserialize(buffer);
		this.property_info.push(property_info_v);
	}
}

function MSG_120001() {
	this.account = ""
	this.role_id = 0;
	this.timestamp = ""
	this.ip = ""
	this.agent_num = 0;
	this.server_num = 0;
}

MSG_120001.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_int64(this.role_id);
	buffer.write_string(this.timestamp);
	buffer.write_string(this.ip);
	buffer.write_int32(this.agent_num);
	buffer.write_int32(this.server_num);
}

MSG_120001.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.role_id = buffer.read_int64();
	this.timestamp = buffer.read_string();
	this.ip = buffer.read_string();
	this.agent_num = buffer.read_int32();
	this.server_num = buffer.read_int32();
}

function MSG_520001() {
	this.role_info = new Role_Info();
}

MSG_520001.prototype.serialize = function(buffer) {
	this.role_info.serialize(buffer);
}

MSG_520001.prototype.deserialize = function(buffer) {
	this.role_info.deserialize(buffer);
}

function MSG_120002() {
	this.account = ""
	this.role_name = ""
	this.gender = 0;
}

MSG_120002.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_string(this.role_name);
	buffer.write_uint8(this.gender);
}

MSG_120002.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.role_name = buffer.read_string();
	this.gender = buffer.read_uint8();
}

function MSG_520002() {
	this.role_id = 0;
}

MSG_520002.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
}

MSG_520002.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
}

function MSG_120003() {
}

MSG_120003.prototype.serialize = function(buffer) {
}

MSG_120003.prototype.deserialize = function(buffer) {
}

function MSG_520003() {
	this.vitality = 0;
}

MSG_520003.prototype.serialize = function(buffer) {
	buffer.write_int32(this.vitality);
}

MSG_520003.prototype.deserialize = function(buffer) {
	this.vitality = buffer.read_int32();
}

function MSG_120100() {
}

MSG_120100.prototype.serialize = function(buffer) {
}

MSG_120100.prototype.deserialize = function(buffer) {
}

function MSG_520100() {
	this.item_info = new Array();
}

MSG_520100.prototype.serialize = function(buffer) {
	var len = this.item_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.item_info[i].serialize(buffer);
	}
}

MSG_520100.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var item_info_v = new Item_Info();
		item_info_v.deserialize(buffer);
		this.item_info.push(item_info_v);
	}
}

function MSG_120101() {
	this.item = new Item_Info();
}

MSG_120101.prototype.serialize = function(buffer) {
	this.item.serialize(buffer);
}

MSG_120101.prototype.deserialize = function(buffer) {
	this.item.deserialize(buffer);
}

function MSG_520101() {
}

MSG_520101.prototype.serialize = function(buffer) {
}

MSG_520101.prototype.deserialize = function(buffer) {
}

function MSG_120102() {
	this.item = new Item_Info();
}

MSG_120102.prototype.serialize = function(buffer) {
	this.item.serialize(buffer);
}

MSG_120102.prototype.deserialize = function(buffer) {
	this.item.deserialize(buffer);
}

function MSG_520102() {
}

MSG_520102.prototype.serialize = function(buffer) {
}

MSG_520102.prototype.deserialize = function(buffer) {
}

function MSG_120200() {
}

MSG_120200.prototype.serialize = function(buffer) {
}

MSG_120200.prototype.deserialize = function(buffer) {
}

function MSG_520200() {
	this.mail_info = new Array();
}

MSG_520200.prototype.serialize = function(buffer) {
	var len = this.mail_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.mail_info[i].serialize(buffer);
	}
}

MSG_520200.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var mail_info_v = new Mail_Detail();
		mail_info_v.deserialize(buffer);
		this.mail_info.push(mail_info_v);
	}
}

function MSG_120201() {
	this.mail_id = 0;
}

MSG_120201.prototype.serialize = function(buffer) {
	buffer.write_int32(this.mail_id);
}

MSG_120201.prototype.deserialize = function(buffer) {
	this.mail_id = buffer.read_int32();
}

function MSG_520201() {
	this.mail_id_info = new Array();
}

MSG_520201.prototype.serialize = function(buffer) {
	var len = this.mail_id_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int32(this.mail_id_info[i]);
	}
}

MSG_520201.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var mail_id_info_v;
		mail_id_info_v = buffer.read_int32();
		this.mail_id_info.push(mail_id_info_v);
	}
}

function MSG_120202() {
	this.mail_id = 0;
}

MSG_120202.prototype.serialize = function(buffer) {
	buffer.write_int32(this.mail_id);
}

MSG_120202.prototype.deserialize = function(buffer) {
	this.mail_id = buffer.read_int32();
}

function MSG_520202() {
	this.mail_id_info = new Array();
}

MSG_520202.prototype.serialize = function(buffer) {
	var len = this.mail_id_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int32(this.mail_id_info[i]);
	}
}

MSG_520202.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var mail_id_info_v;
		mail_id_info_v = buffer.read_int32();
		this.mail_id_info.push(mail_id_info_v);
	}
}

function MSG_120203() {
	this.receiver_name = ""
	this.mail_detail = new Mail_Detail();
}

MSG_120203.prototype.serialize = function(buffer) {
	buffer.write_string(this.receiver_name);
	this.mail_detail.serialize(buffer);
}

MSG_120203.prototype.deserialize = function(buffer) {
	this.receiver_name = buffer.read_string();
	this.mail_detail.deserialize(buffer);
}

function MSG_520203() {
}

MSG_520203.prototype.serialize = function(buffer) {
}

MSG_520203.prototype.deserialize = function(buffer) {
}

function MSG_120300() {
}

MSG_120300.prototype.serialize = function(buffer) {
}

MSG_120300.prototype.deserialize = function(buffer) {
}

function MSG_520300() {
	this.hero_info = new Array();
}

MSG_520300.prototype.serialize = function(buffer) {
	var len = this.hero_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.hero_info[i].serialize(buffer);
	}
}

MSG_520300.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var hero_info_v = new Hero_Detail();
		hero_info_v.deserialize(buffer);
		this.hero_info.push(hero_info_v);
	}
}

function MSG_120301() {
	this.hero_id = 0;
}

MSG_120301.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
}

MSG_120301.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
}

function MSG_520301() {
	this.hero_id = 0;
	this.star = 0;
}

MSG_520301.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.star);
}

MSG_520301.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.star = buffer.read_int32();
}

function MSG_120302() {
	this.hero_id = 0;
}

MSG_120302.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
}

MSG_120302.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
}

function MSG_520302() {
	this.hero_id = 0;
	this.quality = 0;
}

MSG_520302.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.quality);
}

MSG_520302.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.quality = buffer.read_int32();
}

function MSG_120303() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.item_info = new Array();
}

MSG_120303.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.equip_index);
	var len = this.item_info.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.item_info[i].serialize(buffer);
	}
}

MSG_120303.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.equip_index = buffer.read_int32();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var item_info_v = new Item_Info();
		item_info_v.deserialize(buffer);
		this.item_info.push(item_info_v);
	}
}

function MSG_520303() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.equip_level = 0;
	this.equip_exp = 0;
}

MSG_520303.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.equip_index);
	buffer.write_int32(this.equip_level);
	buffer.write_int32(this.equip_exp);
}

MSG_520303.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.equip_index = buffer.read_int32();
	this.equip_level = buffer.read_int32();
	this.equip_exp = buffer.read_int32();
}

function MSG_120304() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.on = 0;
	this.equip_info = new Item_Info();
}

MSG_120304.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.equip_index);
	buffer.write_bool(this.on);
	this.equip_info.serialize(buffer);
}

MSG_120304.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.equip_index = buffer.read_int32();
	this.on = buffer.read_bool();
	this.equip_info.deserialize(buffer);
}

function MSG_520304() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.on = 0;
	this.equip_info = new Item_Info();
}

MSG_520304.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.equip_index);
	buffer.write_bool(this.on);
	this.equip_info.serialize(buffer);
}

MSG_520304.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.equip_index = buffer.read_int32();
	this.on = buffer.read_bool();
	this.equip_info.deserialize(buffer);
}

function MSG_120305() {
	this.hero_id = 0;
	this.skill_id = 0;
}

MSG_120305.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.skill_id);
}

MSG_120305.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.skill_id = buffer.read_int32();
}

function MSG_520305() {
	this.hero_id = 0;
	this.skill_id = 0;
}

MSG_520305.prototype.serialize = function(buffer) {
	buffer.write_int32(this.hero_id);
	buffer.write_int32(this.skill_id);
}

MSG_520305.prototype.deserialize = function(buffer) {
	this.hero_id = buffer.read_int32();
	this.skill_id = buffer.read_int32();
}

function MSG_120400() {
	this.shopype = 0;
}

MSG_120400.prototype.serialize = function(buffer) {
	buffer.write_int32(this.shopype);
}

MSG_120400.prototype.deserialize = function(buffer) {
	this.shopype = buffer.read_int32();
}

function MSG_520400() {
	this.shop = new Shop_Detail();
}

MSG_520400.prototype.serialize = function(buffer) {
	this.shop.serialize(buffer);
}

MSG_520400.prototype.deserialize = function(buffer) {
	this.shop.deserialize(buffer);
}

function MSG_120401() {
	this.shopype = 0;
	this.product_id = 0;
	this.amount = 0;
}

MSG_120401.prototype.serialize = function(buffer) {
	buffer.write_int32(this.shopype);
	buffer.write_int32(this.product_id);
	buffer.write_int32(this.amount);
}

MSG_120401.prototype.deserialize = function(buffer) {
	this.shopype = buffer.read_int32();
	this.product_id = buffer.read_int32();
	this.amount = buffer.read_int32();
}

function MSG_520401() {
}

MSG_520401.prototype.serialize = function(buffer) {
}

MSG_520401.prototype.deserialize = function(buffer) {
}

function MSG_120402() {
	this.shopype = 0;
}

MSG_120402.prototype.serialize = function(buffer) {
	buffer.write_int32(this.shopype);
}

MSG_120402.prototype.deserialize = function(buffer) {
	this.shopype = buffer.read_int32();
}

function MSG_520402() {
}

MSG_520402.prototype.serialize = function(buffer) {
}

MSG_520402.prototype.deserialize = function(buffer) {
}

function MSG_100000() {
	this.account = ""
	this.password = ""
}

MSG_100000.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_string(this.password);
}

MSG_100000.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.password = buffer.read_string();
}

function MSG_500000() {
	this.ip = ""
	this.port = 0;
	this.session = ""
}

MSG_500000.prototype.serialize = function(buffer) {
	buffer.write_string(this.ip);
	buffer.write_int32(this.port);
	buffer.write_string(this.session);
}

MSG_500000.prototype.deserialize = function(buffer) {
	this.ip = buffer.read_string();
	this.port = buffer.read_int32();
	this.session = buffer.read_string();
}

function MSG_100001() {
	this.account = ""
	this.password = ""
}

MSG_100001.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_string(this.password);
}

MSG_100001.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.password = buffer.read_string();
}

function MSG_500001() {
	this.ip = ""
	this.port = 0;
	this.session = ""
}

MSG_500001.prototype.serialize = function(buffer) {
	buffer.write_string(this.ip);
	buffer.write_int32(this.port);
	buffer.write_string(this.session);
}

MSG_500001.prototype.deserialize = function(buffer) {
	this.ip = buffer.read_string();
	this.port = buffer.read_int32();
	this.session = buffer.read_string();
}

function MSG_100002() {
	this.account = ""
	this.session = ""
}

MSG_100002.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_string(this.session);
}

MSG_100002.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.session = buffer.read_string();
}

function MSG_500002() {
	this.account = ""
}

MSG_500002.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
}

MSG_500002.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
}

function MSG_100003() {
	this.client_time = 0;
}

MSG_100003.prototype.serialize = function(buffer) {
	buffer.write_int32(this.client_time);
}

MSG_100003.prototype.deserialize = function(buffer) {
	this.client_time = buffer.read_int32();
}

function MSG_500003() {
	this.client_time = 0;
	this.server_time = 0;
}

MSG_500003.prototype.serialize = function(buffer) {
	buffer.write_int32(this.client_time);
	buffer.write_int32(this.server_time);
}

MSG_500003.prototype.deserialize = function(buffer) {
	this.client_time = buffer.read_int32();
	this.server_time = buffer.read_int32();
}

function MSG_110001() {
	this.chatype = 0;
	this.chat_content = ""
	this.role_name = ""
}

MSG_110001.prototype.serialize = function(buffer) {
	buffer.write_int8(this.chatype);
	buffer.write_string(this.chat_content);
	buffer.write_string(this.role_name);
}

MSG_110001.prototype.deserialize = function(buffer) {
	this.chatype = buffer.read_int8();
	this.chat_content = buffer.read_string();
	this.role_name = buffer.read_string();
}

function MSG_510001() {
	this.chatype = 0;
	this.chat_content = ""
	this.role_name = ""
	this.gender = 0;
	this.career = 0;
	this.vip_level = 0;
}

MSG_510001.prototype.serialize = function(buffer) {
	buffer.write_int8(this.chatype);
	buffer.write_string(this.chat_content);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.gender);
	buffer.write_int32(this.career);
	buffer.write_int32(this.vip_level);
}

MSG_510001.prototype.deserialize = function(buffer) {
	this.chatype = buffer.read_int8();
	this.chat_content = buffer.read_string();
	this.role_name = buffer.read_string();
	this.gender = buffer.read_int32();
	this.career = buffer.read_int32();
	this.vip_level = buffer.read_int32();
}

function MSG_110101() {
	this.guild_name = ""
}

MSG_110101.prototype.serialize = function(buffer) {
	buffer.write_string(this.guild_name);
}

MSG_110101.prototype.deserialize = function(buffer) {
	this.guild_name = buffer.read_string();
}

function MSG_510101() {
	this.guild_id = 0;
}

MSG_510101.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
}

MSG_510101.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
}

function MSG_110102() {
	this.guild_id = 0;
}

MSG_110102.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
}

MSG_110102.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
}

function MSG_510102() {
	this.guild_id = 0;
}

MSG_510102.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
}

MSG_510102.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
}

function MSG_110103() {
	this.guild_id = 0;
}

MSG_110103.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
}

MSG_110103.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
}

function MSG_510103() {
}

MSG_510103.prototype.serialize = function(buffer) {
}

MSG_510103.prototype.deserialize = function(buffer) {
}

function MSG_110104() {
	this.guild_id = 0;
	this.role_id = 0;
	this.allow = 0;
}

MSG_110104.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
	buffer.write_int64(this.role_id);
	buffer.write_bool(this.allow);
}

MSG_110104.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
	this.role_id = buffer.read_int64();
	this.allow = buffer.read_bool();
}

function MSG_510104() {
}

MSG_510104.prototype.serialize = function(buffer) {
}

MSG_510104.prototype.deserialize = function(buffer) {
}

function MSG_110105() {
	this.guild_id = 0;
	this.role_id = 0;
}

MSG_110105.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
	buffer.write_int64(this.role_id);
}

MSG_110105.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
	this.role_id = buffer.read_int64();
}

function MSG_510105() {
}

MSG_510105.prototype.serialize = function(buffer) {
}

MSG_510105.prototype.deserialize = function(buffer) {
}

function MSG_110201() {
	this.type = 0;
}

MSG_110201.prototype.serialize = function(buffer) {
	buffer.write_int32(this.type);
}

MSG_110201.prototype.deserialize = function(buffer) {
	this.type = buffer.read_int32();
}

function MSG_510201() {
	this.rank_type = 0;
	this.self_rank = 0;
	this.rank_list = new Array();
}

MSG_510201.prototype.serialize = function(buffer) {
	buffer.write_int32(this.rank_type);
	buffer.write_int32(this.self_rank);
	var len = this.rank_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.rank_list[i].serialize(buffer);
	}
}

MSG_510201.prototype.deserialize = function(buffer) {
	this.rank_type = buffer.read_int32();
	this.self_rank = buffer.read_int32();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var rank_list_v = new Rank_Member();
		rank_list_v.deserialize(buffer);
		this.rank_list.push(rank_list_v);
	}
}
