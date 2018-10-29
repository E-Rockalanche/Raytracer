#include "models.hpp"
#include <iostream>

Rectangle::Rectangle(Vec3 position, Vec3 w, Vec3 h, int material_handle)
		: Plane(position, w, h, material_handle) {
	setDimensions(w, h);
}

Rectangle::setDimensions(Vec3 w, Vec3 h) {
	this->h = h - Vec3::project(h, w);
	this->w = w - Vec3::project(w, h);
}

bool Rectangle::lineIntersection(Vec3 origin, Vec3 direction, Vec3 position, Vec3 normal, Vec3 w, Vec3 h, float& t) {
	bool intersection = false;
	if (Plane::lineIntersection(origin, direction, position, normal, t)) {
		Vec3 point = origin + t * direction;

		// check within width and height
		float w_len = w.length();
		float wt = Vec3::dotProduct(point - position, w) / (w_len * w_len);

		if (wt >= 0 && wt <= 1) {
			float h_len = h.length();
			float ht = Vec3::dotProduct(point - position, h) / (h_len * h_len);

			intersection = (ht >= 0 && ht <= 1);
		}
	}
	return intersection;
}

bool Rectangle::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;
	float t;
	if (Rectangle::lineIntersetion(origin, direction, position, this->normal, w, h, t)) {
		assignPointer(collision_point, origin + t * direction);
		assignPointer(normal, this->normal);
		assignPointer(distance, t);
		collided = true;
	}
	return collided;
}

std::istream& operator>>(std::istream& in, Rectangle& rec) {
	std::string material_name;
	Vec3 pos, w, h;
	in >> pos >> w >> h >> material_name;
	int handle = MaterialHandler::getHandle(material.name);
	rec = Rectangle(position, w, h, handle);
}