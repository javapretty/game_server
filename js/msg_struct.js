/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.3]
*/

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
	this.agent_num = 0;
	this.server_num = 0;
}

MSG_120002.prototype.serialize = function(buffer) {
	buffer.write_string(this.account);
	buffer.write_string(this.role_name);
	buffer.write_uint8(this.gender);
	buffer.write_int32(this.agent_num);
	buffer.write_int32(this.server_num);
}

MSG_120002.prototype.deserialize = function(buffer) {
	this.account = buffer.read_string();
	this.role_name = buffer.read_string();
	this.gender = buffer.read_uint8();
	this.agent_num = buffer.read_int32();
	this.server_num = buffer.read_int32();
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

function MSG_120004() {
}

MSG_120004.prototype.serialize = function(buffer) {
}

MSG_120004.prototype.deserialize = function(buffer) {
}

function MSG_520004() {
	this.copper = 0;
}

MSG_520004.prototype.serialize = function(buffer) {
	buffer.write_int32(this.copper);
}

MSG_520004.prototype.deserialize = function(buffer) {
	this.copper = buffer.read_int32();
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

function MSG_120306() {
	this.exp = 0;
}

MSG_120306.prototype.serialize = function(buffer) {
	buffer.write_int32(this.exp);
}

MSG_120306.prototype.deserialize = function(buffer) {
	this.exp = buffer.read_int32();
}

function MSG_520306() {
}

MSG_520306.prototype.serialize = function(buffer) {
}

MSG_520306.prototype.deserialize = function(buffer) {
}

function MSG_120400() {
	this.shop_type = 0;
}

MSG_120400.prototype.serialize = function(buffer) {
	buffer.write_uint32(this.shop_type);
}

MSG_120400.prototype.deserialize = function(buffer) {
	this.shop_type = buffer.read_uint32();
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
	this.shop_type = 0;
	this.product_id = 0;
	this.amount = 0;
}

MSG_120401.prototype.serialize = function(buffer) {
	buffer.write_uint32(this.shop_type);
	buffer.write_uint32(this.product_id);
	buffer.write_uint32(this.amount);
}

MSG_120401.prototype.deserialize = function(buffer) {
	this.shop_type = buffer.read_uint32();
	this.product_id = buffer.read_uint32();
	this.amount = buffer.read_uint32();
}

function MSG_520401() {
}

MSG_520401.prototype.serialize = function(buffer) {
}

MSG_520401.prototype.deserialize = function(buffer) {
}

function MSG_120402() {
	this.shop_type = 0;
}

MSG_120402.prototype.serialize = function(buffer) {
	buffer.write_uint32(this.shop_type);
}

MSG_120402.prototype.deserialize = function(buffer) {
	this.shop_type = buffer.read_uint32();
}

function MSG_520402() {
}

MSG_520402.prototype.serialize = function(buffer) {
}

MSG_520402.prototype.deserialize = function(buffer) {
}

function MSG_110001() {
	this.chat_type = 0;
	this.chat_content = ""
	this.role_name = ""
}

MSG_110001.prototype.serialize = function(buffer) {
	buffer.write_int8(this.chat_type);
	buffer.write_string(this.chat_content);
	buffer.write_string(this.role_name);
}

MSG_110001.prototype.deserialize = function(buffer) {
	this.chat_type = buffer.read_int8();
	this.chat_content = buffer.read_string();
	this.role_name = buffer.read_string();
}

function MSG_510001() {
	this.chat_type = 0;
	this.chat_content = ""
	this.role_name = ""
	this.gender = 0;
	this.career = 0;
	this.vip_level = 0;
}

MSG_510001.prototype.serialize = function(buffer) {
	buffer.write_int8(this.chat_type);
	buffer.write_string(this.chat_content);
	buffer.write_string(this.role_name);
	buffer.write_int32(this.gender);
	buffer.write_int32(this.career);
	buffer.write_int32(this.vip_level);
}

MSG_510001.prototype.deserialize = function(buffer) {
	this.chat_type = buffer.read_int8();
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
	this.rank = 0;
	this.rank_list = new Rank_List();
}

MSG_510201.prototype.serialize = function(buffer) {
	buffer.write_int32(this.rank);
	this.rank_list.serialize(buffer);
}

MSG_510201.prototype.deserialize = function(buffer) {
	this.rank = buffer.read_int32();
	this.rank_list.deserialize(buffer);
}

function MSG_150000() {
}

MSG_150000.prototype.serialize = function(buffer) {
}

MSG_150000.prototype.deserialize = function(buffer) {
}

function MSG_550000() {
	this.db_cache_vec = new Array();
}

MSG_550000.prototype.serialize = function(buffer) {
	var len = this.db_cache_vec.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		this.db_cache_vec[i].serialize(buffer);
	}
}

MSG_550000.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var db_cache_vec_v = new Player_DB_Cache();
		db_cache_vec_v.deserialize(buffer);
		this.db_cache_vec.push(db_cache_vec_v);
	}
}

function MSG_150001() {
	this.account_info = new Account_Info();
}

MSG_150001.prototype.serialize = function(buffer) {
	this.account_info.serialize(buffer);
}

MSG_150001.prototype.deserialize = function(buffer) {
	this.account_info.deserialize(buffer);
}

function MSG_550001() {
	this.player_data = new Player_Data();
}

MSG_550001.prototype.serialize = function(buffer) {
	this.player_data.serialize(buffer);
}

MSG_550001.prototype.deserialize = function(buffer) {
	this.player_data.deserialize(buffer);
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
	this.player_data = new Player_Data();
}

MSG_550002.prototype.serialize = function(buffer) {
	this.player_data.serialize(buffer);
}

MSG_550002.prototype.deserialize = function(buffer) {
	this.player_data.deserialize(buffer);
}

function MSG_150003() {
	this.player_data = new Player_Data();
}

MSG_150003.prototype.serialize = function(buffer) {
	this.player_data.serialize(buffer);
}

MSG_150003.prototype.deserialize = function(buffer) {
	this.player_data.deserialize(buffer);
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

function MSG_150004() {
	this.role_id = 0;
	this.mail_detail = new Mail_Detail();
}

MSG_150004.prototype.serialize = function(buffer) {
	buffer.write_int64(this.role_id);
	this.mail_detail.serialize(buffer);
}

MSG_150004.prototype.deserialize = function(buffer) {
	this.role_id = buffer.read_int64();
	this.mail_detail.deserialize(buffer);
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

function MSG_150101() {
}

MSG_150101.prototype.serialize = function(buffer) {
}

MSG_150101.prototype.deserialize = function(buffer) {
}

function MSG_550101() {
	this.guild_info = new Guild_Info();
	this.offline_info = new Offline_Info();
	this.rank_info = new Rank_Info();
}

MSG_550101.prototype.serialize = function(buffer) {
	this.guild_info.serialize(buffer);
	this.offline_info.serialize(buffer);
	this.rank_info.serialize(buffer);
}

MSG_550101.prototype.deserialize = function(buffer) {
	this.guild_info.deserialize(buffer);
	this.offline_info.deserialize(buffer);
	this.rank_info.deserialize(buffer);
}

function MSG_150102() {
	this.guild_info = new Guild_Info();
}

MSG_150102.prototype.serialize = function(buffer) {
	this.guild_info.serialize(buffer);
}

MSG_150102.prototype.deserialize = function(buffer) {
	this.guild_info.deserialize(buffer);
}

function MSG_550102() {
}

MSG_550102.prototype.serialize = function(buffer) {
}

MSG_550102.prototype.deserialize = function(buffer) {
}

function MSG_150103() {
	this.guild_list = new Array();
}

MSG_150103.prototype.serialize = function(buffer) {
	var len = this.guild_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int64(this.guild_list[i]);
	}
}

MSG_150103.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var guild_list_v;
		guild_list_v = buffer.read_int64();
		this.guild_list.push(guild_list_v);
	}
}

function MSG_550103() {
}

MSG_550103.prototype.serialize = function(buffer) {
}

MSG_550103.prototype.deserialize = function(buffer) {
}

function MSG_150104() {
	this.offline_info = new Offline_Info();
}

MSG_150104.prototype.serialize = function(buffer) {
	this.offline_info.serialize(buffer);
}

MSG_150104.prototype.deserialize = function(buffer) {
	this.offline_info.deserialize(buffer);
}

function MSG_550104() {
}

MSG_550104.prototype.serialize = function(buffer) {
}

MSG_550104.prototype.deserialize = function(buffer) {
}

function MSG_150105() {
	this.offline_list = new Array();
}

MSG_150105.prototype.serialize = function(buffer) {
	var len = this.offline_list.length;
	buffer.write_uint16(len);
	for(var i = 0; i < len; ++i) {
		buffer.write_int64(this.offline_list[i]);
	}
}

MSG_150105.prototype.deserialize = function(buffer) {
	var len = buffer.read_uint16();
	for(var i = 0; i < len; ++i) {
		var offline_list_v;
		offline_list_v = buffer.read_int64();
		this.offline_list.push(offline_list_v);
	}
}

function MSG_550105() {
}

MSG_550105.prototype.serialize = function(buffer) {
}

MSG_550105.prototype.deserialize = function(buffer) {
}

function MSG_150106() {
	this.rank_info = new Rank_Info();
}

MSG_150106.prototype.serialize = function(buffer) {
	this.rank_info.serialize(buffer);
}

MSG_150106.prototype.deserialize = function(buffer) {
	this.rank_info.deserialize(buffer);
}

function MSG_550106() {
}

MSG_550106.prototype.serialize = function(buffer) {
}

MSG_550106.prototype.deserialize = function(buffer) {
}
