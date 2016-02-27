// -*- C++ -*-
/*
 * Configurator.h
 *
 *  Created on: Dec 24, 2015
 *      Author: zhangyalei
 */

#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include "Mutex_Guard.h"
#include "Thread_Mutex.h"
#include "json/json.h"
#include "boost/unordered_map.hpp"

class Configurator {
public:
	typedef RE_MUTEX Config_Lock;

	typedef boost::unordered_map<std::string, Json::Value> Config_Map;
	typedef boost::unordered_map<int, Json::Value> Int_Json_Map;

	typedef void (*JsonKey_Func)(const Json::Value &json, int &key);

	struct Config_Entry {
		Config_Entry(void) : cur_version(0) { }

		inline void cover_cur_json_to_map(void) {
			get_cur_map_json().clear();
			for (Json::Value::iterator it = get_cur_json().begin(); it != get_cur_json().end(); ++it)
				get_cur_map_json().insert(std::make_pair(atoi(it.key().asCString()), *it));
		}
		inline void cover_next_json_to_map(void) {
			get_next_map_json().clear();
			for (Json::Value::iterator it = get_next_json().begin(); it != get_next_json().end(); ++it)
				get_next_map_json().insert(std::make_pair(atoi(it.key().asCString()), *it));
		}

		inline int get_cur_version(void) { return cur_version; }
		inline int get_next_version(void) {
			return (!cur_version);
		}

		inline void update_version(void) {
			cur_version = (cur_version + 1) % 2;
		}

		inline Json::Value &get_cur_json(void) { return json[get_cur_version()]; }
		inline Json::Value &get_next_json(void) { return json[get_next_version()]; }

		inline Int_Json_Map &get_cur_map_json(void) { return json_map[get_cur_version()]; }
		inline Int_Json_Map &get_next_map_json(void) { return json_map[get_next_version()]; }

		inline void clear_cur_json(void) { json[get_cur_version()].clear(); }
		inline void clear_next_json(void) { json[get_next_version()].clear(); }

		Json::Value json[2];
		Int_Json_Map json_map[2];
		int cur_version;
	};

public:
	struct Server_Config {
		Config_Entry log_misc;
		Config_Entry server_conf;
		Config_Entry server_list;
		Config_Entry server_maintainer;
		Config_Entry server_misc;

		Config_Lock lock;
	};

	static Configurator *instance();
	int load_json_file(const char *file_path, Json::Value &conf_value);

	inline void load_config_with_wlock(const std::string &path, Config_Lock &lock, Config_Entry &config);
	inline void load_config_map_with_wlock(const std::string &path, Config_Lock &lock, Config_Entry &config);

	inline const Json::Value &get_json_value_in_map(const int id, Config_Entry &config);
	inline const Json::Value &get_json_value_with_rlock(Config_Lock &lock, Config_Entry &config);
	inline const Json::Value &get_json_value_with_rlock_in_map(Config_Lock &lock, const int id, Config_Entry &config);

	int hot_update_conf(std::string &module, int server = 0);
	void load_all_config(void);

	/// server
	void load_server_config(void);
	const Json::Value &log_misc(void);
	const Json::Value &server_conf(void);
	const Json::Value &server_list(void);
	const Json::Value &server_maintainer(void);
	const Json::Value &server_misc(void);

private:
	Configurator(void);
	virtual ~Configurator(void);
	Configurator(const Configurator &);
	const Configurator &operator=(const Configurator &);

private:
	Json::Reader reader_;
	static Configurator *instance_;

	Server_Config server_config_;
};

///////////////////////////////////////////////////////////////////////////////

#define CONFIG_INSTANCE			Configurator::instance()

inline const Json::Value &Configurator::get_json_value_in_map(const int id, Config_Entry &config) {
	Int_Json_Map &map = config.get_cur_map_json();
	Int_Json_Map::const_iterator it = map.find(id);
	if (it != map.end()) {
		return it->second;
	} else {
		return Json::Value::null;
	}
}

inline const Json::Value &Configurator::get_json_value_with_rlock_in_map(Config_Lock &lock, const int id, Config_Entry &config) {
	GUARD_READ(Config_Lock, mon, lock);
	return get_json_value_in_map(id, config);
}

inline const Json::Value &Configurator::get_json_value_with_rlock(Config_Lock &lock, Config_Entry &config) {
	GUARD_READ(Config_Lock, mon, lock);
	return config.get_cur_json();
}


#endif /* CONFIGURATOR_H_ */
