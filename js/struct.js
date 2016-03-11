/** 
* struct JS file description
* 
* [This file was auto-generated. Please Do not edit]
*
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
	this.today_buy = 0;
	this.vip_level = 0;
	this.vip_exp = 0;
	this.charge_gold = 0;

	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_string(this.account);
		buffer.write_string(this.role_name);
		buffer.write_int32(this.level);
		buffer.write_int32(this.exp);
		buffer.write_int32(this.career);
		buffer.write_int32(this.gender);
		buffer.write_int32(this.vitality);
		buffer.write_int32(this.today_buy);
		buffer.write_int32(this.vip_level);
		buffer.write_int32(this.vip_exp);
		buffer.write_int32(this.charge_gold);
	}

	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
		this.account = buffer.read_string();
		this.role_name = buffer.read_string();
		this.level = buffer.read_int32();
		this.exp = buffer.read_int32();
		this.career = buffer.read_int32();
		this.gender = buffer.read_int32();
		this.vitality = buffer.read_int32();
		this.today_buy = buffer.read_int32();
		this.vip_level = buffer.read_int32();
		this.vip_exp = buffer.read_int32();
		this.charge_gold = buffer.read_int32();
	}
}

function Property_Detail() {
	this.type = 0;
	this.value = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.type);
		buffer.write_int32(this.value);
	}

	this.deserialize = function(buffer) {
		this.type = buffer.read_int32();
		this.value = buffer.read_int32();
	}
}

function Item_Info() {
	this.item_id = 0;
	this.amount = 0;
	this.level = 0;
	this.exp = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.item_id);
		buffer.write_int32(this.amount);
		buffer.write_int32(this.level);
		buffer.write_int32(this.exp);
	}

	this.deserialize = function(buffer) {
		this.item_id = buffer.read_int32();
		this.amount = buffer.read_int32();
		this.level = buffer.read_int32();
		this.exp = buffer.read_int32();
	}
}

function Hero_Detail() {
	this.hero_id = 0;
	this.level = 0;
	this.exp = 0;
	this.star = 0;
	this.quality = 0;
	this.equip_info = new Array();
	this.property_info = new Array();

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.level);
		buffer.write_int32(this.exp);
		buffer.write_int32(this.star);
		buffer.write_int32(this.quality);
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

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.level = buffer.read_int32();
		this.exp = buffer.read_int32();
		this.star = buffer.read_int32();
		this.quality = buffer.read_int32();
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

	this.serialize = function(buffer) {
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

	this.deserialize = function(buffer) {
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
}
/** 
* struct JS file description
* 
* [This file was auto-generated. Please Do not edit]
*
*/

function Account_Info() {
	this.account = ""
	this.agent_num = 0;
	this.server_num = 0;

	this.serialize = function(buffer) {
		buffer.write_string(this.account);
		buffer.write_int32(this.agent_num);
		buffer.write_int32(this.server_num);
	}

	this.deserialize = function(buffer) {
		this.account = buffer.read_string();
		this.agent_num = buffer.read_int32();
		this.server_num = buffer.read_int32();
	}
}

function Ip_Info() {
	this.ip = ""
	this.port = 0;

	this.serialize = function(buffer) {
		buffer.write_string(this.ip);
		buffer.write_int32(this.port);
	}

	this.deserialize = function(buffer) {
		this.ip = buffer.read_string();
		this.port = buffer.read_int32();
	}
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

	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_string(this.account);
		buffer.write_string(this.role_name);
		buffer.write_int32(this.agent_num);
		buffer.write_int32(this.server_num);
		buffer.write_int32(this.level);
		buffer.write_int32(this.gender);
		buffer.write_int32(this.career);
	}

	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
		this.account = buffer.read_string();
		this.role_name = buffer.read_string();
		this.agent_num = buffer.read_int32();
		this.server_num = buffer.read_int32();
		this.level = buffer.read_int32();
		this.gender = buffer.read_int32();
		this.career = buffer.read_int32();
	}
}

function Login_Player_Info() {
	this.account = ""
	this.gate_ip = ""
	this.gate_port = 0;
	this.session = ""

	this.serialize = function(buffer) {
		this.session_tick.serialize(buffer);
		buffer.write_string(this.account);
		buffer.write_string(this.gate_ip);
		buffer.write_int32(this.gate_port);
		buffer.write_string(this.session);
	}

	this.deserialize = function(buffer) {
		this.session_tick.deserialize(buffer);
		this.account = buffer.read_string();
		this.gate_ip = buffer.read_string();
		this.gate_port = buffer.read_int32();
		this.session = buffer.read_string();
	}
}

function Gate_Player_Info() {
	this.role_id = 0;
	this.account = ""

	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_string(this.account);
	}

	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
		this.account = buffer.read_string();
	}
}

function Master_Player_Info() {
	this.role_id = 0;
	this.account = ""
	this.role_name = ""
	this.level = 0;

	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_string(this.account);
		buffer.write_string(this.role_name);
		buffer.write_int32(this.level);
	}

	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
		this.account = buffer.read_string();
		this.role_name = buffer.read_string();
		this.level = buffer.read_int32();
	}
}

function Player_Info() {
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
	this.last_change_time = 0;
	this.today_buy = 0;
	this.vip_level = 0;
	this.vip_exp = 0;
	this.charge_gold = 0;

	this.serialize = function(buffer) {
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
		buffer.write_int64(this.last_change_time);
		buffer.write_int32(this.today_buy);
		buffer.write_int32(this.vip_level);
		buffer.write_int32(this.vip_exp);
		buffer.write_int32(this.charge_gold);
	}

	this.deserialize = function(buffer) {
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
		this.last_change_time = buffer.read_int64();
		this.today_buy = buffer.read_int32();
		this.vip_level = buffer.read_int32();
		this.vip_exp = buffer.read_int32();
		this.charge_gold = buffer.read_int32();
	}
}

function Hero_Info() {
	this.hero_map = new Map();

	this.serialize = function(buffer) {
		buffer.write_uint16(this.hero_map.size());
		this.hero_map.each(function(_key,_v,index) {
			_v.serialize(buffer);
		});
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var _v = new Hero_Detail();
			_v.deserialize(buffer);
			this.hero_map.put(_v.hero_id, _v);
		}
	}
}

function Bag_Info() {
	this.copper = 0;
	this.gold = 0;
	this.item_map = new Map();

	this.serialize = function(buffer) {
		buffer.write_int32(this.copper);
		buffer.write_int32(this.gold);
		buffer.write_uint16(this.item_map.size());
		this.item_map.each(function(_key,_v,index) {
			_v.serialize(buffer);
		});
	}

	this.deserialize = function(buffer) {
		this.copper = buffer.read_int32();
		this.gold = buffer.read_int32();
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var _v = new Item_Info();
			_v.deserialize(buffer);
			this.item_map.put(_v.item_id, _v);
		}
	}
}

function Mail_Info() {
	this.total_count = 0;
	this.mail_map = new Map();

	this.serialize = function(buffer) {
		buffer.write_int32(this.total_count);
		buffer.write_uint16(this.mail_map.size());
		this.mail_map.each(function(_key,_v,index) {
			_v.serialize(buffer);
		});
	}

	this.deserialize = function(buffer) {
		this.total_count = buffer.read_int32();
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var _v = new Mail_Detail();
			_v.deserialize(buffer);
			this.mail_map.put(_v.mail_id, _v);
		}
	}
}
