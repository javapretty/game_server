/*
 * ActiveMessage.h
 *
 *  Created on: 2015年12月28日
 *      Author: zhangyalei
 */

#ifndef MSG_ACTIVE_MESSAGE_H_
#define MSG_ACTIVE_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"

/*
断开连接
错误号：
1013 3小时防沉迷时间到
1039 防沉迷时间到且未满5小时
1016 账号在其它地方登陆
1017 封ip
1018 封号
1091 封角色
1038 服务端主动关闭
1103 非法客户端
1006 操作过于频繁
3403 非法客户端 -- 包序列错
3404 非法客户端 -- 包时间戳错
*/
struct MSG_310000 {
	MSG_310000(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};


#endif /* MSG_ACTIVE_MESSAGE_H_ */
