/*
*	描述：玩家系统
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

function Player() {
	this.cid = 0;
	this.cplayer = null;
	this.player_info = new Player_Info();
	this.hero = new Hero();
	this.bag = new Bag();
	this.mail = new Mail();

	//玩家上线，加载数据
	this.load_player_data = function(buffer) {
		if (buffer == null) {
			return;
		}
	
		var gate_cid = buffer.read_int32();
		var player_cid = buffer.read_int32();
		var status = buffer.read_int8();
		var size = buffer.read_uint16();
		this.player_info.deserialize(buffer);
		this.hero.load_data(this, buffer);
		this.bag.load_data(this, buffer);
		this.mail.load_data(this, buffer);
		
		print('------load_data,status:', status, ' role_id:', this.player_info.role_id, ' role_name:', this.player_info.role_name);
		this.cid = gate_cid * 10000 + player_cid;
		this.cplayer = get_player_by_cid(gate_cid, player_cid);
		
		player_cid_map.put(this.cid, this);
		player_role_id_map.put(this.player_info.role_id, this);
	}
	
	//玩家离线，保存数据
	this.save_player_data = function() {
		var buffer = this.cplayer.player_data_buffer();
		if (buffer == null) {
			return;
		}
		
		buffer.write_int8(0);				//status
		buffer.write_uint16(0);			//change_set size
		this.player_info.serialize(buffer);
		this.hero.save_data(buffer);
		this.bag.save_data(buffer);
		this.mail.save_data(buffer);
		print('------save_data,role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name);
		
		player_cid_map.remove(this.cid);
		player_role_id_map.remove(this.player_info.role_id);
	}
	
	this.set_data_change = function() {
		this.cplayer.set_player_data_change(DATA_CHANGE.PLAYER_CHANGE);
	}
	
	this.getMaxVitality = function() {
        var levelArray = new Array();
		levelArray.push(this.player_info.level.toString());
        var maxVitality = Math.floor(util.lookupDataTable("config/vitality/playerLevel.json", "Max Vitality", levelArray));
        return maxVitality;
    }
	
	this.buy_vitality = function() {
		print('buy_vitality, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " msec:", msec());
		//1.检查元宝是否充足
		var todyBuyArry = new Array();
		todyBuyArry.push((this.player_info.today_buy + 1).toString());
		var costGold = util.lookupDataTable("config/vitality/gradientPrice.json", "Vitality", todyBuyArry);
		var curGold = this.bag.bag_info.gold;
		if (curGold < costGold){
			return this.cplayer.respond_error_result(MSG_RES.RES_BUY_VITALITY_INFO, ERROR_CODE.ERROR_GOLD_NOT_ENOUGH);
		}
		
		//2.检查可以购买体力次数
		var vipArry = new Array();
		vipArry.push(this.player_info.vip.toString());
		var canBuyTimes = util.lookupDataTable("config/vip/vip.json", "Buy Vit Max", vipArry);
		if (this.player_info.today_buy >= canBuyTimes){
			return this.cplayer.respond_error_result(MSG_RES.RES_BUY_VITALITY_INFO, ERROR_CODE.ERROR_VITALITY_TIMES_NOT_ENOUGH);
		}

		//3.更新元宝
		var result = this.bag.bag_sub_money(0, costGold);
		
		//4.更新购买次数以及体力修改时间
		this.player_info.last_change_time = util.getTimestamp();
		this.player_info.today_buy = this.player_info.today_buy + 1;
		
		//5.更新体力(120应该为配置)
		var maxVit = this.getMaxVitality();
		this.player_info.vitality = Math.min(Math.max(0, (this.player_info.vitality + 120)), maxVit);
		//6.返回消息给客户端
		var buf = pop_buffer();
		buf.write_int32(this.player_info.vitality);
		this.cplayer.respond_success_result(MSG_RES.RES_BUY_VITALITY_INFO, buf);
		push_buffer(buf);
		this.set_data_change();
	}
	
	this.update_vip = function(transactionType){
		print('update_vip, role_id:', this.player_info.role_id, " role_name:", this.player_info.role_name, " msec:", msec());
		
		var vipArry = new Array();
		vipArry.push(transactionType);
		var newVipExp = this.player_info.vip_exp + util.lookupDataTable("config/vip/recharge.json", "VIP Exp", vipArry);
		
		var vipModule = util.lookupDataTable("config/vip/vip.json", null, null);
		var vipMaxLevle = util.lookupDataTable("config/parameter/parameterTable.josn", "max_vip_level", null);
		for (var i = 0; i < vipMaxLevle; i++)
		{
			if (newVipExp < vipModule[i.toString()]["Recharge"]){
				continue;
			}
			
			this.player_info.vip = i;
		}
		
		print('after update vip:', this.player_info.vip);
	}
}