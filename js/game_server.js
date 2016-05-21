/*
*	描述：game_server服务器脚本
*	作者：张亚磊
*	时间：2016/02/24
*/

require('map.js');
require('enum.js');
require('error.js');
require('message.js');
require('struct.js');
require('msg_struct.js');
require('config.js');
require('util.js');
require('timer.js');
require('game_player.js');
require('hero.js');
require('bag.js');
require('mail.js');
require('shop.js');

//cid----game_player  全局玩家对象
var game_player_cid_map = new Map();
//role_id---game_player 全局玩家对象
var game_player_role_id_map = new Map();

//加载配置文件
var config = new Config();
config.init();

//定时器管理器
var timer = new Timer();
timer.init(Server_Type.GAME_SERVER);

//执行脚本主循环函数
main();

function main() {
	while (true) {
		var all_empty = true;
		
		//获得客户端消息
		var buffer = pop_game_client_buffer();
		if (buffer != null) {
			all_empty = false;
			process_game_client_buffer(buffer);
		}
	
		//获得上线玩家的信息
		buffer = get_game_player_load_data_buffer();
		if (buffer != null) {
			all_empty = false;
			var game_player = new Game_Player();
			game_player.load_player_data(buffer);
		}
	
		//获得下线玩家的cid
		var cid = get_drop_game_player_cid();
		if (cid > 0) {
			all_empty = false;
			var game_player = game_player_cid_map.get(cid);
			if (game_player) {
				game_player.save_player_data();
			}
		}
		
		//获得master过来的同步消息
		var buffer = pop_sync_master_data_buffer();
		if(buffer != null){
			all_empty = false;
			process_game_master_buffer(buffer);
		}

		//处理定时器消息
		while(true) {
			var timer_id = get_game_timer_id();
			if (timer_id == 0)
				break;
	
			all_empty = false;
			var timer_handler = timer.get_game_timer_handler(timer_id);
			if (timer_handler != null) {
				timer_handler();
			}
		}
		
		if (all_empty) {
			sleep();
			continue;
		}
	}	
}

function process_game_client_buffer(buffer) {
	var gate_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	var player_cid = buffer.read_int32();
	
	var cid = gate_cid * 10000 + player_cid;
	var game_player = game_player_cid_map.get(cid);
	if (!game_player) {
		print('game_player not exist, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);
		return push_game_client_buffer(gate_cid, buffer);
	}
	
	switch(msg_id) {
	case Msg_CG.REQ_BUY_VITALITY:
		game_player.buy_vitality();
		break;	
	case Msg_CG.REQ_EXCHANGE_MONEY:
		game_player.exchange_money(buffer);
		break;
	case Msg_CG.REQ_FETCH_BAG_INFO:
		game_player.bag.fetch_bag_info();
		break;
	case Msg_CG.REQ_USE_ITEM:
		game_player.bag.use_item(buffer);
		break;
	case Msg_CG.REQ_SELL_ITEM:
		game_player.bag.sell_item(buffer);
		break
	case Msg_CG.REQ_FETCH_MAIL_INFO:
		game_player.mail.fetch_mail_info();
		break;
	case Msg_CG.REQ_PICKUP_MAIL:
		game_player.mail.pickup_mail(buffer);
		break;
	case Msg_CG.REQ_DEL_MAIL:
		game_player.mail.delete_mail(buffer);
		break;
	case Msg_CG.REQ_SEND_MAIL:
		game_player.mail.send_mail(buffer);
		break;
	case Msg_CG.REQ_FETCH_HERO_INFO:
		game_player.hero.fetch_hero_info(buffer);
		break;
	case Msg_CG.REQ_ADD_HERO_STAR:
		game_player.hero.add_hero_star(buffer);
		break;
	case Msg_CG.REQ_ADD_HERO_QUALITY:
		game_player.hero.add_hero_quality(buffer);
		break;
	case Msg_CG.REQ_ADD_EQUIP_LEVEL:
		game_player.hero.add_equip_level(buffer);
		break;
	case Msg_CG.REQ_EQUIP_ON_OFF:
		game_player.hero.equip_on_off(buffer);
		break;
	case Msg_CG.REQ_ADD_SKILL_LEVEL:
		game_player.hero.add_skill_level(buffer);
		break;
	case Msg_CG.REQ_FETCH_SHOP_INFO:
		game_player.shop.fetch_shop_info(buffer);
		break;
	case Msg_CG.REQ_BUY_PRODUCT:
		game_player.shop.buy_product(buffer);
		break;
	case Msg_CG.REQ_REFRESH_SHOP:
		game_player.shop.refresh_by_player(buffer);
		break;
	default:
		print('msg_id not exist, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);print("can not find the msg_id:", msg_id);
		break;
	}
	push_game_client_buffer(gate_cid, buffer);
}

function process_game_master_buffer(buffer) {
	var cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	var role_id = buffer.read_int64();
	var game_player = game_player_role_id_map.get(role_id);
	if(!game_player){
		print('role_id: ', role_id, ' do not exist!');
		return;
	}
	
	switch(msg_id){
	case Msg_GM.SYNC_MASTER_GAME_SET_GUILD:
		game_player.set_guild_info(buffer);
		break;
	default:
		print('msg_id: ', msg_id, ' from master do not exist!');
		break;
	}
}
