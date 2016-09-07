/*
 * Server_Config.h
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#ifndef SERVER_CONFIG_H_
#define SERVER_CONFIG_H_

#include "Config.h"

class Server_Config : public Config {
public:
	struct Server_Config_Entry {
		Config_Entry server_conf;
		Config_Entry server_misc;

		Config_Lock lock;
	};

	static Server_Config *instance();

	/// server
	void load_server_config(void);
	const Json::Value &server_conf(void);
	const Json::Value &server_misc(void);

private:
	Server_Config(void);
	virtual ~Server_Config(void);
	Server_Config(const Server_Config &);
	const Server_Config &operator=(const Server_Config &);

private:
	static Server_Config *instance_;
	Server_Config_Entry server_config_;
};

#define SERVER_CONFIG Server_Config::instance()

#endif /* SERVER_CONFIG_H_ */
