/*
 * ActiveMessage.cpp
 *
 *  Created on: 2015年12月28日
 *      Author: zhangyalei
 */

#include "Active_Message.h"


MSG_310000::MSG_310000(void) { reset(); };

void MSG_310000::serialize(Block_Buffer & w) const {}

int MSG_310000::deserialize(Block_Buffer & r) {
	return 0;
}

void MSG_310000::reset(void) {}
