/*
*	描述：配置文件管理类
*	作者：张亚磊
*	时间：2016/03/24
*/

function Config() {
	this.item_json = null;			//道具配置
	this.level_json = null;			//玩家等级相关配置
	this.util_json = null;			//公共综合配置
	this.vip_json = null;				//vip配置
	this.recharge_json = null;		//充值相关配置
	this.ai_json = null;		//ai相关配置
	
	this.init = function() {
		try {
			var item_str = read_json("config/bag/item.json");
			this.item_json = JSON.parse(item_str);
			
			var level_str = read_json("config/player/level.json");
			this.level_json = JSON.parse(level_str);
			
			var util_str = read_json("config/player/util.json");
			this.util_json = JSON.parse(util_str);
			
			var vip_str = read_json("config/vip/vip.json");
			this.vip_json = JSON.parse(vip_str);
			
			var recharge_str = read_json("config/vip/recharge.json");
			this.recharge_json = JSON.parse(recharge_str);
			
			var ai_str = read_json("config/monster/ai_config.json");
			this.ai_json = JSON.parse(ai_str);
		} catch (err) {
			print(err.message);
		}
	}
}
