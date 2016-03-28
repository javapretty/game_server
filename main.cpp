/*
 * main.cpp
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#include <signal.h>
#include "Time_Value.h"
#include "Debug_Server.h"
#include "Daemon_Server.h"
#include "Server_Config.h"

static void sighandler(int sig_no) { exit(0); } /// for gprof need normal exit

int main(int argc, char *argv[]) {
	srand(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	signal(SIGPIPE, SIG_IGN);
	signal(SIGUSR1, sighandler);

	SERVER_CONFIG->load_server_config();
	if (SERVER_CONFIG->server_misc()["server_type"].asInt() == 1) {
		DEBUG_SERVER->init(argc, argv);
		DEBUG_SERVER->start(argc, argv);
	} else {
		DAEMON_SERVER->init(argc, argv);
		DAEMON_SERVER->start(argc, argv);
	}

	return 0;
}
