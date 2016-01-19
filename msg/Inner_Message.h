/*
 * InnerMessage.h
 *
 *  Created on: 2015年12月23日
 *      Author: zhangyalei
 */

#ifndef MSG_INNER_MESSAGE_H_
#define MSG_INNER_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"

/*
配制文件热更消息
*/
struct MSG_410001 {
	std::string module;/**/

	MSG_410001(void) ;
	void serialize(Block_Buffer& w) const ;
	int deserialize(Block_Buffer& r);
	void reset(void) ;
};

#endif /* MSG_INNER_MESSAGE_H_ */
