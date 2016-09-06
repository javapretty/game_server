/*
*	描述：NPC类
*	作者：李俊良
*	时间：2016/08/24
*/

function NPC(id) {
	this.id = id;
	this.type = 1000;
	this.hp = 0;
	this.mp = 0;
	this.centity = null;
}

NPC.prototype.create_npc = function() {
	
}

NPC.prototype.co_low_hp_handler = function() {
	if(this.hp < config.ai_json.low_hp)
		return true;
	return false;
}

NPC.prototype.co_low_mp_handler = function() {
	if(this.mp < config.ai_json.low_mp)
		return true;
	return false;
}

NPC.prototype.act_recover_hp_handler = function() {
	//print("npc ", this.id, " recover hp");
}

NPC.prototype.act_recover_mp_handler = function() {
	//print("npc ", this.id, " recover mp");
}

