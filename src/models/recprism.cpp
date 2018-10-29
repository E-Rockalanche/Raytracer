#include <cfloat>
#include "models.hpp"

RecPrism::RecPrism(Vec3 position, Vec3 w, Vec3 h, Vec3 d, Material material)
		: Model(position, material) {
	faces[0] = Rectangle(position, w, h, material);
	faces[1] = Rectangle(position + w, d, h, material);
	faces[2] = Rectangle(position + d, h, w, material);
	faces[3] = Rectangle(position, h, d, material);
	faces[4] = Rectangle(position + h, w, d, material);
	faces[5] = Rectangle(position, d, w, material);
}

bool RecPrism::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;
	float min_distance = FLT_MAX;

	for(int i = 0; i < FACES; i++) {
		Vec3 cur_point;
		Vec3 cur_normal;
		float cur_distance = FLT_MAX;
		bool cur_collided = faces[i].lineCollision(origin, direction, &cur_point, &cur_normal, &cur_distance);
		if (cur_collided && (cur_distance < min_distance)) {
			min_distance = cur_distance;
			assignPointer(collision_point, cur_point);
			assignPointer(normal, cur_normal);
			assignPointer(distance, cur_distance);
			collided = true;
		}
	}

	return collided;
}