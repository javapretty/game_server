/*
*	描述：脚本结构体
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function Player_Info() {
	this.role_id = 0;						//玩家id
	this.account = "";						//玩家账号名
	this.role_name = "";					//玩家名字
	this.client_ip = "";					//客户端ip
	this.agent_num = 0;						//平台编号
	this.server_num = 0;					//服务器编号
	this.level = 0;     			  		//玩家等级
	this.exp = 0;									//玩家经验
	this.gender = 0; 							//0(女),1(男)
	this.career = 0; 							//职业1-3
	this.create_time = 0;					//创建角色时刻
	this.last_sign_in_time = 0;		//最后登录时间
	this.last_sign_out_time = 0;		//最后登出时间
	this.vitality = 0;						//玩家体力
	this.last_change_time = 0;				//上次更改体力时间
	this.today_buy = 0;						//当前购买次数
	this.vip = 0;									//vip等级
	this.vip_exp = 0;								//vip对应的经验值
	this.charge_gold = 0;					//总共充值的元宝数
	
	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_string(this.account);
		buffer.write_string(this.role_name);
		buffer.write_string(this.client_ip);
		buffer.write_int32(this.agent_num);
		buffer.write_int32(this.server_num);
		buffer.write_int32(this.level);
		buffer.write_int32(this.exp);
		buffer.write_uint32(this.gender);
		buffer.write_uint32(this.career);
		buffer.write_int32(this.create_time);
		buffer.write_int32(this.last_sign_in_time);
		buffer.write_int32(this.last_sign_out_time);
		buffer.write_int32(this.vitality);
		buffer.write_int64(this.last_change_time);
		buffer.write_int32(this.today_buy);
		buffer.write_int32(this.vip);
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
		this.vip = buffer.read_int32();
		this.vip_exp = buffer.read_int32();
		this.charge_gold = buffer.read_int32();
	}
}

//英雄属性信息
function Property_Detail() {
	this.type = 0;				//属性类型
	this.value = 0;				//属性值
	
	this.serialize = function(buffer) {
		buffer.write_int32(type);
		buffer.write_int32(value);
	}
	
	this.deserialize = function(buffer) {
		this.type = buffer.read_int32();
		this.value = buffer.read_int32();
	}
}

function Item_Info() {
	this.item_id = 0;				//道具id
	this.amount = 0;				//道具数量
	this.level = 0;					//道具等级
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
	this.hero_id = 0;				//	英雄id
	this.level = 0;					//英雄等级
	this.exp = 0;						//英雄经验
	this.star = 0;					//英雄星级
	this.quality = 0;				//英雄品质级别
	this.equip_info = new Array();			//英雄装备信息
	this.property_info = new Array();	//英雄属性信息
	
	this.serialize = function(buffer) {
		buffer.write_int32(this.hero_id);
		buffer.write_int32(this.level);
		buffer.write_int32(this.exp);
		buffer.write_int32(this.star);
		buffer.write_int32(this.quality);
		var equip_len = this.equip_info.length;
		buffer.write_uint16(equip_len);
		for (var i = 0; i < equip_len; ++i) {
			this.equip_info[i].serialize(buffer);
		}
		var property_len = this.property_info.length;
		buffer.write_uint16(property_len);
		for (var i = 0; i < property_len; ++i) {
			this.property_info[i].serialize(buffer);
		}
	}
	
	this.deserialize = function(buffer) {
		this.hero_id = buffer.read_int32();
		this.level = buffer.read_int32();
		this.exp = buffer.read_int32();
		this.star = buffer.read_int32();
		this.quality = buffer.read_int32();
		var equip_len = buffer.read_uint16();
		for (var i = 0; i < equip_len; ++i) {
			var item_info = new Item_Info();
			item_info.deserialize(buffer);
			this.equip_info.push(item_info);
		}
		var property_len = buffer.read_uint16();
		for (var i = 0; i < property_len; ++i) {
			var property_detail = new Property_Detail();
			property_detail.deserialize(buffer);
			this.property_info.push(property_detail);
		}
	}
}

function Hero_Info() {
	this.hero_map = new Map();			//英雄信息
	
	this.serialize = function(buffer) {
		buffer.write_uint16(this.hero_map.size());
		this.hero_map.each(function(key,value,index) {
			value.serialize(buffer);
     	});
	}
	
	this.deserialize = function(buffer) {
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var hero_detail = new Hero_Detail();
			hero_detail.deserialize(buffer);
			this.hero_map.put(hero_detail.hero_id, hero_detail);
		}
	}
}

function Bag_Info() {
	this.copper = 0; 						//铜钱
	this.gold = 0;							//元宝
	this.item_map = new Map();		//物品信息
	
	this.serialize = function(buffer) {
		buffer.write_int32(this.copper);
		buffer.write_int32(this.gold);
		buffer.write_uint16(this.item_map.size());
		this.item_map.each(function(key,value,index) {
			value.serialize(buffer);
     	});
	}
	
	this.deserialize = function(buffer) {
		this.copper = buffer.read_int32();
		this.gold = buffer.read_int32();
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var item_info = new Item_Info();
			item_info.deserialize(buffer);
			this.mail_map.put(item_info.id, item_info);
		}
	}
}

function Mail_Detail() {
	this.pickup = 0; 				//是否收取过附件
	this.mail_id = 0; 			//邮件id
	this.send_time = 0; 		//发邮件时间
	this.sender_type = 0; 	//发件人类型
	this.sender_id = 0; 		//发件人id
	this.sender_name = ""; 	//发件人名字
	this.mail_title = ""; 	//邮件标题
	this.mail_content = ""; //邮件内容
	this.copper = 0;				//铜钱
	this.gold = 0;					//元宝
	this.item_info = new Array();	//道具信息
	
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
		for (var i = 0; i < len; ++i) {
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
		for (var i = 0; i < len; ++i) {
			var item = new Item_Info;
			item.deserialize(buffer);
			this.item_info.push(item);
		}
    };
}

function Mail_Info() {
	this.total_count = 0; 			//邮件的总数量，即目前为止收到的所有邮件数
	this.mail_map = new Map();		//邮件信息
	
	this.serialize = function(buffer) {
		buffer.write_int32(this.total_count);
		buffer.write_uint16(this.mail_map.size());
		this.mail_map.each(function(key,value,index) {
			value.serialize(buffer);
     	});
	}
	
	this.deserialize = function(buffer) {
		this.total_count = buffer.read_int32();
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var mail_detail = new Mail_Detail();
			mail_detail.deserialize(buffer);
			this.mail_map.put(mail_detail.mail_id, mail_detail);
		}
	}
}