#include "models.hpp"
#include <iostream>

Rectangle::Rectangle(Vec3 position, Vec3 w, Vec3 h, Material material)
		: Plane(position, w, h, material) {
	this->h = h - Vec3::project(h, w);
	this->w = w - Vec3::project(w, h);
}

bool Rectangle::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;

	float dir_dot = Vec3::dotProduct(direction, this->normal);
	if (dir_dot < 0) {
		float t = (constant - Vec3::dotProduct(origin, this->normal)) / dir_dot;
		if (t > 0) {
			Vec3 point = origin + t * direction;

			// within width?
			float w_len = w.length();
			float wt = Vec3::dotProduct(point - position, w) / (w_len * w_len);
			if (wt >= 0 && wt <= 1) {

				// within height?
				float h_len = h.length();
				float ht = Vec3::dotProduct(point - position, h) / (h_len * h_len);
				if (ht >= 0 && ht <= 1) {
					assignPointer(collision_point, point);
					assignPointer(normal, this->normal);
					assignPointer(distance, t);
					collided = true;
				}
			}
		}
	}

	return collided;
}