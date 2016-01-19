/*
  * Public_Define.h
  *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef PUBLIC_DEFINE_H_
#define PUBLIC_DEFINE_H_

#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include <stdint.h>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <limits.h>
#include "Time_Value.h"

#define SLEEP_TIME (Time_Value(0, 100))

typedef int64_t role_id_t;				/// 角色id

typedef std::set<int> Int_Set;
typedef std::set<uint32_t> UInt_Set;
typedef std::vector<int8_t> Int8_Vec;
typedef std::vector<int> Int_Vec;
typedef std::vector<long> Long_Vec;
typedef std::vector<int64_t> Int64Vec;
typedef std::vector<std::string> String_Vec;
typedef std::vector<uint32_t> UInt_Vec;
typedef std::vector<role_id_t> Role_Vec;
typedef std::vector<double> Double_Vec;
typedef std::list<int> Int_List;
typedef std::deque<role_id_t> Role_Deque;
typedef std::deque<int> Int_Deque;
typedef std::deque<uint32_t> UInt_Deque;
typedef std::queue<role_id_t> Role_Queue;
typedef boost::unordered_set<role_id_t> Role_Set;
typedef std::set<role_id_t> Role_Sort_Set;
typedef std::set<long> Long_Set;
typedef std::set<std::string> String_Set;

typedef boost::unordered_map<int, Int_Set> Int_Int_Set_Map;
typedef boost::unordered_map<role_id_t, double> RoleID_Double_Map;
typedef boost::unordered_map<role_id_t, int8_t> RoleID_Int8_Map;
typedef boost::unordered_map<int, int> Int_Int_Map;
typedef boost::unordered_map<role_id_t, Int_Int_Map> Role_IntIntMap_Map;
typedef boost::unordered_map<int, std::string> Int_String_Map;
typedef boost::unordered_map<int8_t, int> Int8_Int_Map;
typedef boost::unordered_map<int, int8_t> Int_Bool_Map;
typedef boost::unordered_map<int, double> Int_Double_Map;
typedef boost::unordered_map<role_id_t, int> Role_Int_Map;
typedef boost::unordered_map<int, Time_Value> Int_Time_Map;
typedef boost::unordered_map<int, Role_Set> Int_Role_Set_Map;

typedef boost::unordered_set<int> Int_Hash_Set;
typedef boost::unordered_set<int64_t> Int64_Hash_Set;
typedef boost::unordered_set<uint32_t> UInt_Hash_Set;
typedef std::vector<Int_Hash_Set > Int_Set_Vec;

typedef boost::unordered_map<role_id_t, uint32_t> RoleID_Uint_Map;
typedef boost::unordered_map<role_id_t, int32_t> RoleID_Int_Map;
typedef boost::unordered_map<role_id_t, RoleID_Int_Map> Role_RoleIntMap_Map;
typedef boost::unordered_map<int32_t, role_id_t> Int_RoleID_Map;
typedef boost::unordered_map<int32_t, role_id_t> Int_RoleID_Map;
typedef boost::unordered_map<role_id_t, role_id_t> RoleID_RoleID_Map;
typedef boost::unordered_map<int, RoleID_Int_Map> Int_Role_Id_Int_Map;
typedef boost::unordered_map<role_id_t, Time_Value> Role_Time_Map;
typedef boost::unordered_map<role_id_t, Int_Vec> Role_IntVec_Map;

typedef boost::unordered_map<role_id_t, Int_Vec> RoleID_IntVec_Map;
typedef boost::unordered_map<int, Int_Set> Int_IntSet_Map;
typedef boost::unordered_map<int, Int_Hash_Set> Int_IntHashSet_Map;
typedef boost::unordered_map<int, Int_Int_Map> Int_IntIntMap_Map;
typedef boost::unordered_map<int, Int_Vec> Int_IntVec_Map;
typedef std::map<int, Int_Int_Map > Int_IntIntSort_Map;
typedef std::map<int, int> Int_Int_Sort_Map;
typedef std::map<Time_Value, int> Time_Int_Sort_Map;

typedef std::vector<Int_Int_Map> Int_Int_Map_Vec;
typedef std::vector<Int_Vec> IntVec_Vec;
typedef std::vector<Int_Int_Map> IntIntMap_Vec;
typedef std::vector<IntIntMap_Vec> IntInt_Vec_Vec;

typedef boost::unordered_set<std::string> String_List;

typedef boost::unordered_map<std::string, int64_t> Str_Int_Map;
typedef boost::unordered_map<std::string, std::string> Str_Str_Map;
typedef std::vector<Time_Value> Time_Value_Vec;

#endif /* PUBLIC_DEFINE_H_ */
