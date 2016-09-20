/*
 *		Daemon_Server.cpp
 *
 *  Created on: Mar 26, 2016
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
: process_map_(64),
  wait_watcher_(0)
{ }

Daemon_Server::~Daemon_Server(void) { }

Daemon_Server *Daemon_Server::instance_;

std::string Daemon_Server::exec_name_ = "./server";

struct option Daemon_Server::long_options[] = {
		{"daemon",		no_argument,				0,	'a'},
		{"log",				no_argument,				0,	'b'},
		{"db",				no_argument,				0,	'c'},
		{"login",			no_argument,				0,	'd'},
		{"master",		no_argument,				0,	'e'},
		{"game",			no_argument,				0,	'f'},
		{"gate",			no_argument,				0,	'g'},
		{"label",			required_argument,	0,	'h'},
		{"server_id",required_argument,	0,	'i'},
		{0, 0, 0, 0}
};

Daemon_Server *Daemon_Server::instance(void) {
    if (! instance_)
        instance_ = new Daemon_Server();
    return instance_;
}

int Daemon_Server::init(int argc, char *argv[]) {
	if ((wait_watcher_ = new Epoll_Watcher) == 0) {
		LOG_FATAL("new Epoll_Watcher fatal");
	}
	server_conf_.init_server_conf();

	const Json::Value server_misc = SERVER_CONFIG->server_misc();
	Lib_Log::instance()->set_file_switcher(server_misc["lib_log_switcher"].asInt());
	Log::instance()->set_file_switcher(server_misc["server_log_switcher"].asInt());
	server_label_ = server_misc["server_label"].asString();
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

	int server_id;
	int c = 0;
	while ((c = getopt_long(argc, argv, "vdm:", long_options, NULL)) != -1) {
		switch (c) {
		case 'a': { //daemon server manager
			switch_daemon_server = true;
			break;
		}
		case 'b': { //log server
			switch_log_server = true;
			break;
		}
		case 'c': { //db server
			switch_db_server = true;
			break;
		}
		case 'd': { //login server
			switch_login_server = true;
			break;
		}
		case 'e': { //master server
			switch_master_server = true;
			break;
		}
		case 'f': { //game server
			switch_game_server = true;
			break;
		}
		case 'g': { //gate server
			switch_gate_server = true;
			break;
		}
		case 'h': { //label
			server_label_ = optarg;
			break;
		}
		case 'i': { //server_id
			server_id = atoi(optarg);
			break;
		}
		default: {
			switch_daemon_server = true;
			break;
		}
		}
	}

	if (!(switch_log_server || switch_db_server || switch_login_server || switch_gate_server || switch_game_server || switch_master_server)) {
		switch_daemon_server = true;
	}

	if (switch_daemon_server)
		run_daemon_server();

	if (switch_log_server)
		run_log_server(server_id);

	if (switch_db_server)
		run_db_server(server_id);

	if (switch_login_server)
		run_login_server(server_id);

	if (switch_master_server)
		run_master_server(server_id);

	if (switch_game_server)
		run_game_server(server_id);

	if (switch_gate_server)
		run_gate_server(server_id);

	return 0;
}

int Daemon_Server::fork_exec_args(const char *server_name, int server_type, int server_id) {
	std::stringstream execname_stream;
	execname_stream << exec_name_ << server_name <<" --server_id=" << server_id << " --label=" << server_label_;
	LOG_INFO("exec_str=[%s], server_id=%d", execname_stream.str().c_str(), server_id);

	std::vector<std::string> exec_str_tok;
	std::istringstream exec_str_stream(execname_stream.str().c_str());
	std::copy(std::istream_iterator<std::string>(exec_str_stream), std::istream_iterator<std::string>(), std::back_inserter(exec_str_tok));
	if (exec_str_tok.empty()) {
		LOG_FATAL("exec_str = %s", execname_stream.str().c_str());
	}

	const char *pathname = (*exec_str_tok.begin()).c_str();
	std::vector<const char *> vargv;
	for (std::vector<std::string>::iterator tok_it = exec_str_tok.begin(); tok_it != exec_str_tok.end(); ++tok_it) {
		vargv.push_back((*tok_it).c_str());
	}
	vargv.push_back((const char *)0);

	pid_t pid = fork();
	if (pid < 0) {
		LOG_FATAL("fork fatal, pid:%d", pid);
	} else if (pid == 0) { /// child
		if (execvp(pathname, (char* const*)&*vargv.begin()) < 0) {
			LOG_FATAL("execvp %s fatal", pathname);
		}
	} else { /// parent
		Process_Info info;
		info.server_type = server_type;
		info.server_id = server_id;
		process_map_.insert(std::make_pair(pid, info));
		LOG_WARN("fork new process, pid:%d, server_type:%d, server_id:%d", pid, server_type, server_id);
	}

	return pid;
}

void Daemon_Server::sigcld_handle(int signo) {
	LOG_ERROR("Daemon_Server receive signo = %d.", signo);
	signal(SIGCHLD, sigcld_handle);
	pid_t pid = wait(NULL);
	if (pid < 0) {
		LOG_ERROR("wait error, pid:%d", pid);
	}
	sleep(5);
	DAEMON_SERVER->restart_process(pid);
}

void Daemon_Server::restart_process(int pid) {
	Int_Process_Map::iterator iter = process_map_.find(pid);
	if (iter == process_map_.end()) {
		LOG_ERROR("cannot find process, pid = %d.", pid);
		return;
	}

	//core太多，服务器不再重启
	Int_Int_Map::iterator core_map_it = core_dump_num_.find(iter->second.server_type);
	if (core_map_it != core_dump_num_.end()) {
		if (core_map_it->second++ > max_core_dump_num) {
			LOG_ERROR("so many core dump, core_dump_num = %d", iter->second.server_type);
			return;
		}
	} else {
		core_dump_num_.insert(std::make_pair(iter->second.server_type, 0));
	}

	switch (iter->second.server_type) {
	case LOG_LOG_SERVER: {
		fork_exec_args(" --log", LOG_LOG_SERVER, server_conf_.log_server.server_id);
		break;
	}
	case LOG_DB_SERVER: {
		fork_exec_args(" --db", LOG_DB_SERVER, server_conf_.db_server.server_id);
		break;
	}
	case LOG_LOGIN_SERVER: {
		fork_exec_args(" --login", LOG_LOGIN_SERVER, server_conf_.login_client_server.server_id);
		break;
	}
	case LOG_MASTER_SERVER: {
		fork_exec_args(" --master", LOG_MASTER_SERVER, server_conf_.master_gate_server.server_id);
		break;
	}
	case LOG_GAME_SERVER: {
		fork_exec_args(" --game", LOG_GAME_SERVER, iter->second.server_id);
		break;
	}
	case LOG_GATE_SERVER: {
		fork_exec_args(" --gate", LOG_GATE_SERVER, iter->second.server_id);
		break;
	}
	default: {
		LOG_ERROR("server type:%d error", iter->second);
		break;
	}
	}
	process_map_.erase(pid);
}

void Daemon_Server::record_pid_file(void) {
	FILE *fp = fopen("./daemon.pid", "w");
	if (! fp) {
		LOG_ERROR("fopen daemon.pid error");
		return;
	}
	char pid_str[128] = {0};
	snprintf(pid_str, sizeof(pid_str), "%d", getpid());
	fputs(pid_str, fp);
	fclose(fp);
}

void Daemon_Server::run_daemon_server(void) {
	//if (daemon(1, 0) != 0) {
	//	LOG_FATAL("daemon fatal");
	//}
	//record_pid_file();

	//start log server
	fork_exec_args(" --log", LOG_LOG_SERVER, server_conf_.log_server.server_id);

	//start db server
	fork_exec_args(" --db", LOG_DB_SERVER, server_conf_.db_server.server_id);

	//start login server
	fork_exec_args(" --login", LOG_LOGIN_SERVER, server_conf_.login_client_server.server_id);

	//start master server
	fork_exec_args(" --master", LOG_MASTER_SERVER, server_conf_.master_gate_server.server_id);

	//start game server
	for(Server_List::iterator iter = server_conf_.game_list.begin();
			iter != server_conf_.game_list.end(); iter++) {
		fork_exec_args(" --game", LOG_GAME_SERVER, iter->server_id);
	}

	//start gate_server
	for(Server_List::iterator iter = server_conf_.gate_list.begin();
			iter != server_conf_.gate_list.end(); iter++) {
		fork_exec_args(" --gate", LOG_GATE_SERVER, iter->server_id);
	}
}

void Daemon_Server::run_log_server(int server_id) {
	DAEMON_LOG->start_server(server_id);
	DAEMON_LOG->start_client();
}

void Daemon_Server::run_db_server(int server_id) {
	DAEMON_DB->start_server(server_id);
	DAEMON_DB->start_client();
}

void Daemon_Server::run_login_server(int server_id) {
	DAEMON_LOGIN->start_server(server_id);
	DAEMON_LOGIN->start_client();
}

void Daemon_Server::run_master_server(int server_id) {
	DAEMON_MASTER->start_server(server_id);
	DAEMON_MASTER->start_client();
}

void Daemon_Server::run_game_server(int server_id) {
	DAEMON_GAME->start_server(server_id);
	DAEMON_GAME->start_client();
}

void Daemon_Server::run_gate_server(int server_id) {
	DAEMON_GATE->start_server(server_id);
	DAEMON_GATE->start_client();
}
