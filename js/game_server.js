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
require('config.js');
require('util.js');
require('timer.js');
require('game_player.js');
require('hero.js');
require('bag.js');
require('mail.js');
require('shop.js');

//cid----game_player
var game_player_cid_map = new Map();
//role_id---game_player
var game_player_role_id_map = new Map();
//role_name---game_player
var game_player_role_name_map = new Map();
//account---Cid_Info
var login_map = new Map();
//account---now_sec
var logout_map = new Map();

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
		
		//获得gate客户端消息
		var buffer = pop_game_gate_buffer();
		if (buffer != null) {
			all_empty = false;
			process_game_gate_buffer(buffer);
		}
		
		//获得db服务器消息
		var buffer = pop_game_db_buffer();
		if (buffer != null) {
			all_empty = false;
			process_game_db_buffer(buffer);
		}
		
		//获得master服务器消息
		var buffer = pop_game_master_buffer();
		if (buffer != null) {
			all_empty = false;
			process_game_master_buffer(buffer);
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

function process_game_gate_buffer(buffer) {
	var gate_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	var player_cid = buffer.read_int32();
	
	if (msg_id == Msg_CG.REQ_FETCH_ROLE_INFO) {
		fetch_role_info(gate_cid, player_cid, buffer);
		return;
	} else if (msg_id == Msg_CG.REQ_CREATE_ROLE) { 
		create_role(gate_cid, player_cid, buffer);
		return;
	}
	
	var cid = gate_cid * 10000 + player_cid;
	var game_player = game_player_cid_map.get(cid);
	if (!game_player) {
		print('process_game_gate_buffer, game_player not exist, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);
		return push_game_gate_buffer(gate_cid, buffer);
	}
	
	switch(msg_id) {
	case Msg_Gate.SYNC_GATE_GAME_PLAYER_LOGOUT:
		game_player.cplayer.link_close();
		break;	
	case Msg_CG.REQ_BUY_VITALITY:
		game_player.buy_vitality();
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
	case Msg_CG.REQ_ADD_HERO_EXP:
		game_player.add_hero_exp_test(buffer);
		break;
	default:
		print('process_game_gate_buffer, msg_id not exist, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);
		break;
	}
	push_game_gate_buffer(gate_cid, buffer);
}

function process_game_db_buffer(buffer) {
	var db_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	
	switch(msg_id){
	case Msg_GD.SYNC_DB_GAME_LOAD_PLAYER:
	case Msg_GD.SYNC_DB_GAME_CREATE_PLAYER: {
		process_loaded_player_data(buffer);
		break;
	}
	case Msg_GD.SYNC_DB_GAME_SAVE_PLAYER: {
		var msg = new MSG_550003();
		msg.deserialize(buffer);
		logout_map.remove(msg.role_id);
		break;
	}
	default:
		print('process_game_gate_buffer msg_id not exist, cid:', cid, " msg_id:", msg_id);
		break;
	}
	push_game_db_buffer(db_cid, buffer);
}

function process_game_master_buffer(buffer) {

}

function fetch_role_info(gate_cid, player_cid, buffer) {
	var msg = new MSG_120001;
	msg.deserialize(buffer);
	
	//帐号还在登录/登出流程中判断
	if (login_map.get(msg.account) || logout_map.get(msg.role_id) ) {
		print('account in logining status, account:', msg.account);
		game_close_client(gate_cid, player_cid, Error_Code.ERROR_DISCONNECT_RELOGIN);
		return;
	}

	//重复登录判断
	var player = game_player_role_id_map.get(msg.role_id);
	if (! player) {
		//登录加载玩家信息，玩家存在，直接从数据库取数据
		print('fetch_role_info, load player info from db, account:', msg.account);
		var cid_info = new Cid_Info();
		cid_info.gate_cid = gate_cid;
		cid_info.player_cid = player_cid;
		login_map.insert(msg.account, cid_info);

		var buf = pop_game_buffer();
		buf.make_inner_message(Msg_GD.SYNC_GAME_DB_LOAD_PLAYER);
		var db_msg = new MSG_150001();
		db_msg.account = msg.account;
		db_msg.client_ip = msg.client_ip;
		db_msg.serialize(buf);
		buf.finish_message();
		send_game_buffer_to_db(buf);
		push_game_buffer(buf);
	} else {
		//玩家重复登录，获取信息
		print('player login again, account:', msg.account);
	}
}

function create_role(gate_cid, player_cid, buffer) {
	var msg = new MSG_120002;
	msg.deserialize(buffer);
	if (msg.role_info.account.length <= 0 || msg.role_info.role_name.length <= 0) {
		print('create_role parameter invalid, account:', msg.role_info.account, " role_name:", msg.role_info.role_name);
		return;
	}
	
	if (login_map.get(msg.role_info.account)) {
		print('account in logining status, account:', msg.role_info.account, " role_name:", msg.role_info.role_name);
		game_close_client(gate_cid, player_cid, Error_Code.ERROR_DISCONNECT_RELOGIN);
		return;	
	}

	var cid_info = new Cid_Info();
	cid_info.gate_cid = gate_cid;
	cid_info.player_cid = player_cid;
	login_map.insert(msg.role_info.account, cid_info);

	var buf = pop_game_buffer();
	buf.make_inner_message(Msg_GD.SYNC_GAME_DB_CREATE_PLAYER);
	var db_msg = new MSG_150002();
	db_msg.role_info = msg.role_info;
	db_msg.serialize(buf);
	buf.finish_message();
	send_game_buffer_to_db(buf);
	push_game_buffer(buf);
}

function process_loaded_player_data(buffer) {
	var account = buffer.read_string();
	var client_ip = buffer.read_string();
	var status = buffer.read_int8();
	
	var cid_info = login_map.get(account);
	if (cid_info == null) {
		print('player is not in login map, account:', account);
		return;	
	}

	var gate_cid = cid_info.gate_cid;
	var player_cid = cid_info.player_cid;
	if (gate_cid < 2 || player_cid < 2) {
		print('player cid error, account:', account, " gate_cid:", gate_cid, " player_cid:", player_cid);
		return;
	}
	login_map.remove(account);

	switch(status) {
	//[角色登录]没有玩家数据, 提示创建新角色
	case Role_Status.ROLE_NOT_EXIST:	{
		var buf = pop_game_buffer();
		buf.make_player_message(Msg_GC.RES_FETCH_ROLE_INFO, Error_Code.ERROR_ROLE_NOT_EXIST, player_cid);
		buf.finish_message();
		send_game_buffer_to_gate(gate_cid, buf);
		push_game_buffer(buf);
		break;
	}
	//[创建角色]创建的角色名已存在
	case Role_Status.ROLE_HAS_EXIST: {
		var buf = pop_game_buffer();
		buf.make_player_message(Msg_GC.RES_CREATE_ROLE, Error_Code.ERROR_ROLE_NAME_EXIST, player_cid);
		buf.finish_message();
		send_game_buffer_to_gate(gate_cid, buf);
		push_game_buffer(buf);
		break;
	}
	//[创建角色]创建角色成功
	case Role_Status.ROLE_SUCCESS_CREATE:
	//[角色登录]数据加载成功
	case Role_Status.ROLE_SUCCESS_LOAD: {
		var game_player = new Game_Player();
		game_player.load_player_data(gate_cid, player_cid, client_ip, buffer);
		break;
	}
	default: {
		print("role status:%d error", status);
		break;
	}
	}
}