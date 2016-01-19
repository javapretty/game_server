// -*- C++ -*-
/*
 * main.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#include "Debug_Starter.h"
#include "Time_Value.h"
#include <signal.h>

static void sighandler(int sig_no) { exit(0); } /// for gprof need normal exit

int main(int argc, char *argv[]) {
	srand(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	srandom(Time_Value::gettimeofday().sec() + Time_Value::gettimeofday().usec());
	signal(SIGPIPE, SIG_IGN);
	signal(SIGUSR1, sighandler);

	Debug_Starter::instance()->init(argc, argv);
	Debug_Starter::instance()->start(argc, argv);
	return 0;
}
