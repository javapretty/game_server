/*
 *		Daemon_Server.cpp
 *
 *  Created on: 2016年3月26日
 *      Author: zhangyalei
 */

#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <algorithm>
#include "Epoll_Watcher.h"
#include "Time_Value.h"
#include "Log.h"
#include "Daemon.h"
#include "Daemon_Server.h"
#include "Server_Config.h"

Daemon_Server::Daemon_Server(void)
: daemon_map_(64),
  wait_watcher_(0)
{ }

Daemon_Server::~Daemon_Server(void) { }

Daemon_Server *Daemon_Server::instance_;

std::string Daemon_Server::exec_name_ = "./server";

struct option Daemon_Server::long_options[] = {
		{"daemon",	no_argument,		0,	'a'},
		{"log",		no_argument,		0,	'b'},
		{"db",			no_argument,		0,	'c'},
		{"login",	no_argument,		0,	'd'},
		{"master",	no_argument,		0,	'e'},
		{"game",		no_argument,		0,	'f'},
		{"gate",		no_argument,		0,	'g'},
		{"label",	no_argument,		0,	'h'},
		{0, 0, 0, 0}
};

Daemon_Server *Daemon_Server::instance(void) {
    if (! instance_)
        instance_ = new Daemon_Server();
    return instance_;
}

int Daemon_Server::init(int argc, char *argv[]) {
	if ((wait_watcher_ = new Epoll_Watcher) == 0) {
		LOG_FATAL("new Epoll_Watcher return 0");
	}
	server_label_ = "null";
	return 0;
}

int Daemon_Server::start(int argc, char *argv[]) {
	signal(SIGCHLD, sigcld_handle);
	parse_cmd_arguments(argc, argv);
	wait_watcher_->loop();
	return 0;
}

int Daemon_Server::parse_cmd_arguments(int argc, char *argv[]) {
	bool switch_daemon_server = false;
	bool switch_log_server = false;
	bool switch_db_server = false;
	bool switch_login_server = false;
	bool switch_master_server = false;
	bool switch_game_server = false;
	bool switch_gate_server = false;

	int c = 0;
	while ((c = getopt_long(argc, argv, "vdm:", long_options, NULL)) != -1) {
		switch (c) {
		case 'a': { /// daemon server manager
			switch_daemon_server = true;
			break;
		}
		case 'b': { /// log server
			switch_log_server = true;
			break;
		}
		case 'c': { /// db server
			switch_db_server = true;
			break;
		}
		case 'd': { /// login server
			switch_login_server = true;
			break;
		}
		case 'e': { /// master server
			switch_master_server = true;
			break;
		}
		case 'f': { /// game server
			switch_game_server = true;
			break;
		}
		case 'g': { /// gate server
			switch_gate_server = true;
			break;
		}
		case 'h': { /// label
			server_label_ = optarg;
			break;
		}
		default: {
			switch_daemon_server = true;
			break;
		}
		}
	}

	if (switch_daemon_server)
		run_daemon_server();

	if (switch_log_server)
		run_log_server();

	if (switch_db_server)
		run_db_server();

	if (switch_login_server)
		run_login_server();

	if (switch_master_server)
		run_master_server();

	if (switch_game_server)
		run_game_server();

	if (switch_gate_server)
		run_gate_server();

	if (! (switch_daemon_server || switch_log_server || switch_db_server || switch_login_server || switch_gate_server || switch_game_server || switch_master_server)) {
		usage();
		exit(1);
	}

	return 0;
}

void Daemon_Server::usage(void) {
	std::cout << "usage:" << std::endl
			<< "\t-a\tstart daemon server\n"
			<< "\t-b\tstart log server\n"
			<< "\t-c\tstart db server\n"
			<< "\t-d\tstart login server\n"
			<< "\t-e\tstart master server\n"
			<< "\t-f\tstart game server\n"
			<< "\t-g\tstart gate server\n"
			<< " in general, you just use -a arguement to start server.\n" << std::endl;
}

int Daemon_Server::fork_exec_args(const char *exec_str, int server_type) {
	LOG_DEBUG("exec_str = [%s], server_type = %d", exec_str, server_type);

	std::vector<std::string> exec_str_tok;
	std::istringstream exec_str_stream(exec_str);
	std::copy(std::istream_iterator<std::string>(exec_str_stream), std::istream_iterator<std::string>(), std::back_inserter(exec_str_tok));
	if (exec_str_tok.empty()) {
		LOG_FATAL("exec_str = %s", exec_str);
	}

	const char *pathname = (*exec_str_tok.begin()).c_str();
	std::vector<const char *> vargv;
	for (std::vector<std::string>::iterator tok_it = exec_str_tok.begin(); tok_it != exec_str_tok.end(); ++tok_it) {
		vargv.push_back((*tok_it).c_str());
	}
	vargv.push_back((const char *)0);

	pid_t pid = fork();
	if (pid < 0) {
		LOG_INFO("fork");
		return -1;
	} else if (pid == 0) { /// child
		if (execvp(pathname, (char* const*)&*vargv.begin()) < 0) {
			LOG_INFO("execl");
			LOG_FATAL();
		}
	} else { /// parent
		daemon_map_.insert(std::make_pair(pid, server_type));
	}

	return pid;
}

int Daemon_Server::fork_exec_log_server(void) {
	std::stringstream execname_stream;
	execname_stream << exec_name_ << " --log " << "--label=" << server_label_;
	fork_exec_args(execname_stream.str().c_str(), Log::LOG_LOG_SERVER);
	sleep(1); /// wait for log server started
	return 0;
}

int Daemon_Server::fork_exec_db_server(void) {
	std::stringstream execname_stream;
	execname_stream << exec_name_ << " --db " << "--label=" << server_label_;
	fork_exec_args(execname_stream.str().c_str(), Log::LOG_DB_SERVER);
	return 0;
}

int Daemon_Server::fork_exec_login_server(void){
	std::stringstream execname_stream;
	execname_stream << exec_name_ << " --login " <<"--label=" << server_label_;
	fork_exec_args(execname_stream.str().c_str(), Log::LOG_LOGIN_SERVER);
	return 0;
}

int Daemon_Server::fork_exec_master_server(void) {
	std::stringstream execname_stream;
	execname_stream << exec_name_ << " --master " << "--label=" << server_label_;
	fork_exec_args(execname_stream.str().c_str(), Log::LOG_MASTER_SERVER);
	return 0;
}

int Daemon_Server::fork_exec_game_server(void) {
	std::stringstream execname_stream;
	execname_stream << exec_name_ << " --game " << "--label=" << server_label_;
	fork_exec_args(execname_stream.str().c_str(), Log::LOG_GAME_SERVER);
	return 0;
}

int Daemon_Server::fork_exec_gate_server(void) {
	std::stringstream execname_stream;
	execname_stream << exec_name_ << " --gate " << "--label=" << server_label_;
	fork_exec_args(execname_stream.str().c_str(), Log::LOG_GATE_SERVER);
	return 0;
}

void Daemon_Server::sigcld_handle(int signo) {
	LOG_DEBUG("Daemon_Server receive signo = %d.", signo);
	signal(SIGCHLD, sigcld_handle);
	pid_t pid = wait(NULL);
	if (pid < 0) {
		LOG_INFO("wait error");
	}
	DAEMON_SERVER->restart_process(pid);
}

void Daemon_Server::restart_process(int pid) {
	Int_Int_Map::iterator it = daemon_map_.find(pid);
	if (it == daemon_map_.end()) {
		LOG_INFO("cannot find process, pid = %d.", pid);
		return;
	}

	/// cond core dump max times
	Int_Int_Map::iterator core_map_it = core_dump_num_.find(it->second);
	if (core_map_it != core_dump_num_.end()) {
		if (core_map_it->second++ > max_core_dump_num) {
			LOG_INFO("so many core dump, core_dump_num = %d", core_map_it->second);
			return;
		}
	} else {
		core_dump_num_.insert(std::make_pair(it->second, 0));
	}

	switch (it->second) {
	case Log::LOG_LOG_SERVER: {
		fork_exec_log_server();
		break;
	}
	case Log::LOG_DB_SERVER: {
		fork_exec_db_server();
		break;
	}
	case Log::LOG_LOGIN_SERVER: {
		fork_exec_db_server();
		break;
	}
	case Log::LOG_MASTER_SERVER: {
		fork_exec_master_server();
		break;
	}
	case Log::LOG_GAME_SERVER: {
		fork_exec_game_server();
		break;
	}
	case Log::LOG_GATE_SERVER: {
		fork_exec_gate_server();
		break;
	}
	default: {
		LOG_INFO("unknow server type, server_type = %d.", it->second);
		break;
	}
	}
	daemon_map_.erase(pid);
}

void Daemon_Server::record_pid_file(void) {
	FILE *fp = fopen("./daemon.pid", "w");
	if (! fp) {
		LOG_INFO("fopen");
		return;
	}
	char pid_str[128] = {0};
	snprintf(pid_str, sizeof(pid_str), "%d", getpid());
	fputs(pid_str, fp);
	fclose(fp);
}

void Daemon_Server::run_daemon_server(void) {
	if (daemon(1, 0) != 0) {
		LOG_FATAL("daemon return != 0");
	}
	record_pid_file();

	/// start log server
	fork_exec_log_server();

	/// start db server !
	fork_exec_db_server();

	/// start login server !
	fork_exec_login_server();

	/// start master server
	fork_exec_master_server();

	/// start game server
	fork_exec_game_server();

	/// start gate_server
	fork_exec_gate_server();
}

void Daemon_Server::run_log_server(void) {
	SERVER_CONFIG;
	DAEMON_LOG->start_server();
}

void Daemon_Server::run_db_server(void) {
	SERVER_CONFIG;
	DAEMON_DB->start_server();
}

void Daemon_Server::run_login_server(void) {
	SERVER_CONFIG;
	DAEMON_LOGIN->start_server();
}

void Daemon_Server::run_master_server(void) {
	SERVER_CONFIG;
	DAEMON_MASTER->start_server();
}

void Daemon_Server::run_game_server(void) {
	SERVER_CONFIG;
	DAEMON_GAME->start_server();
}

void Daemon_Server::run_gate_server(void) {
	SERVER_CONFIG;
	DAEMON_GATE->start_server();
	DAEMON_GAME->start_client();
	DAEMON_MASTER->start_client();
}
