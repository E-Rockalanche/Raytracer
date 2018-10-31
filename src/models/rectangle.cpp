
#include <iostream>
#include "models.hpp"
#include "material_handler.hpp"

Rectangle::Rectangle(Vec3 position, Vec3 w, Vec3 h, int material_handle)
		: Plane(position, w, h, material_handle) {
	setDimensions(w, h);
}

void Rectangle::setDimensions(Vec3 w, Vec3 h) {
	this->h = h - Vec3::project(h, w);
	this->w = w - Vec3::project(w, h);
}

bool Rectangle::lineIntersection(Vec3 origin, Vec3 direction, Vec3 position,
		Vec3 w, Vec3 h, float& t) {
	bool intersection = false;
	Vec3 normal = Vec3::normalize(Vec3::crossProduct(w, h));
	if (Vec3::dotProduct(direction, normal) < 0) {
		if (Plane::lineIntersection(origin, direction, position, normal, t)) {
			Vec3 point = origin + t * direction;

			// check within width and height
			float w_len = w.length();
			float wt = Vec3::dotProduct(point - position, w) / (w_len * w_len);

			if (wt > -0.00001 && wt < 1.00001) {
				float h_len = h.length();
				float ht = Vec3::dotProduct(point - position, h) / (h_len * h_len);

				intersection = (ht > -0.00001 && ht < 1.00001);
			}
		}
	}
	return intersection;
}

bool Rectangle::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point,
		Vec3* normal, int* material_handle, float* tex_x, float* tex_y, float* distance) const {
	bool collided = false;
	float t;
	if (Rectangle::lineIntersection(origin, direction, position, w, h, t)) {
		assignPointer(collision_point, origin + t * direction);
		assignPointer(normal, this->normal);
		assignPointer(distance, t);
		assignPointer(material_handle, this->material_handle);
		collided = true;
	}
	return collided;
}

std::istream& operator>>(std::istream& in, Rectangle& rec) {
	std::string material_name;
	Vec3 pos, w, h;
	in >> pos >> w >> h >> material_name;
	int handle = MaterialHandler::getHandle(material_name);
	rec = Rectangle(pos, w, h, handle);
	return in;
}
