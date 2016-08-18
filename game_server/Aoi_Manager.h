/*
 *  Created on: Aug 10, 2016
 *      Author: lijunliang
 */

#ifndef AOI_MANAGER_H_
#define AOI_MANAGER_H_

#include <Aoi_Entity.h>

class Aoi_Manager {
public:
	Aoi_Manager();
	~Aoi_Manager();
	int on_enter_aoi(Aoi_Entity *entity);
	int on_move_aoi(Aoi_Entity *entity);
	int on_leave_aoi(Aoi_Entity *entity);

private:
	void insert_entity(Aoi_Entity *entity);
	void update_list(Aoi_Entity *entity, bool direct, int xy);
	void update_aoi_map(Aoi_Entity *entity);
private:
	AOI_LIST x_list_;
	AOI_LIST y_list_;
};

#endif
