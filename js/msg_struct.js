/** 
* struct JS file description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.0]
*
*/

function MSG_300000() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_300001() {
	this.vip_level = 0;
	this.vip_exp = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.vip_level);
		buffer.write_int32(this.vip_exp);
	}

	this.deserialize = function(buffer) {
		this.vip_level = buffer.read_int32();
		this.vip_exp = buffer.read_int32();
	}
}

function MSG_300100() {
	this.copper = 0;
	this.gold = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.copper);
		buffer.write_int32(this.gold);
	}

	this.deserialize = function(buffer) {
		this.copper = buffer.read_int32();
		this.gold = buffer.read_int32();
	}
}

function MSG_300101() {
	this.item_info = new Array();

	this.serialize = function(buffer) {
		var len = this.item_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			this.item_info[i].serialize(buffer);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var item_info_v = new Item_Info();
			item_info_v.deserialize(buffer);
			this.item_info.push(item_info_v);
		}
	}
}

function MSG_300200() {
	this.mail_info = new Array();

	this.serialize = function(buffer) {
		var len = this.mail_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			this.mail_info[i].serialize(buffer);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var mail_info_v = new Mail_Detail();
			mail_info_v.deserialize(buffer);
			this.mail_info.push(mail_info_v);
		}
	}
}

function MSG_300300() {
	this.property_info = new Array();

	this.serialize = function(buffer) {
		var len = this.property_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			this.property_info[i].serialize(buffer);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var property_info_v = new Property_Detail();
			property_info_v.deserialize(buffer);
			this.property_info.push(property_info_v);
		}
	}
}
/** 
* struct JS file description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.0]
*
*/

function MSG_120001() {
	this.account = ""
	this.role_id = 0;
	this.timestamp = ""
	this.ip = ""
	this.agent_num = 0;
	this.server_num = 0;

	this.serialize = function(buffer) {
		buffer.write_string(this.account);
		buffer.write_int64(this.role_id);
		buffer.write_string(this.timestamp);
		buffer.write_string(this.ip);
		buffer.write_int32(this.agent_num);
		buffer.write_int32(this.server_num);
	}

	this.deserialize = function(buffer) {
		this.account = buffer.read_string();
		this.role_id = buffer.read_int64();
		this.timestamp = buffer.read_string();
		this.ip = buffer.read_string();
		this.agent_num = buffer.read_int32();
		this.server_num = buffer.read_int32();
	}
}

function MSG_520001() {
	this.role_info = new Role_Info();

	this.serialize = function(buffer) {
		this.role_info.serialize(buffer);
	}

	this.deserialize = function(buffer) {
		this.role_info.deserialize(buffer);
	}
}

function MSG_120002() {
	this.account = ""
	this.role_name = ""
	this.gender = 0;
	this.agent_num = 0;
	this.server_num = 0;

	this.serialize = function(buffer) {
		buffer.write_string(this.account);
		buffer.write_string(this.role_name);
		buffer.write_uint8(this.gender);
		buffer.write_int32(this.agent_num);
		buffer.write_int32(this.server_num);
	}

	this.deserialize = function(buffer) {
		this.account = buffer.read_string();
		this.role_name = buffer.read_string();
		this.gender = buffer.read_uint8();
		this.agent_num = buffer.read_int32();
		this.server_num = buffer.read_int32();
	}
}

function MSG_520002() {
	this.role_id = 0;

	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
	}

	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
	}
}

function MSG_120003() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_520003() {
	this.vitality = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.vitality);
	}

	this.deserialize = function(buffer) {
		this.vitality = buffer.read_int32();
	}
}

function MSG_120100() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_520100() {
	this.item_info = new Array();

	this.serialize = function(buffer) {
		var len = this.item_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			this.item_info[i].serialize(buffer);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var item_info_v = new Item_Info();
			item_info_v.deserialize(buffer);
			this.item_info.push(item_info_v);
		}
	}
}

function MSG_120101() {
	this.item = new Item_Info();

	this.serialize = function(buffer) {
		this.item.serialize(buffer);
	}

	this.deserialize = function(buffer) {
		this.item.deserialize(buffer);
	}
}

function MSG_520101() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_120102() {
	this.item = new Item_Info();

	this.serialize = function(buffer) {
		this.item.serialize(buffer);
	}

	this.deserialize = function(buffer) {
		this.item.deserialize(buffer);
	}
}

function MSG_520102() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_120200() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_520200() {
	this.mail_info = new Array();

	this.serialize = function(buffer) {
		var len = this.mail_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			this.mail_info[i].serialize(buffer);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var mail_info_v = new Mail_Detail();
			mail_info_v.deserialize(buffer);
			this.mail_info.push(mail_info_v);
		}
	}
}

function MSG_120201() {
	this.mail_id = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.mail_id);
	}

	this.deserialize = function(buffer) {
		this.mail_id = buffer.read_int32();
	}
}

function MSG_520201() {
	this.mail_id_info = new Array();

	this.serialize = function(buffer) {
		var len = this.mail_id_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			buffer.write_int32(this.mail_id_info[i]);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var mail_id_info_v;
			mail_id_info_v = buffer.read_int32();
			this.mail_id_info.push(mail_id_info_v);
		}
	}
}

function MSG_120202() {
	this.mail_id = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.mail_id);
	}

	this.deserialize = function(buffer) {
		this.mail_id = buffer.read_int32();
	}
}

function MSG_520202() {
	this.mail_id_info = new Array();

	this.serialize = function(buffer) {
		var len = this.mail_id_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			buffer.write_int32(this.mail_id_info[i]);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var mail_id_info_v;
			mail_id_info_v = buffer.read_int32();
			this.mail_id_info.push(mail_id_info_v);
		}
	}
}

function MSG_120203() {
	this.receiver_name = ""
	this.mail_detail = new Mail_Detail();

	this.serialize = function(buffer) {
		buffer.write_string(this.receiver_name);
		this.mail_detail.serialize(buffer);
	}

	this.deserialize = function(buffer) {
		this.receiver_name = buffer.read_string();
		this.mail_detail.deserialize(buffer);
	}
}

function MSG_520203() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_120300() {

	this.serialize = function(buffer) {
	}

	this.deserialize = function(buffer) {
	}
}

function MSG_520300() {
	this.hero_info = new Array();

	this.serialize = function(buffer) {
		var len = this.hero_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			this.hero_info[i].serialize(buffer);
		}
	}

	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var hero_info_v = new Hero_Detail();
			hero_info_v.deserialize(buffer);
			this.hero_info.push(hero_info_v);
		}
	}
}

function MSG_120301() {
	this.hero_id = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
	}
}

function MSG_520301() {
	this.hero_id = 0;
	this.star = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.star);
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.star = buffer.read_int32();
	}
}

function MSG_120302() {
	this.hero_id = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
	}
}

function MSG_520302() {
	this.hero_id = 0;
	this.quality = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.quality);
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.quality = buffer.read_int32();
	}
}

function MSG_120303() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.item_info = new Array();

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.equip_index);
		var len = this.item_info.length;
		buffer.write_uint16(len);
		for(var i = 0; i < len; ++i) {
			this.item_info[i].serialize(buffer);
		}
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.equip_index = buffer.read_int32();
		var len = buffer.read_uint16();
		for(var i = 0; i < len; ++i) {
			var item_info_v = new Item_Info();
			item_info_v.deserialize(buffer);
			this.item_info.push(item_info_v);
		}
	}
}

function MSG_520303() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.equip_level = 0;
	this.equip_exp = 0;

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.equip_index);
		buffer.write_int32(this.equip_level);
		buffer.write_int32(this.equip_exp);
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.equip_index = buffer.read_int32();
		this.equip_level = buffer.read_int32();
		this.equip_exp = buffer.read_int32();
	}
}

function MSG_120304() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.on = 0;
	this.equip_info = new Item_Info();

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.equip_index);
		buffer.write_bool(this.on);
		this.equip_info.serialize(buffer);
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.equip_index = buffer.read_int32();
		this.on = buffer.read_bool();
		this.equip_info.deserialize(buffer);
	}
}

function MSG_520304() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.on = 0;
	this.equip_info = new Item_Info();

	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.equip_index);
		buffer.write_bool(this.on);
		this.equip_info.serialize(buffer);
	}

	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.equip_index = buffer.read_int32();
		this.on = buffer.read_bool();
		this.equip_info.deserialize(buffer);
	}
}
