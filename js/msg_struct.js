/*
*	描述：消息结构体
*	作者：张亚磊
*	时间：2016/03/04
*	qq:784599938
*	tel:18268193219
*/

//物品信息更新
function MSG_300100() {
	this.item_info = new Array();
	
	this.serialize = function(buffer) {
		var len = this.item_info.length;
		buffer.write_uint16(len);
		for (var i = 0; i < len; ++i) {
			this.item_info[i].serialize(buffer);
		}
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var item = new Item_Info();
			item.deserialize(buffer);
			this.item_info.push(item);
		}
	}
}

//物品附加信息更新
function MSG_300101() {
	this.serialize = function(buffer) {
	}
	
	this.deserialize = function(buffer) {
	}
}

//金钱更新
function MSG_300102() {
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

//邮件信息更新
function MSG_300200() {
	this.mail_info = new Array();
	
	this.serialize = function(buffer) {
		var len = this.mail_info.length;
		buffer.write_uint16(len);
		for (var i = 0; i < len; ++i) {
			this.mail_info[i].serialize(buffer);
		}
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var mail = new Mail_Detail();
			mail.deserialize(buffer);
			this.mail_info.push(mail);
		}
	}
}

//获取背包信息
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
		for (var i = 0; i < len; ++i) {
			this.item_info[i].serialize(buffer);
		}
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var item = new Item_Info();
			item.deserialize(buffer);
			this.item_info.push(item);
		}
	}
}

//使用物品
function MSG_120101() {
	this.item = new Item_Info();

	this.serialize = function(buffer) {
		buffer.write_int32(this.item.id);
		buffer.write_int32(this.item.amount);
	}
	
	this.deserialize = function(buffer) {
		this.item.id = buffer.read_int32();
		this.item.amount = buffer.read_int32();
	}
}

function MSG_520101() {
	this.serialize = function(buffer) {
	}
	
	this.deserialize = function(buffer) {
	}
}

//使用物品
function MSG_120102() {
	this.item = new Item_Info();

	this.serialize = function(buffer) {
		buffer.write_int32(this.item.id);
		buffer.write_int32(this.item.amount);
	}
	
	this.deserialize = function(buffer) {
		this.item.id = buffer.read_int32();
		this.item.amount = buffer.read_int32();
	}
}

function MSG_520102() {
	this.serialize = function(buffer) {
	}
	
	this.deserialize = function(buffer) {
	}
}

//获取邮件信息
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
		for (var i = 0; i < len; ++i) {
			this.mail_info[i].serialize(buffer);
		}
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var mail = new Mail_Detail();
			mail.deserialize(buffer);
			this.mail_info.push(mail);
		}
	}
}

//收取附件
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
		for (var i = 0; i < len; ++i) {
			buffer.write_int32(this.mail_id_info[i]);
		}
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var mail_id = buffer.read_int32();
			this.mail_id_info.push(mail_id);
		}
	}
}

//删除邮件
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
		for (var i = 0; i < len; ++i) {
			buffer.write_int32(this.mail_id_info[i]);
		}
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var mail_id = buffer.read_int32();
			this.mail_id_info.push(mail_id);
		}
	}
}

//发送邮件
function MSG_120203() {
	this.receiver_name = "";
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

//获取英雄信息
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
		for (var i = 0; i < len; ++i) {
			this.hero_info[i].serialize(buffer);
		}
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var hero = new Hero_Detail();
			hero.deserialize(buffer);
			this.hero_info.push(hero);
		}
	}
}

//提高英雄星级
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

//提高英雄品质
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

//提高英雄装备等级
function MSG_120303() {
	this.hero_id = 0;
	this.equip_index = 0;
	this.item_info = new Array();
	
	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.equip_index);
		var length = this.item_info.length;
		bufer.write_uint16(length);
		for (var i = 0; i < length; ++i) {
			this.item_info[i].serialize(buffer);
		}
	}
	
	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.equip_index = buffer.read_int32();
		var length = buffer.read_uint16();
		for (var i = 0; i < length; ++i) {
			var item = new Item_Info();
			item.deserialize(buffer);
			this.item_info.push(item);
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
	this.on = 0;
	this.equip_info = new Item_Info();
	
	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_bool(this.on);
		equip_info.serialize(buffer);
	}
	
	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.on = buffer.read_bool();
		equip_info.deserialize(buffer);
	}
}

function MSG_520304() {
	this.hero_id = 0;
	this.on = 0;
	this.equip_info = new Item_Info();
	
	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_bool(this.on);
		equip_info.serialize(buffer);
	}
	
	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.on = buffer.read_bool();
		equip_info.deserialize(buffer);
	}
}