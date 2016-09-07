/*
 * Master_Connector.h
 *
 *  Created on: Jan 13, 2016
 *      Author: zhangyalei
 */

#ifndef MASTER_CONNECTOR_H_
#define MASTER_CONNECTOR_H_

#include "Connector.h"

class Master_DB_Connector: public Connector {
public:
	static Master_DB_Connector *instance(void);

	virtual void process_list(void);

private:
	Master_DB_Connector(void);
	virtual ~Master_DB_Connector(void);
	Master_DB_Connector(const Master_DB_Connector &);
	const Master_DB_Connector &operator=(const Master_DB_Connector &);

private:
	static Master_DB_Connector *instance_;
};

#define MASTER_DB_CONNECTOR Master_DB_Connector::instance()

#endif /* MASTER_CONNECTOR_H_ */
