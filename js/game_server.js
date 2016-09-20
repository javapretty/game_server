/*
*	描述：game_server服务器脚本
*	作者：张亚磊
*	时间：2016/02/24
*/

require('enum.js');
require('error.js');
require('message.js');
require('struct.js');
require('config.js');
require('util.js');
require('timer.js');
require('game_player.js');
require('bag.js');
require('mail.js');
require('npc.js');

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
var npc_map = new Map();
//加载配置文件
var config = new Config();
config.init();

//test用npc
var npc = new NPC(10);
npc_map.set(npc.id, npc);

//定时器管理器
var timer = new Timer();
timer.init(Server_Type.GAME_SERVER);

//执行脚本主循环函数
main();

function main() {
	while (true) {
		var all_empty = true;
		
		//获得gate客户端消息object
		var obj = pop_game_gate_msg_object();
		if (obj != null) {
			all_empty = false;
			process_game_gate_msg(obj);
		}
		
		//获得db服务器消息object
		var obj = pop_game_db_msg_object();
		if (obj != null) {
			all_empty = false;
			process_game_db_msg(obj);
		}
		
		//获得master服务器消息object
		var obj = pop_game_master_msg_object();
		if (obj != null) {
			all_empty = false;
			process_game_master_msg(obj);
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

function process_game_gate_msg(obj) {
	if (obj.msg_id == Msg.REQ_FETCH_ROLE_INFO) {
		fetch_role_info(obj);
		return;
	} else if (obj.msg_id == Msg.REQ_CREATE_ROLE) { 
		create_role(obj);
		return;
	}
	
	var cid = get_cid(obj.cid, obj.player_cid);
	var game_player = game_player_cid_map.get(cid);
	if (!game_player) {
		print('process_game_gate_msg, game_player not exist, gate_cid:', obj.cid, ' player_cid:', obj.player_cid, ' msg_id:', obj.msg_id);
		return;
	}
	
	switch(obj.msg_id) {
	case Msg.SYNC_GATE_GAME_PLAYER_LOGOUT:
		game_close_client(game_player.gate_cid, game_player.player_cid, 0);
		break;	
	case Msg.REQ_FETCH_BAG_INFO:
		game_player.bag.fetch_bag_info();
		break;
	case Msg.REQ_USE_ITEM:
		game_player.bag.use_item(obj);
		break;
	case Msg.REQ_SELL_ITEM:
		game_player.bag.sell_item(obj);
		break
	case Msg.REQ_FETCH_MAIL_INFO:
		game_player.mail.fetch_mail_info();
		break;
	case Msg.REQ_PICKUP_MAIL:
		game_player.mail.pickup_mail(obj);
		break;
	case Msg.REQ_DEL_MAIL:
		game_player.mail.delete_mail(obj);
		break;
	case Msg.REQ_SEND_MAIL:
		game_player.mail.send_mail(obj);
		break;
	case Msg.REQ_MOVE_TO_POINT:
		game_player.move_to_point(obj);
		break;
	case Msg.REQ_CHANGE_SCENE:
		game_player.change_scene(obj);
		break;
	default:
		print('process_game_gate_msg, msg_id: not exist', obj.msg_id);
		break;
	}
}

function process_game_db_msg(obj) {	
	switch(obj.msg_id){
	case Msg.SYNC_DB_GAME_LOAD_PLAYER:
	case Msg.SYNC_DB_GAME_CREATE_PLAYER: {
		process_loaded_player_data(obj);
		break;
	}
	case Msg.SYNC_DB_GAME_SAVE_PLAYER: {
		logout_map.delete(obj.account);
		break;
	}
	default:
		print('process_game_db_msg, msg_id: not exist', obj.msg_id);
		break;
	}
}

function process_game_master_msg(obj) {
	var game_player = game_player_role_id_map.get(obj.role_id);
	if (!game_player) {
		print('process_game_master_msg, game_player not exist, role_id:', obj.role_id, ' msg_id:', obj.msg_id);
		return;
	}
	
	switch(obj.msg_id){
	case Msg.SYNC_MASTER_GAME_GUILD_INFO: {
		game_player.set_guild_info(obj);
		break;
	}
	default:
		print('process_game_master_msg, msg_id: not exist', obj.msg_id);
		break;
	}
}

function fetch_role_info(obj) {	
	//帐号还在登录/登出流程中判断
	if (login_map.get(obj.account) || logout_map.get(obj.account) ) {
		print('account in logining status, account:', obj.account);
		game_close_client(obj.cid, obj.player_cid, Error_Code.ERROR_DISCONNECT_RELOGIN);
		return;
	}

	//重复登录判断
	var player = game_player_role_id_map.get(obj.role_id);
	if (! player) {
		//登录加载玩家信息，玩家存在，直接从数据库取数据
		print('fetch_role_info, load player info from db, account:', obj.account, ' gate_cid:', obj.cid, ' player_cid:', obj.player_cid);
		var cid_info = new Cid_Info();
		cid_info.gate_cid = obj.cid;
		cid_info.player_cid = obj.player_cid;
		login_map.set(obj.account, cid_info);

		var msg = new MSG_150001();
		msg.account = obj.account;
		send_game_msg_to_db(Msg.SYNC_GAME_DB_LOAD_PLAYER, msg);
	} else {
		//玩家重复登录，获取信息
		print('player login again, account:', obj.account);
	}
}

function create_role(obj) {
	if (obj.account.length <= 0 || obj.role_name.length <= 0) {
		print('create_role parameter invalid, account:', obj.account, ' role_name:', obj.role_name);
		return;
	}
	
	if (login_map.get(obj.account)) {
		print('account in logining status, account:', obj.account, ' role_name:', obj.role_name);
		game_close_client(obj.cid, obj.player_cid, Error_Code.ERROR_DISCONNECT_RELOGIN);
		return;	
	}

	var cid_info = new Cid_Info();
	cid_info.gate_cid = obj.cid;
	cid_info.player_cid = obj.player_cid;
	login_map.set(obj.account, cid_info);
	
	var msg = new MSG_150002();
	msg.role_info.account = obj.account;
	msg.role_info.role_name = obj.role_name;
	msg.role_info.client_ip = obj.client_ip;
	msg.role_info.gender = obj.gender;
	msg.role_info.career = obj.career;
	send_game_msg_to_db(Msg.SYNC_GAME_DB_CREATE_PLAYER, msg);
}

function process_loaded_player_data(obj) {
	print('process_loaded_player_data, status:', obj.status, ' account:', obj.account);
	
	var cid_info = login_map.get(obj.account);
	if (cid_info == null) {
		print('player is not in login map, account:', obj.account);
		return;	
	}

	var gate_cid = cid_info.gate_cid;
	var player_cid = cid_info.player_cid;
	if (gate_cid < 2 || player_cid < 2) {
		print('player cid error, account:', obj.account, ' gate_cid:', gate_cid, ' player_cid:', player_cid);
		return;
	}
	login_map.delete(obj.account);

	switch(obj.status) {
	//[角色登录]没有玩家数据, 提示创建新角色
	case Role_Status.ROLE_NOT_EXIST:	{
		send_game_msg_to_gate(gate_cid, player_cid, Msg.RES_FETCH_ROLE_INFO, Error_Code.ERROR_ROLE_NOT_EXIST);
		break;
	}
	//[创建角色]创建的角色名已存在
	case Role_Status.ROLE_HAS_EXIST: {
		send_game_msg_to_gate(gate_cid, player_cid, Msg.RES_CREATE_ROLE, Error_Code.ERROR_ROLE_NAME_EXIST);
		break;
	}
	//[创建角色]创建角色成功
	case Role_Status.ROLE_SUCCESS_CREATE:
	//[角色登录]数据加载成功
	case Role_Status.ROLE_SUCCESS_LOAD: {
		var game_player = new Game_Player();
		game_player.load_player_data(gate_cid, player_cid, obj);
		break;
	}
	default: {
		print("role status:%d error", obj.status);
		break;
	}
	}
}
