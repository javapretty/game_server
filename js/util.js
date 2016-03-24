/*
*	描述：基础函数类
*	作者：张亚磊
*	时间：2016/03/15
*/

var util = function () {};

//获取当前时间戳(秒单位)
util.now_sec = function() {
	return Math.round(new Date().getTime()/1000);
}

//获取当前时间戳(毫秒单位)
util.now_msec = function(date) {
	return new Date().getTime(); 
}
