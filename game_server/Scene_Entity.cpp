/*
 *  Created on: Jun 21, 2016
 *      Author: lijunliang
 */

#include <Scene_Entity.h>

Position3D::Position3D(float posx = 0, float posy = 0, float posz = 0):
		x(posx),
		y(posy),
		z(posz)
{
}

Position3D::~Position3D(){

}

float Position3D::operator - (Position3D pos){
		return (float)sqrt(pow(x - pos.x, 2) + pow(y - pos.y, 2) + pow(z - pos.z, 2));
}

void Position3D::setPosition(float posx = 0, float posy = 0, float posz = 0){
		x = posx; y=posy; z=posz;
}

void Position3D::setPosition(Position3D pos){
	setPosition(pos.x, pos.y, pos.z);
}

void Position3D::serialize(Block_Buffer &buffer) const {
	buffer.write_double(x);
	buffer.write_double(y);
	buffer.write_double(z);
}

int Position3D::deserialize(Block_Buffer &buffer) {
	x = buffer.read_double();
	y = buffer.read_double();
	z = buffer.read_double();
	return 0;
}

void Position3D::reset(void) {
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

void Position3D::print(void) {}

Scene_Entity::Scene_Entity():
	entity_id_(0),
	player_id_(0),
	pos_(0, 0, 0),
	speedx_(0),
	speedy_(0),
	speedz_(0),
	scene_(0),
	aoi_map_(),
	need_sync_(false)
{

}

Scene_Entity::~Scene_Entity() {

}
