/*
*	描述：master_server服务器脚本
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
require('master_player.js');
require('guild.js');
require('offline.js');
require('rank.js');

//cid----master_player  cid = gate_cid * 10000 + player_cid
var master_player_gate_cid_map = new Map();
//cid----master_player  cid = game_cid * 10000 + player_cid
var master_player_game_cid_map = new Map();
//role_id---master_player
var master_player_role_id_map = new Map();
//role_name---master_player
var master_player_role_name_map = new Map();

//加载配置文件
var config = new Config();
config.init();

//公会
var guild_manager = new Guild();
//离线管理器
var offline_manager = new Offline();
//排行榜
var rank_manager = new Rank();

//定时器管理器
var timer = new Timer();
timer.init(Server_Type.MASTER_SERVER);

//执行脚本主循环函数
main();

function main() {
	while (true) {
		var all_empty = true;
		
		//获得game服务器消息
		var buffer = pop_master_game_buffer();
		if(buffer != null) {
			all_empty = false;
			process_master_game_buffer(buffer);
		}
		
		//获得gate客户端消息
		var buffer = pop_master_gate_buffer();
		if (buffer != null) {
			all_empty = false;
			process_master_gate_buffer(buffer);
		}
		
		//获得db服务器消息
		var buffer = pop_master_db_buffer();
		if(buffer != null) {
			all_empty = false;
			process_master_db_buffer(buffer);
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

function process_master_gate_buffer(buffer) {
	var gate_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	var player_cid = buffer.read_int32();
	
	//gate通知master玩家登录，加载信息
	if (msg_id == Msg_Gate.SYNC_GATE_MASTER_PLAYER_LOGIN) {
		var role_id = buffer.read_int64();
		var master_player = master_player_role_id_map.get(role_id);
		if (master_player == null) {
			master_player = new Master_Player();
		}
		master_player.set_gate_cid(gate_cid, player_cid, role_id);
		return;
	}
	
	var cid = gate_cid * 10000 + player_cid;
	var master_player = master_player_gate_cid_map.get(cid);
	if (!master_player) {
		print('master_player not exist, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);
		return push_master_gate_buffer(gate_cid, buffer);
	}
	
	switch(msg_id) {
	case Msg_Gate.SYNC_GATE_MASTER_PLAYER_LOGOUT:
		master_player.cplayer.link_close();
		break;
	case Msg_CM.REQ_SEND_CHAT_INFO:
		master_player.send_chat_info(buffer);
		break;
	case Msg_CM.REQ_CREATE_GUILD:
		guild_manager.create_guild(master_player, buffer);
		break;
	case Msg_CM.REQ_DISSOVE_GUILD:
		guild_manager.dissove_guild(master_player, buffer);
		break;
	case Msg_CM.REQ_JOIN_GUILD:
		guild_manager.join_guild(master_player, buffer);
		break;
	case Msg_CM.REQ_FETCH_RANK:
		rank_manager.fetch_rank_info(master_player, buffer);
		break;
	case Msg_CM.REQ_GUILD_ALLOW_JOIN:
		guild_manager.allow_join_player(master_player, buffer);
		break;
	case Msg_CM.REQ_GUILD_KICK_OUT:
		guild_manager.kick_out_player(master_player, buffer);
		break;
	default:
		print('process_master_gate_buffer msg_id not exist, gate_cid:', gate_cid, " player_cid:", player_cid, " msg_id:", msg_id);
		break;
	}
	push_master_gate_buffer(gate_cid, buffer);
}

function process_master_db_buffer(buffer) {
	var db_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status = buffer.read_int32();
	
	switch(msg_id) {
	case Msg_MD.SYNC_DB_MASTER_CREATE_GUILD:
		guild_manager.create_guild_res(buffer);
		break;
	case Msg_MD.SYNC_DB_MASTER_LOAD_GUILD:
		guild_manager.load_data(buffer);
		break;
	case Msg_MD.SYNC_DB_MASTER_LOAD_OFFLINE:
		offline_manager.load_data(buffer);
		break;
	case Msg_MD.SYNC_DB_MASTER_LOAD_RANK:
		rank_manager.load_data(buffer);
		break;
	default:
		print('process_master_db_buffer msg_id not exist:', msg_id);
		break;
	}
	push_master_db_buffer(db_cid, buffer);
}

function process_master_game_buffer(buffer) {
	var game_cid = buffer.read_int32();
	var len = buffer.read_int16();
	var msg_id = buffer.read_int32();
	var status =  buffer.read_int32();
	var player_cid = buffer.read_int32();
	
	if (msg_id == Msg_GM.SYNC_GAME_MASTER_PLYAER_LOGIN) {
		var msg = new MSG_160000();
		msg.deserialize(buffer);
		var master_player = master_player_role_id_map.get(msg.player_info.role_id);
		if (master_player == null) {
			master_player = new Master_Player();
		}
		master_player.load_player_data(game_cid, player_cid, msg.player_info);
	}
}