#include "models.hpp"

Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Material material)
		: Plane(v1, v2 - v1, v3 - v1, material) {}

bool Triangle::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;

	float dir_dot = Vec3::dotProduct(direction, this->normal);
	if (dir_dot < 0) {
		float t = (constant - Vec3::dotProduct(origin, this->normal)) / dir_dot;
		if (t > 0) {
			Vec3 point = origin + t * direction;

			// check if inside triangle
			bool inside = true;
			Vec3 vertices[3] = { position, position+w, position+h };
			for(int i = 0; i < 3; i++) {
				int j = (i+1)%3;
				Vec3 side_dir = Vec3::crossProduct(this->normal, vertices[j] - vertices[i]);
				Vec3 point_dir = point - vertices[i];
				if (Vec3::dotProduct(side_dir, point_dir) < 0) {
					inside = false;
					break;
				}
			}

			if (inside) {
				assignPointer(collision_point, point);
				assignPointer(normal, this->normal);
				assignPointer(distance, t);
				collided = true;
			}
		}
	}

	return collided;
}