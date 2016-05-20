/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.3]
*/

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
	this.skill_point = 0;
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
	buffer.write_int32(this.skill_point);
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
	this.skill_point = buffer.read_int32();
}

function Property_Detail() {
	this.type = 0;
	this.value = 0;
}

Property_Detail.prototype.serialize = function(buffer) {
	buffer.write_int32(this.type);
	buffer.write_int32(this.value);
}

Property_Detail.prototype.deserialize = function(buffer) {
	this.type = buffer.read_int32();
	this.value = buffer.read_int32();
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

function Account_Info() {
	this.account = ""
	this.agent_num = 0;
	this.server_num = 0;
}

Account_Info.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_int32(this.agent_num);
	buffer.write_int32(this.server_num);
}

Account_Info.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.agent_num = buffer.read_int32();
	this.server_num = buffer.read_int32();
}

function Ip_Info() {
	this.ip = ""
	this.port = 0;
}

Ip_Info.prototype.serialize = function(buffer) {
	buffer.write_string(this.ip);
	buffer.write_int32(this.port);
}

Ip_Info.prototype.deserialize = function(buffer) {
	this.ip = buffer.read_string();
	this.port = buffer.read_int32();
}

function Player_DB_Cache() {
	this.role_id = 0;
	this.account = ""
	this.role_name = ""
	this.agent_num = 0;
	this.server_num = 0;
	this.level = 0;
	this.gender = 0;
	this.career = 0;
}

Player_DB_Cache.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.account);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.agent_num);
	buffer.write_int32(this.server_num);
	buffer.write_int32(this.level);
	buffer.write_int32(this.gender);
	buffer.write_int32(this.career);
}

Player_DB_Cache.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.account = buffer.read_string();
	this.role_name = buffer.read_string();
	this.agent_num = buffer.read_int32();
	this.server_num = buffer.read_int32();
	this.level = buffer.read_int32();
	this.gender = buffer.read_int32();
	this.career = buffer.read_int32();
}

function Login_Player_Info() {
	this.account = ""
	this.gate_ip = ""
	this.gate_port = 0;
	this.session = ""
	this.session_tick = 0;
}

Login_Player_Info.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_string(this.gate_ip);
	buffer.write_int32(this.gate_port);
	buffer.write_string(this.session);
	buffer.write_int64(this.session_tick);
}

Login_Player_Info.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.gate_ip = buffer.read_string();
	this.gate_port = buffer.read_int32();
	this.session = buffer.read_string();
	this.session_tick = buffer.read_int64();
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
	this.skill_point = 0;
	this.recover_skill_time = 0;
	this.exchange_count = 0;
	this.guild_id = 0;
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
	buffer.write_int32(this.skill_point);
	buffer.write_int64(this.recover_skill_time);
	buffer.write_int32(this.exchange_count);
	buffer.write_int32(this.guild_id);
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
	this.skill_point = buffer.read_int32();
	this.recover_skill_time = buffer.read_int64();
	this.exchange_count = buffer.read_int32();
	this.guild_id = buffer.read_int32();
}

function Hero_Info() {
	this.hero_map = new Map();
}

Hero_Info.prototype.serialize = function(buffer) {
	buffer.write_uint16(this.hero_map.size());
	this.hero_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Hero_Info.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Hero_Detail();
		_v.deserialize(buffer);
		this.hero_map.insert(_v.hero_id, _v);
	}
}

function Bag_Info() {
	this.copper = 0;
	this.gold = 0;
	this.item_map = new Map();
}

Bag_Info.prototype.serialize = function(buffer) {
	buffer.write_int32(this.copper);
	buffer.write_int32(this.gold);
	buffer.write_uint16(this.item_map.size());
	this.item_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Bag_Info.prototype.deserialize = function(buffer) {
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
	this.total_count = 0;
	this.mail_map = new Map();
}

Mail_Info.prototype.serialize = function(buffer) {
	buffer.write_int32(this.total_count);
	buffer.write_uint16(this.mail_map.size());
	this.mail_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Mail_Info.prototype.deserialize = function(buffer) {
	this.total_count = buffer.read_int32();
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Mail_Detail();
		_v.deserialize(buffer);
		this.mail_map.insert(_v.mail_id, _v);
	}
}

function Shop_Info() {
	this.shop_detail = new Map();
}

Shop_Info.prototype.serialize = function(buffer) {
	buffer.write_uint16(this.shop_detail.size());
	this.shop_detail.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Shop_Info.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Shop_Detail();
		_v.deserialize(buffer);
		this.shop_detail.insert(_v.shop_type, _v);
	}
}

function Player_Data() {
	this.status = 0;
	this.change_module = new Array();
	this.player_info = new Game_Player_Info();
	this.hero_info = new Hero_Info();
	this.bag_info = new Bag_Info();
	this.mail_info = new Mail_Info();
	this.shop_info = new Shop_Info();
}

Player_Data.prototype.serialize = function(buffer) {
	buffer.write_int8(this.status);
	var len = this.change_module.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int32(this.change_module[i]);
	}
	this.player_info.serialize(buffer);
	this.hero_info.serialize(buffer);
	this.bag_info.serialize(buffer);
	this.mail_info.serialize(buffer);
	this.shop_info.serialize(buffer);
}

Player_Data.prototype.deserialize = function(buffer) {
	this.status = buffer.read_int8();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var change_module_v;
		change_module_v = buffer.read_int32();
		this.change_module.push(change_module_v);
	}
	this.player_info.deserialize(buffer);
	this.hero_info.deserialize(buffer);
	this.bag_info.deserialize(buffer);
	this.mail_info.deserialize(buffer);
	this.shop_info.deserialize(buffer);
}

function Member_Info() {
	this.role_id = 0;
	this.role_name = ""
	this.level = 0;
	this.career = 0;
}

Member_Info.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.level);
	buffer.write_int32(this.career);
}

Member_Info.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.role_name = buffer.read_string();
	this.level = buffer.read_int32();
	this.career = buffer.read_int32();
}

function Guild_Data() {
	this.guild_id = 0;
	this.guild_name = ""
	this.chief_id = 0;
	this.applicant_list = new Array();
	this.member_list = new Array();
}

Guild_Data.prototype.serialize = function(buffer) {
	buffer.write_int64(this.guild_id);
	buffer.write_string(this.guild_name);
	buffer.write_int64(this.chief_id);
	var len = this.applicant_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int64(this.applicant_list[i]);
	}
	var len = this.member_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.member_list[i].serialize(buffer);
	}
}

Guild_Data.prototype.deserialize = function(buffer) {
	this.guild_id = buffer.read_int64();
	this.guild_name = buffer.read_string();
	this.chief_id = buffer.read_int64();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var applicant_list_v;
		applicant_list_v = buffer.read_int64();
		this.applicant_list.push(applicant_list_v);
	}
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var member_list_v = new Member_Info();
		member_list_v.deserialize(buffer);
		this.member_list.push(member_list_v);
	}
}

function Guild_Info() {
	this.guild_map = new Map();
}

Guild_Info.prototype.serialize = function(buffer) {
	buffer.write_uint16(this.guild_map.size());
	this.guild_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Guild_Info.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Guild_Data();
		_v.deserialize(buffer);
		this.guild_map.insert(_v.guild_id, _v);
	}
}

function Offline_Msg_Detail() {
	this.role_id = 0;
	this.guild_msg = new Array();
}

Offline_Msg_Detail.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	var len = this.guild_msg.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int64(this.guild_msg[i]);
	}
}

Offline_Msg_Detail.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var guild_msg_v;
		guild_msg_v = buffer.read_int64();
		this.guild_msg.push(guild_msg_v);
	}
}

function Offline_Msg() {
	this.msg_map = new Map();
}

Offline_Msg.prototype.serialize = function(buffer) {
	buffer.write_uint16(this.msg_map.size());
	this.msg_map.each(function(_key,_v,index) {
		_v.serialize(buffer);
	});
}

Offline_Msg.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for (var i = 0; i < len; ++i) {
		var _v = new Offline_Msg_Detail();
		_v.deserialize(buffer);
		this.msg_map.insert(_v.role_id, _v);
	}
}
