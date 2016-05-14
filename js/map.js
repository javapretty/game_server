/*
*	描述：关联容器map类
*	作者：张亚磊
*	时间：2016/02/24
*/

Array.prototype.remove = function(s) {
	for (var i = 0; i < this.length; i++) {
		if (s == this[i])
			this.splice(i, 1);
	}
}

function Map() {
    //存放键
	this.keys = new Array();
    //存放数据
	this.data = new Object();
}

//插入元素
Map.prototype.insert = function(key, value) {
	if (this.data[key] == null) {
		this.keys.push(key);
	}
	this.data[key] = value;
};

//获得元素
Map.prototype.get = function(key) {
	return this.data[key];
};

//删除元素
Map.prototype.remove = function(key) {
	this.keys.remove(key);
	this.data[key] = null;
};

//删除第一个元素
Map.prototype.pop_front = function() {
	var len = this.keys.length;
	if (len <= 0) {
		return;
	}
	var key = this.keys[0];
	this.keys.remove(key);
	this.data[key] = null;
};

//清空map
Map.prototype.clear = function() {
	this.keys = new Array();
	this.data = new Object();
};

//判断Map是否为空
Map.prototype.empty = function() {
	return this.keys.length == 0;
};
    
//获得map元素个数
Map.prototype.size = function(){
	return this.keys.length;
};
    
//遍历Map,执行处理函数fn
Map.prototype.each = function(fn) {
	if(typeof fn != 'function') {
		return;
	}
	var len = this.keys.length;
	for(var i=0;i<len;i++) {
		var k = this.keys[i];
		fn(k,this.data[k],i);
	}
};
