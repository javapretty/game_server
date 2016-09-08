/*
 *  Created on: Aug 10, 2016
 *      Author: lijunliang
 */

#ifndef AOI_MANAGER_H_
#define AOI_MANAGER_H_

#include "Object_Pool.h"
#include "Aoi_Entity.h"

typedef Object_Pool<Aoi_Entity, Spin_Lock> Aoi_Entity_Pool;

class Aoi_Manager {
public:
	Aoi_Manager();
	~Aoi_Manager();
	static Aoi_Entity *create_aoi_entity(Scene_Entity *entity);
	static void reclaim_aoi_entity(Aoi_Entity *entity);
	int on_enter_aoi(Aoi_Entity *entity);
	int on_update_aoi(Aoi_Entity *entity);
	int on_leave_aoi(Aoi_Entity *entity);

private:
	void insert_entity(Aoi_Entity *entity);
	void update_list(Aoi_Entity *entity, bool direct, int xy);
	void update_aoi_map(Aoi_Entity *entity);
private:
	static Aoi_Entity_Pool aoi_entity_pool_;
	AOI_LIST x_list_;
	AOI_LIST y_list_;
};

#endif
