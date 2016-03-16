/*
*	描述：定时器系统
*	作者：李俊良
*	时间：2016/03/15
*	qq:975338436
*	tel:15618920470
*/

function Timers() {
	this.timers = new Map(); //储存参与定时的对象
	this.timer_id = 1; //全局定时器编号
	
	this.check_timer = function(timer_id){
		if(this.timers.get(timer_id) == null){
			return;
		}
		if(this.timers.get(timer_id).timing == 0){
			this.timers.remove(timer_id);
			this.deregister(timer_id);
			return;
		}
		this.timers.get(timer_id).handleTimeOut();
	}

	this.register = function(handler, interval, nexttime = 0){
		register_timer_in(this.timer_id, interval, nexttime, 1);
		this.timers.put(this.timer_id, handler);
		this.timer_id++;
	}

	this.deregister = function(timer_id){
		register_timer_in(timer_id, 0, 0, 0);
	}
}
