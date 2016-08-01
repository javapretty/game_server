/*
*	描述：序列容器list类
*	作者：张亚磊
*	时间：2016/08/01
*/

function List() {  
    this.value = [];     
}  

//添加对象
List.prototype.add = function(obj) {  
	return this.value.push(obj);  
};  
  
//获得对象
List.prototype.get = function(index) {  
	return this.value[index];  
};

//获取list长度
List.prototype.size = function() {  
	return this.value.length;  
};  
   
//删除指定索引的对象
List.prototype.remove = function(index) {  
	this.value.splice(index,1);  
	return this.value;  
};  
  
//删除全部值  
List.prototype.removeAll = function() {  
	return this.value = [];  
};  
  
//是否包含某个对象  
List.prototype.constains = function(obj) {  
	for ( var i in this.value) {  
		if (obj == this.value[i]) {  
			return true;  
		} else {  
			continue;  
		}  
	}  
	return false;  
};  
      
//获取所有对象  
List.prototype.getAll = function() {  
	var allInfos = '';  
	for ( var i in this.value) {  
		if(i != (value.length-1)){  
			allInfos += this.value[i]+",";  
		} else {  
			allInfos += this.value[i];  
		}  
	}  
	return allInfos += this.value[i]+",";;  
};  