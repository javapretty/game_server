
function Aoi_Info() {
	this.name = "";
	this.level = 0;
}

function Position() {
	this.x = 0;
	this.y = 0;
	this.z = 0;
}

function Cid_Info() {
	this.gate_cid = 0;
	this.player_cid = 0;
}

function Create_Role_Info() {
	this.account = "";
	this.role_name = "";
	this.gender = 0;
	this.career = 0;
}

function Create_Guild_Info() {
	this.guild_id = 0;
	this.guild_name = "";
	this.chief_id = 0;
}

function Role_Info() {
	this.role_id = 0;
	this.role_name = "";
	this.account = "";
	this.level = 0;
	this.exp = 0;
	this.gender = 0;
	this.career = 0;
	this.vip_level = 0;
	this.vip_exp = 0;
	this.charge_gold = 0;
}

function Item_Detail() {
	this.item_id = 0;
	this.amount = 0;
	this.bind_type = 0;
}

function Mail_Detail() {
	this.pickup = 0;
	this.mail_id = 0;
	this.send_time = 0;
	this.sender_type = 0;
	this.sender_id = 0;
	this.sender_name = "";
	this.mail_title = "";
	this.mail_content = "";
	this.copper = 0;
	this.gold = 0;
	this.item_info = new Array();
}

function Guild_Member_Detail() {
	this.role_id = 0;
	this.role_name = "";
	this.level = 0;
}

function Rank_Member_Detail() {
	this.role_id = 0;
	this.role_name = "";
	this.value = 0;
}

function Master_Player_Info() {
	this.role_id = 0;
	this.account = "";
	this.role_name = "";
	this.level = 0;
	this.gender = 0;
	this.career = 0;
}

function Player_Data() {
	this.player_info = new Game_Player_Info();
	this.bag_info = new Bag_Info();
	this.mail_info = new Mail_Info();
}

function Game_Player_Info() {
	this.role_id = 0;
	this.role_name = "";
	this.account = "";
	this.client_ip = "";
	this.agent_num = 0;
	this.server_num = 0;
	this.level = 0;
	this.exp = 0;
	this.gender = 0;
	this.career = 0;
	this.create_time = 0;
	this.login_time = 0;
	this.logout_time = 0;
	this.vip_level = 0;
	this.vip_exp = 0;
	this.charge_gold = 0;
	this.guild_id = 0;
	this.guild_name = "";
	this.scene_id = 0;
	this.pos_x = 0;
	this.pos_y = 0;
	this.pos_z = 0;
}

function Bag_Info() {
	this.role_id = 0;
	this.copper = 0;
	this.gold = 0;
	this.item_map = new Map();
}

function Mail_Info() {
	this.role_id = 0;
	this.total_count = 0;
	this.mail_map = new Map();
}

function Guild_Info() {
	this.guild_id = 0;
	this.guild_name = "";
	this.chief_id = 0;
	this.create_time = 0;
	this.member_list = new Array();
	this.app_list = new Array();
}

function Rank_Info() {
	this.rank_type = 0;
	this.min_value = 0;
	this.min_role_id = 0;
	this.member_map = new Map();
}

function Offline_Info() {
	this.role_id = 0;
	this.guild_id = 0;
	this.guild_name = "";
}

function MSG_100001() {
	this.account = "";
	this.password = "";
}

function MSG_500001() {
	this.gate_ip = "";
	this.gate_port = 0;
	this.session = "";
}

function MSG_100101() {
	this.account = "";
	this.session = "";
}

function MSG_500101() {
	this.account = "";
}

function MSG_100102() {
	this.client_time = 0;
}

function MSG_500102() {
	this.client_time = 0;
	this.server_time = 0;
}

function MSG_110001() {
	this.chat_type = 0;
	this.chat_content = "";
	this.role_name = "";
}

function MSG_510001() {
	this.chat_type = 0;
	this.chat_content = "";
	this.role_name = "";
	this.gender = 0;
	this.career = 0;
	this.vip_level = 0;
}

function MSG_110101() {
	this.guild_name = "";
}

function MSG_510101() {
	this.guild_id = 0;
}

function MSG_110102() {
	this.guild_id = 0;
}

function MSG_510102() {
	this.guild_id = 0;
}

function MSG_110103() {
	this.guild_id = 0;
	this.role_id = 0;
}

function MSG_510103() {
}

function MSG_110201() {
	this.type = 0;
}

function MSG_510201() {
	this.rank_type = 0;
	this.self_rank = 0;
	this.rank_list = new Array();
}

function MSG_120001() {
	this.account = "";
}

function MSG_520001() {
	this.role_info = new Role_Info();
}

function MSG_120002() {
	this.account = "";
	this.role_name = "";
	this.gender = 0;
	this.career = 0;
}

function MSG_520002() {
	this.role_id = 0;
}

function MSG_120100() {
}

function MSG_520100() {
	this.item_info = new Array();
}

function MSG_120101() {
	this.item = new Item_Detail();
}

function MSG_520101() {
}

function MSG_120102() {
	this.item = new Item_Detail();
}

function MSG_520102() {
}

function MSG_120200() {
}

function MSG_520200() {
	this.mail_info = new Array();
}

function MSG_120201() {
	this.mail_id = 0;
}

function MSG_520201() {
	this.mail_id_info = new Array();
}

function MSG_120202() {
	this.mail_id = 0;
}

function MSG_520202() {
	this.mail_id_info = new Array();
}

function MSG_120203() {
	this.receiver_name = "";
	this.mail_detail = new Mail_Detail();
}

function MSG_520203() {
}

function MSG_120300() {
	this.pos = new Position();
}

function MSG_520300() {
	this.cid = 0;
	this.role_name = "";
	this.pos = new Position();
}

function MSG_120301() {
	this.target_scene = 0;
	this.pos = new Position();
}

function MSG_520301() {
}

function MSG_140100() {
}

function MSG_140200() {
}

function MSG_140201() {
	this.role_id = 0;
}

function MSG_140202() {
	this.game_server_id = 0;
}

function MSG_150001() {
	this.account = "";
}

function MSG_550001() {
	this.player_data = new Player_Data();
}

function MSG_150002() {
	this.role_info = new Create_Role_Info();
}

function MSG_550002() {
	this.player_data = new Player_Data();
}

function MSG_150003() {
	this.logout = 0;
	this.account = "";
	this.player_data = new Player_Data();
}

function MSG_550003() {
	this.account = "";
}

function MSG_150100() {
	this.guild_info = new Create_Guild_Info();
}

function MSG_550100() {
	this.guild_info = new Create_Guild_Info();
}

function MSG_150101() {
}

function MSG_150102() {
	this.struct_name = "";
	this.index_list = new Array();
}

function MSG_150103() {
	this.guild_list = new Array();
}

function MSG_550103() {
	this.guild_list = new Array();
}

function MSG_150104() {
	this.rank_list = new Array();
}

function MSG_550104() {
	this.rank_list = new Array();
}

function MSG_150105() {
	this.offline_list = new Array();
}

function MSG_550105() {
	this.offline_list = new Array();
}

function MSG_160000() {
	this.scene_id = 0;
	this.game_id = 0;
}

function MSG_160001() {
	this.player_info = new Master_Player_Info();
}

function MSG_160002() {
	this.target_scene = 0;
	this.role_id = 0;
}

function MSG_160100() {
	this.role_id = 0;
	this.guild_id = 0;
	this.guild_name = "";
}

function MSG_170000() {
	this.role_id = 0;
	this.role_name = "";
	this.account = "";
	this.level = 0;
	this.client_ip = "";
	this.login_time = 0;
	this.logout_time = 0;
}

function MSG_180000() {
	this.role_name = "";
	this.gold = 0;
}

function MSG_300001() {
	this.player_level = 0;
	this.player_exp = 0;
}

function MSG_300002() {
	this.vip_level = 0;
	this.vip_exp = 0;
}

function MSG_300003() {
	this.cid = 0;
}

function MSG_300100() {
	this.copper = 0;
	this.gold = 0;
}

function MSG_300101() {
	this.item_info = new Array();
}

function MSG_300200() {
	this.mail_info = new Array();
}
