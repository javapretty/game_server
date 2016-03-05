/*
*	description: js util class
*   Date: 16-3-3
*/

var util = function () {};

//获取json配置项
util.lookupDataTable = function(file, destElement, ElementArr) {
	var content = read_json(file);
	try {
		var tableData = JSON.parse(content);
		if (tableData == null) {
			return null;
		}
		
		if (ElementArr instanceof Array) {			
			var ArrayLen = ElementArr.length;
			for(var i = 0; i < ArrayLen; i++) {
				var element = ElementArr[i];
				if (tableData[element] != null) {
					tableData = tableData[element];
				} else {
					return null;
				}
			}
		}
		
		if (destElement != null) {
			if (tableData[destElement] != null) {
				return tableData[destElement];
			} else {
				return null;
			}
		}
		return tableData;	
			
	} catch (err) {
		print(err.message);
		return null;
	}
}

//获取当前时间戳(毫秒单位)
util.getTimestamp  = function(date){
	if (arguments.length){
		return date.getTime();
	}
	else{
		var nowTime = new Date();
		return nowTime.getTime();
	}
}

//time格式int
util.isSameDayByStamp  = function(timeA, timeB){	
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
util.isTimeNeedReset = function(lastResetTimestamp){
    var h = 5; //每日重置 Hour
    var m = 0; //每日重置 Min
    var s = 0; //每日重置 Sec
    var ts = (h * 3600 * 1000 + m * 60 * 1000 + s);
    return !util.isSameDayByStamp(time() - $ts, $lastResetTimestamp - $ts);
}