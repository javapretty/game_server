/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "Game_Scene.h"
#include "Public_Struct.h"
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"

typedef boost::unordered_map<SCENE_ID, Game_Scene *> SCENES_MAP;

class Scene_Manager {
private:
	Scene_Manager();
	~Scene_Manager();
public:
	static Scene_Manager *instance();
	int load_scene();
	int create_new_scene(int type);
	Game_Scene *get_scene(SCENE_ID scene_id);
	void tick(Time_Value now);
	Thread_Mutex &lock();
	bool has_scene(int scene_id);

	inline ENTITY_ID get_next_id(){return auto_allocated_id_++;}
private:
	int create_scene_id(int type);
private:
	static Scene_Manager *instance_;
	SCENES_MAP scenes_map_;
	ENTITY_ID auto_allocated_id_;
	int aoi_broadcast_interval_;
	Time_Value last_sync_;
	Thread_Mutex lock_;
};

#define SCENE_MANAGER Scene_Manager::instance()

#endif
