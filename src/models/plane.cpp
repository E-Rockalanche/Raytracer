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
	if (dir_dot != 0) {
		t = (Vec3::dotProduct(position - origin, normal)) / dir_dot;
		intersection = (t > 0);
	}
	return intersection;
}

bool Plane::lineCollision(Vec3 origin, Vec3 direction, CollisionData* collision_data) const {
	bool collided = false;
	float t;
	if (Plane::lineIntersection(origin, direction, position, normal, t)) {
		collided = true;
		
		if (collision_data) {
			collision_data->collision_point = origin + t * direction;
			collision_data->normal = normal;
			collision_data->distance = t;
			collision_data->material_handle = material_handle;
			collision_data->tex_x = Vec3::dotProduct(collision_data->collision_point - position, w)
				/ Vec3::dotProduct(w, w);
			collision_data->tex_y = Vec3::dotProduct(collision_data->collision_point - position, h)
				/ Vec3::dotProduct(h, h);
		}
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
