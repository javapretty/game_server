/*
*	描述：脚本结构体
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function Player_Info() {
	this.role_id = 0;							//玩家id
	this.account = "";						//玩家账号名
	this.role_name = "";					//玩家名字
	this.client_ip = "";					//客户端ip
	this.agent_num = 0;						//平台编号
	this.server_num = 0;					//服务器编号
	this.level = 0;     			  		//玩家等级
	this.experience = 0;					//玩家经验
	this.gender = 0; 							//0(女),1(男)
	this.career = 0; 							//职业1-3
	this.create_time = 0;					//创建角色时刻
	this.last_sign_in_time = 0;		//最后登录时间
	this.last_sign_out_time = 0;		//最后登出时间
	this.vitality = 0;						//玩家体力
	this.vip = 0;									//vip等级
	this.charge_gold = 0;					//总共充值的元宝数
	this.is_change = false;				//数据是否改变
	
	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_string(this.account);
		buffer.write_string(this.role_name);
		buffer.write_string(this.client_ip);
		buffer.write_int32(this.agent_num);
		buffer.write_int32(this.server_num);
		buffer.write_int32(this.level);
		buffer.write_int32(this.experience);
		buffer.write_uint32(this.gender);
		buffer.write_uint32(this.career);
		buffer.write_int32(this.create_time);
		buffer.write_int32(this.last_sign_in_time);
		buffer.write_int32(this.last_sign_out_time);
		buffer.write_int32(this.vitality);
		buffer.write_int32(this.vip);
		buffer.write_int32(this.charge_gold);
		buffer.write_bool(this.is_change);
	}
	
	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
		this.account = buffer.read_string();
		this.role_name = buffer.read_string();
		this.client_ip = buffer.read_string();
		this.agent_num = buffer.read_int32();
		this.server_num = buffer.read_int32();
		this.level = buffer.read_int32();
		this.experience = buffer.read_int32();
		this.gender = buffer.read_int32();
		this.career = buffer.read_int32();
		this.create_time = buffer.read_int32();
		this.last_sign_in_time = buffer.read_int32();
		this.last_sign_out_time = buffer.read_int32();
		this.vitality = buffer.read_int32();
		this.vip = buffer.read_int32();
		this.charge_gold = buffer.read_int32();
		this.is_change = buffer.read_bool();
	}
	
	this.save_change = function() {
		this.is_change = true;
	}
}

function Item_Info() {
	this.id = 0;				//物品id
	this.amount = 0;		//物品数量
	
	this.serialize = function(buffer) {
		buffer.write_int32(id);
		buffer.write_int32(amount);
	}
	
	this.deserialize = function(buffer) {
		id = buffer.read_int32();
		amount = buffer.read_int32();
	}
}

function Bag_Info() {
	this.role_id = 0;						//角色ID
	this.copper = 0; 						//铜钱
	this.gold = 0;							//元宝
	this.item_map = new Map();		//物品信息
	this.is_change = false;			//数据是否改变
	
	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_int32(this.copper);
		buffer.write_int32(this.gold);
		buffer.write_uint16(this.item_map.size());
		this.item_map.each(function(key,value,index) {
			value.serialize(buffer);
     	});
		buffer.write_bool(this.is_change);
	}
	
	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
		this.copper = buffer.read_int32();
		this.gold = buffer.read_int32();
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var item_info = new Item_Info();
			item_info.deserialize(buffer);
			this.mail_map.put(item_info.id, item_info);
		}
		this.is_change = buffer.read_bool();
	}
	
	this.save_change = function() {
		this.is_change = true;
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
    };
}

function Mail_Info() {
	this.role_id = 0;						//角色ID
	this.total_count = 0; 			//邮件的总数量，即目前为止收到的所有邮件数
	this.mail_map = new Map();		//邮件信息
	this.is_change = false;			//数据是否改变
	
	this.serialize = function(buffer) {
		buffer.write_int64(this.role_id);
		buffer.write_int32(this.total_count);
		buffer.write_uint16(this.mail_map.size());
		this.mail_map.each(function(key,value,index) {
			value.serialize(buffer);
     	});
		buffer.write_bool(this.is_change);
	}
	
	this.deserialize = function(buffer) {
		this.role_id = buffer.read_int64();
		this.total_count = buffer.read_int32();
		var len = buffer.read_uint16();
		for (var i = 0; i < len; ++i) {
			var mail_detail = new Mail_Detail();
			mail_detail.deserialize(buffer);
			this.mail_map.put(mail_detail.mail_id, mail_detail);
		}
		this.is_change = buffer.read_bool();
	}
	
	this.save_change = function() {
		this.is_change = true;
	}
}