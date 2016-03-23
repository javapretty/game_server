/*
*	描述：基础函数类
*	作者：张亚磊
*	时间：2016/03/15
*/

var util = function () {};

//获取配置文件对象
util.get_json_config = function(file_path, key) {
	try {
		var json_str = read_json(file_path);
		return JSON.parse(json_str)[key];
	} catch (err) {
		print(err.message);
		return null;
	}
}

//获取当前时间戳(秒单位)
util.now_sec = function() {
	return Math.round(new Date().getTime()/1000);
}

//获取当前时间戳(毫秒单位)
util.now_msec = function(date) {
	return new Date().getTime(); 
}
