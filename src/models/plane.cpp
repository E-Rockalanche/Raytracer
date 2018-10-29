
#include <iostream>
#include "models.hpp"
#include "material_handler.hpp"

Plane::Plane(Vec3 position, Vec3 w, Vec3 h, int material_handle)
		: Model(position, material_handle), w(w), h(h) {
	init();
}

void Plane::init() {
	normal = Vec3::normalize(Vec3::crossProduct(w, h));
	constant = Vec3::dotProduct(position, normal);
}

// static
bool Plane::lineIntersection(Vec3 origin, Vec3 direction, Vec3 position,
		Vec3 normal, float& t) {
	bool intersection = false;
	float dir_dot = Vec3::dotProduct(direction, normal);
	if (dir_dot < 0) {
		t = (Vec3::dotProduct(position - origin, normal)) / dir_dot;
		intersection = (t > 0);
	}
	return intersection;
}

bool Plane::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point,
		Vec3* normal, int* material_handle, float* distance) const {
	bool collided = false;
	float t;
	if (Plane::lineIntersection(origin, direction, position, this->normal, t)) {
		collided = true;
		assignPointer(collision_point, origin + t * direction);
		assignPointer(normal, this->normal);
		assignPointer(distance, t);
		assignPointer(material_handle, this->material_handle);
	}
	return collided;
}

std::istream& operator>>(std::istream& in, Plane& plane) {
	std::string material_name;
	Vec3 pos, w, h;
	in >> pos >> w >> h >> material_name;
	int handle = MaterialHandler::getHandle(material_name);
	plane = Plane(pos, w, h, handle);
	return in;
}