/*
 *		Daemon_Server.h
 *
 *  Created on: 2016年3月26日
 *      Author: zhangyalei
 */

#ifndef DAEMON_SERVER_H_
#define DAEMON_SERVER_H_

#include <getopt.h>
#include <vector>
#include <string>
#include "boost/unordered_map.hpp"

class Epoll_Watcher;
class Daemon_Server {
	typedef boost::unordered_map<int, int> Int_Int_Map;

public:
	const static int max_core_dump_num = 2500;

	static Daemon_Server *instance(void);

	int init(int argc, char *argv[]);
	int start(int argc, char *argv[]);

private:
	Daemon_Server(void);
	~Daemon_Server(void);
	Daemon_Server(const Daemon_Server &);
	const Daemon_Server &operator=(const Daemon_Server &);

	int parse_cmd_arguments(int argc, char *argv[]);
	void usage(void);

	int fork_exec_args(const char *exec_str, int server_type);
	int fork_exec_log_server(void);
	int fork_exec_db_server(void);
	int fork_exec_login_server(void);
	int fork_exec_master_server(void);
	int fork_exec_game_server(void);
	int fork_exec_gate_server(void);

	static void sigcld_handle(int signo);
	void restart_process(int pid);

	void record_pid_file(void);

	void run_daemon_server(void);
	void run_log_server(void);
	void run_db_server(void);
	void run_login_server(void);
	void run_master_server(void);
	void run_game_server(void);
	void run_gate_server(void);

private:
	static std::string exec_name_;
	static Daemon_Server *instance_;
	static struct option long_options[];

	std::string server_label_;

	Int_Int_Map daemon_map_;
	Int_Int_Map core_dump_num_;

	Epoll_Watcher *wait_watcher_;
};

#define DAEMON_SERVER Daemon_Server::instance()

#endif /* DAEMON_SERVER_H_ */
