#include "models.hpp"
#include <cmath>

Sphere::Sphere(Vec3 position, float radius, Material material)
	: Model(position, material), radius(radius) {}

bool Sphere::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;

	Vec3 dist = origin - position;
	float a = Vec3::dotProduct(direction, direction);
	float b = 2 * Vec3::dotProduct(dist, direction);
	float c = Vec3::dotProduct(dist, dist) - radius*radius;

	if (a != 0.0) {
		float det = b*b - 4.0*a*c;
		if (det >= 0.0) {
			float sqrt_det = std::sqrt(det);
			float t = (-b - sqrt_det) / (2.0 * a);
			if (t <= 0.0) {
				t = (-b + sqrt_det) / (2.0 * a);
			}

			if (t > 0.0) {
				assignPointer(distance, t);
				assignPointer(collision_point, origin + t * direction);
				assignPointer(normal, Vec3::normalize(*collision_point - position));
				collided = true;
			}
		}
	}

	return collided;
}
