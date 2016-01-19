/*
 * Log_Connector.h
 *
 *  Created on: Dec30,2015
 *      Author: zhangyalei
 */

#ifndef LOG_CONNECTOR_H_
#define LOG_CONNECTOR_H_

#include "Connector.h"

class Log_Connector: public Connector {
public:
	typedef Object_Pool<Block_Buffer, Thread_Mutex> Block_Pool;

	static Log_Connector *instance(void);

	int push_data_block(Block_Buffer &buf, bool use_swap = true);

	virtual void process_list(void);

private:
	Log_Connector(void);
	virtual ~Log_Connector(void);
	Log_Connector(const Log_Connector &);
	const Log_Connector &operator=(const Log_Connector &);

private:
	static Log_Connector *instance_;
	Block_Pool block_pool_;
};

#define LOG_CONNECTOR Log_Connector::instance()

#endif /* LOG_CONNECTOR_H_ */
