/*
 * InnerMessage.cpp
 *
 *  Created on: 2015年12月23日
 *      Author: zhangyalei
 */

#include "Inner_Message.h"

MSG_410001::MSG_410001(void) {
	reset();
}

void MSG_410001::serialize(Block_Buffer& w) const {
	w.write_string(module);
}

int MSG_410001::deserialize(Block_Buffer& r) {
	if(r.read_string(module)) {
		return -1;
	}
	return 0;
}

void MSG_410001::reset(void) {
	module.clear();
}
