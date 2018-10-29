#include "models.hpp"

Plane::Plane(Vec3 position, Vec3 w, Vec3 h, Material material)
		: Model(position, material), w(w), h(h) {
	normal = Vec3::normalize(Vec3::crossProduct(w, h));
	constant = Vec3::dotProduct(position, normal);

	std::cout << "w = " << w << ", h = " << h << '\n';
}

bool Plane::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;

	float dir_dot = Vec3::dotProduct(direction, this->normal);
	if (dir_dot != 0) {
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