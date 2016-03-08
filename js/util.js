/*
*	description: js util class
*   Date: 16-3-3
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

//获取当前时间戳(毫秒单位)
util.now_msec = function(date){
	if (arguments.length){
		return date.getTime();
	}
	else{
		var nowTime = new Date();
		return nowTime.getTime();
	}
}

//time格式int
util.is_same_day  = function(timeA, timeB){	
	var dateA = new Date(timeA);
	var dateB = new Date(timeB);
	
	if (dateA.getFullYear() == dateB.getFullYear() && dateA.getMonth() == dateB.getMonth() && dateA.getDate() == dateB.getDate() ){
		return true;
	}
	
	return false;
}

/**
 * 传入上次重置时间，返回当前是否需要重置
 *
 * @param $lastResetTimestamp 上次刷新时间
 * @return boolean 返回是否需要重置
 *
 */
util.is_need_reset = function(last_reset_time) {
    var h = 6; //每日重置 Hour
    var m = 0; //每日重置 Min
    var s = 0; //每日重置 Sec
    var ts = (h * 3600 * 1000 + m * 60 * 1000 + s);
    return !util.is_same_day(time() - $ts, $last_reset_time - $ts);
}