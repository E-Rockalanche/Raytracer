#include "models.hpp"
#include <iostream>

Plane::Plane(Vec3 position, Vec3 w, Vec3 h, Material material)
		: Model(position, material) {
	this->h = h - Vec3::project(h, w);
	this->w = w - Vec3::project(w, h);
	normal = Vec3::normalize(Vec3::crossProduct(this->w, this->h));
	constant = Vec3::dotProduct(position, normal);
}

bool Plane::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;

	float dir_dot = Vec3::dotProduct(direction, this->normal);
	if (dir_dot < 0) {
		float t = (constant - Vec3::dotProduct(origin, this->normal)) / dir_dot;
		if (t > 0) {
			collided = true;
			assignPointer(collision_point, origin + t * direction);
			assignPointer(normal, this->normal);
			assignPointer(distance, t);
		}
	}

	return collided;
}