/*
 *		Daemon_Server.h
 *
 *  Created on: Mar 26, 2016
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
public:
	typedef boost::unordered_map<int, Process_Info> Int_Process_Map;
	typedef boost::unordered_map<int, int> Int_Int_Map;

	const static int max_core_dump_num = 2500;

	static Daemon_Server *instance(void);

	int init(int argc, char *argv[]);
	int start(int argc, char *argv[]);

	Server_Conf &server_conf(void) { return server_conf_; }

private:
	Daemon_Server(void);
	~Daemon_Server(void);
	Daemon_Server(const Daemon_Server &);
	const Daemon_Server &operator=(const Daemon_Server &);

	int parse_cmd_arguments(int argc, char *argv[]);
	int fork_exec_args(const char *exec_str, int server_type, int server_id = 0);

	static void sigcld_handle(int signo);
	void restart_process(int pid);
	void record_pid_file(void);

	void run_daemon_server(void);
	void run_log_server(int server_id);
	void run_db_server(int server_id);
	void run_login_server(int server_id);
	void run_master_server(int server_id);
	void run_game_server(int server_id);
	void run_gate_server(int server_id);

private:
	static std::string exec_name_;
	static Daemon_Server *instance_;
	static struct option long_options[];
	std::string server_label_;

	Int_Process_Map process_map_;
	Int_Int_Map core_dump_num_;

	Epoll_Watcher *wait_watcher_;
	Server_Conf server_conf_;
};

#define DAEMON_SERVER Daemon_Server::instance()

#endif /* DAEMON_SERVER_H_ */
