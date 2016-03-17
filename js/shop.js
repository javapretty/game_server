/*
*	描述：商店系统
*	作者：李俊良
*	时间：2016/03/15
*	qq:975338436
*	tel:15618920470
*/

function Shop() {
	this.seq = 0;

	this.handleTimeOut = function(){ //有定时行为的对象的接口函数
		print("shop tick ", this.seq);
		this.seq++;
	}
}
