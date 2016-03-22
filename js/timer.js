/*
*	描述：定时器系统
*	作者：李俊良
*	时间：2016/03/15
*	qq:975338436
*	tel:15618920470
*/

function Timers() {
	this.timers = new Map(); 
	this.timer_id = 1;
	
	this.register = function(handler, interval, nexttime = util.now_msec() / 1000){
		register_timer_in(this.timer_id, interval, nexttime);
		this.timers.put(this.timer_id, handler);
		this.timer_id++;
	}

	this.get_func = function(timer_id){
		return this.timers.get(timer_id);
	}

	this.init = function(){
		//this.register(this.test_time_out, 3);
	}

	this.test_time_out = function(){
		print("timer test");
	}
}
