/*
*	描述：基础函数类
*	作者：张亚磊
*	时间：2016/03/15
*/

var util = function () {};

//一天的秒数
util.whole_day_sec = 60 * 60 * 24;
//一天的毫秒数
util.whole_day_msec = util.whole_day_sec * 1000;

//获取当前时间戳(秒单位)
util.now_sec = function() {
	return Math.round(new Date().getTime()/1000);
}

//获取当前时间戳(毫秒单位)
util.now_msec = function(date) {
	return new Date().getTime(); 
}

//获取当前时间到指定定时时间的间隔(每日)
util.get_next_day_tick = function(hour, min = 0, sec = 0) {
	var now = util.now_sec();
	var date = new Date();
	date.setHours(hour);
	date.setMinutes(min);
	date.setSeconds(sec);
	date.setMilliseconds(0);
	var time = date.getTime() / 1000;
	var next_tick = time - now;
	return next_tick > 0 ? next_tick : (next_tick + util.whole_day_sec);
}

util.is_number = function(value) {
	return typeof(value) == "number";
}
