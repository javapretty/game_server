/*
*	描述：master_server服务器脚本
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
require('master_player.js');
require('guild.js');
require('rank.js');
require('offline.js');

//cid----master_player  cid = get_cid(gate_cid, player_cid)
var master_player_gate_cid_map = new Map();
//cid----master_player  cid = get_cid(game_cid, player_cid)
var master_player_game_cid_map = new Map();
//role_id---master_player
var master_player_role_id_map = new Map();
//role_name---master_player
var master_player_role_name_map = new Map();
//scene_id--game_id
var scene_game_map = new Map();

//加载配置文件
var config = new Config();
config.init();

//公会管理器
var guild_manager = new Guild();
//排行榜管理器
var rank_manager = new Rank();
//离线管理器
var offline_manager = new Offline();

//定时器管理器
var timer = new Timer();
timer.init(Server_Type.MASTER_SERVER);

//执行脚本主循环函数
main();

function main() {
	while (true) {
		var all_empty = true;
		
		//获得gate客户端消息object
		var obj = pop_master_gate_msg_object();
		if (obj != null) {
			all_empty = false;
			process_master_gate_msg(obj);
		}
		
		//获得db服务器消息object
		var obj = pop_master_db_msg_object();
		if(obj != null) {
			all_empty = false;
			process_master_db_msg(obj);
		}
	
		//获得game服务器消息object
		var obj = pop_master_game_msg_object();
		if(obj != null) {
			all_empty = false;
			process_master_game_msg(obj);
		}
		
		//获得http服务器消息object
		var obj = pop_master_http_msg_object();
		if(obj != null) {
			all_empty = false;
			process_master_http_msg(obj);
		}
		
		//获得下线玩家的cid
		var cid = get_drop_master_player_cid();
		if (cid > 0) {
			all_empty = false;
			var master_player = master_player_gate_cid_map.get(cid);
			if (master_player) {
				master_player.save_player_data();
			}
		}

		//处理定时器消息
		while(true) {
			var timer_id = get_master_timer_id();
			if (timer_id == 0)
				break;
	
			all_empty = false;
			var timer_handler = timer.get_master_timer_handler(timer_id);
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

function process_master_gate_msg(obj) {
	//gate通知master玩家登录，加载信息
	if (obj.msg_id == Msg.SYNC_GATE_MASTER_PLAYER_LOGIN) {
		var master_player = master_player_role_id_map.get(obj.role_id);
		if (master_player == null) {
			master_player = new Master_Player();
		}
		master_player.set_gate_cid(obj.cid, obj.player_cid, obj.role_id);
		return;
	}
	
	var cid = get_cid(obj.cid, obj.player_cid);
	var master_player = master_player_gate_cid_map.get(cid);
	if (!master_player) {
		print('master_player not exist, gate_cid:', obj.cid, ' player_cid:', obj.player_cid, ' msg_id:', obj.msg_id);
		return;
	}
	
	switch(obj.msg_id) {
	case Msg.SYNC_GATE_MASTER_PLAYER_LOGOUT:
		master_close_client(master_player.gate_cid, master_player.player_cid, 0);
		break;
	case Msg.REQ_SEND_CHAT_INFO:
		master_player.send_chat_info(obj);
		break;
	case Msg.REQ_CREATE_GUILD:
		guild_manager.create_guild(master_player, obj);
		break;
	case Msg.REQ_DISSOVE_GUILD:
		guild_manager.dissove_guild(master_player, obj);
		break;
	case Msg.REQ_JOIN_GUILD:
		guild_manager.join_guild(master_player, obj);
		break;
	case Msg.REQ_FETCH_RANK:
		rank_manager.fetch_rank_info(master_player, obj);
		break;
	case Msg.REQ_GUILD_ALLOW_JOIN:
		guild_manager.allow_join_player(master_player, obj);
		break;
	case Msg.REQ_GUILD_KICK_OUT:
		guild_manager.kick_out_player(master_player, obj);
		break;
	default:
		print('process_master_gate_msg msg_id not exist, gate_cid:', obj.cid, ' player_cid:', obj.player_cid, ' msg_id:', obj.msg_id);
		break;
	}
}

function process_master_db_msg(obj) {
	switch(obj.msg_id) {
	case Msg.SYNC_DB_MASTER_CREATE_GUILD:
		guild_manager.create_guild_res(obj);
		break;
	case Msg.SYNC_DB_MASTER_LOAD_GUILD:
		guild_manager.load_data(obj);
		break;
	case Msg.SYNC_DB_MASTER_LOAD_RANK:
		rank_manager.load_data(obj);
		break;
	case Msg.SYNC_DB_MASTER_LOAD_OFFLINE:
		offline_manager.load_data(obj);
		break;
	default:
		print('process_master_db_msg, msg_id: not exist', obj.msg_id);
		break;
	}
}

function process_master_game_msg(obj) {
	switch(obj.msg_id) {
	case Msg.SYNC_GAME_MASTER_PLYAER_LOGIN: {
		var master_player = master_player_role_id_map.get(obj.player_info.role_id);
		if (master_player == null) {
			master_player = new Master_Player();
		}
		master_player.load_player_data(obj.cid, obj.player_cid, obj.player_info);
		break;
	}
	case Msg.SYNC_GAME_MASTER_SCENE_ID: {
		scene_game_map.set(obj.scene_id, obj.game_id);
		break;
	}
	case Msg.SYNC_GAME_MASTER_PLAYER_CHANGE_SCENE: {
		var master_player = master_player_role_id_map.get(obj.role_id);
		if(master_player == null) {
			print("change scene error! player not exists!");
			return;
		}
		var msg = new MSG_140202();
		msg.game_server_id = scene_game_map.get(obj.target_scene);
		master_player.send_succuss_msg(Msg.SYNC_MASTER_GATE_PLAYER_CHANGE_SCENE, msg);
		break;
	}
	default:
		print('process_master_game_msg, msg_id: not exist', obj.msg_id);
		break;
	}
}

function process_master_http_msg(obj) {
	switch(obj.msg_id) {
	case Msg.HTTP_MODIFY_PLAYR_GOLD: {
		print('process_master_http_msg, msg_id:', obj.msg_id, " role_name:", obj.role_name, " gold:", obj.gold);
		var msg = new MSG_180000();
		msg.role_name = obj.role_name;
		msg.gold = obj.gold;
		send_master_msg_to_http(obj.cid, obj.msg_id, obj);
		break;
	}
	default:
		print('process_master_game_msg, msg_id: not exist', obj.msg_id);
		break;
	}
}