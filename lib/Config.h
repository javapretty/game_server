/*
 * Config.h
 *
 *  Created on: Mar 1,2016
 *      Author: zhangyalei
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "Mutex_Guard.h"
#include "Thread_Mutex.h"
#include "json/json.h"
#include "boost/unordered_map.hpp"

class Config {
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
	Config(void);
	virtual ~Config(void);

	int load_json_file(const char *file_path, Json::Value &conf_value);
	inline const Json::Value &get_json_value_in_map(const int id, Config_Entry &config);
	inline const Json::Value &get_json_value_with_rlock(Config_Lock &lock, Config_Entry &config);
	inline const Json::Value &get_json_value_with_rlock_in_map(Config_Lock &lock, const int id, Config_Entry &config);

private:
	Json::Reader reader_;
};

///////////////////////////////////////////////////////////////////////////////
inline const Json::Value &Config::get_json_value_in_map(const int id, Config_Entry &config) {
	Int_Json_Map &map = config.get_cur_map_json();
	Int_Json_Map::const_iterator it = map.find(id);
	if (it != map.end()) {
		return it->second;
	} else {
		return Json::Value::null;
	}
}

inline const Json::Value &Config::get_json_value_with_rlock_in_map(Config_Lock &lock, const int id, Config_Entry &config) {
	GUARD_READ(Config_Lock, mon, lock);
	return get_json_value_in_map(id, config);
}

inline const Json::Value &Config::get_json_value_with_rlock(Config_Lock &lock, Config_Entry &config) {
	GUARD_READ(Config_Lock, mon, lock);
	return config.get_cur_json();
}

#endif /* CONFIG_H_ */
