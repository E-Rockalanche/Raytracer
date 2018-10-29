#include "models.hpp"

Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Material material)
		: Plane(v1, v2 - v1, v3 - v1, material) {}

// static
bool Triangle::lineIntersection(Vec3 origin, Vec3 direction, Vec3 v1, Vec3 v2, Vec3 v3, float* t) {
	bool intersected = false;
	Vec3 normal = Vec3::crossProduct(v2 - v1, v3 - v1);
	if (Plane::lineIntersection(origin, direction, v1, normal, t)) {
		Vec3 point = origin + t * direction;

		// check if inside triangle
		intersected = true;
		Vec3 vertices[3] = { v1, v2, v3 };
		for(int i = 0; i < 3; i++) {
			int j = (i+1)%3;
			Vec3 side_dir = Vec3::crossProduct(normal, vertices[j] - vertices[i]);
			Vec3 point_dir = point - vertices[i];
			if (Vec3::dotProduct(side_dir, point_dir) < 0) {
				intersected = false;
				break;
			}
		}
	}
	return intersected;
}

bool Triangle::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;
	float t;
	if (Triangle::lineIntersection(origin, direction, position, w, h, t)) {
		assignPointer(collision_point, origin + t * direction);
		assignPointer(normal, this->normal);
		assignPointer(distance, t);
		collided = true;
	}
	return collided;
}

std::istream& operator>>(std::istream& in, Triangle& tri) {
	std::string material_name;
	Vec3 v1, v2, v3;
	in >> v1, v2, v3 >> material_name;
	int handle = MaterialHandler::getHandle(material.name);
	tri = Triangle(v1, v2, v3, handle);
}