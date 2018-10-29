
#include <cmath>
#include "models.hpp"
#include "material_handler.hpp"

Sphere::Sphere(Vec3 position, float radius, int material_handle)
	: Model(position, material_handle), radius(radius) {}

bool Sphere::lineIntersection(Vec3 origin, Vec3 direction, Vec3 position, float radius, float& t) {
	bool intersected = false;
	Vec3 dist = origin - position;
	float a = Vec3::dotProduct(direction, direction);
	float b = 2 * Vec3::dotProduct(dist, direction);
	float c = Vec3::dotProduct(dist, dist) - radius*radius;
	if (a != 0.0) {
		float det = b*b - 4.0*a*c;
		if (det >= 0.0) {
			float sqrt_det = std::sqrt(det);
			t = (-b - sqrt_det) / (2.0 * a);

			intersected = (t > 0.0);
		}
	}
	return intersected;
}

bool Sphere::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point,
		Vec3* normal, int* material_handle, float* distance) const {
	bool collided = false;
	float t;
	if (Sphere::lineIntersection(origin, direction, position, radius, t)) {
		assignPointer(distance, t);
		assignPointer(collision_point, origin + t * direction);
		assignPointer(normal, Vec3::normalize(*collision_point - position));
		assignPointer(material_handle, this->material_handle);
		collided = true;
	}
	return collided;
}

std::istream& operator>>(std::istream& in, Sphere& sphere) {
	std::string material_name;
	Vec3 pos;
	float radius;
	in >> pos >> radius >> material_name;
	int handle = MaterialHandler::getHandle(material_name);
	sphere = Sphere(pos, radius, handle);
	return in;
}