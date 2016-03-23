/*
*	描述：脚本主循环
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

require('map.js');
require('enum.js');
require('error.js');
require('message.js');
require('struct.js');
require('msg_struct.js');
require('player.js');
require('hero.js');
require('bag.js');
require('mail.js');
require('util.js');
require('timer.js');
require('shop.js');

//cid----player  全局玩家对象
var player_cid_map = new Map();
//role_id---player 全局玩家对象
var player_role_id_map = new Map();

//定时器管理器
var timer = new Timer();
timer.init();

//全局数据对象
var shop_base = JSON.parse(read_json('config/shop/product.json'));

//执行脚本主循环函数
main();

function main() {
	while (true) {
		var all_empty = true;
		
		//获得客户端消息
		var buffer = get_client_buffer();
		if (buffer != null) {
			all_empty = false;
			process_client_buffer(buffer);
		}
	
		//获得上线玩家的信息
		buffer = get_load_player_data();
		if (buffer != null) {
			all_empty = false;
			var player = new Player();
			player.load_player_data(buffer);
		}
	
		//获得下线玩家的cid
		var cid = get_drop_player_cid();
		if (cid > 0) {
			all_empty = false;
			var player = player_cid_map.get(cid);
			if (player) {
				player.save_player_data();
			}
		}
	
		while(true) {
			var timer_id = get_timer_id();
			if (timer_id == 0)
				break;
			
			all_empty = false;
			var timer_handler = timer.get_timer_handler(timer_id);
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

function process_client_buffer(buffer) {
	var gate_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	var player_cid = buffer.read_int32();
	
	if (msg_id == Msg_Req.REQ_FETCH_ROLE_INFO || msg_id == Msg_Req.REQ_CREATE_ROLE || msg_id == Msg_Req.SYNC_GATE_GAME_PLAYER_SIGNOUT) {
		process_login_buffer(buffer, gate_cid, player_cid, msg_id);
	} else {
		var cid = gate_cid * 10000 + player_cid;
		var player = player_cid_map.get(cid);
		if (player) {
			switch(msg_id) {
			case Msg_Req.REQ_BUY_VITALITY:
				player.buy_vitality();
				break;	
			case Msg_Req.REQ_FETCH_BAG_INFO:
				player.bag.fetch_bag_info();
				break;
			case Msg_Req.REQ_USE_ITEM:
				player.bag.use_item(buffer);
				break;
			case Msg_Req.REQ_SELL_ITEM:
				player.bag.sell_item(buffer);
				break
			case Msg_Req.REQ_FETCH_MAIL_INFO:
				player.mail.fetch_mail_info();
				break;
			case Msg_Req.REQ_PICKUP_MAIL:
				player.mail.pickup_mail(buffer);
				break;
			case Msg_Req.REQ_DEL_MAIL:
				player.mail.delete_mail(buffer);
				break;
			case Msg_Req.REQ_SEND_MAIL:
				player.mail.send_mail(buffer);
				break;
			case Msg_Req.REQ_FETCH_HERO_INFO:
				player.hero.fetch_hero_info(buffer);
				break;
			case Msg_Req.REQ_ADD_HERO_STAR:
				player.hero.add_hero_star(buffer);
				break;
			case Msg_Req.REQ_ADD_HERO_QUALITY:
				player.hero.add_hero_quality(buffer);
				break;
			case Msg_Req.REQ_ADD_EQUIP_LEVEL:
				player.hero.add_equip_level(buffer);
				break;
			case Msg_Req.REQ_EQUIP_ON_OFF:
				player.hero.equip_on_off(buffer);
				break;
			case Msg_Req.REQ_ADD_SKILL_LEVEL:
				player.hero.add_skill_level(buffer);
				break;
			case Msg_Req.REQ_FETCH_SHOP_INFO:
				player.shop.fetch_shop_info(buffer);
				break;
			case Msg_Req.REQ_BUY_PRODUCT:
				player.shop.buy_product(buffer);
				break;
			case Msg_Req.REQ_REFRESH_SHOP:
				player.shop.refresh_by_player(buffer);
			default:
				print("Can not find the msg_id:", msg_id);
				break;
			}
		} else {
			print('find game player wrong, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);
		}
	}
	push_client_buffer(gate_cid, buffer);
}
