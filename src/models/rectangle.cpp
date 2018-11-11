#include <iostream>
#include "rectangle.hpp"
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
		Vec3 w, Vec3 h, float& t, float& wt, float& ht) {
	bool intersection = false;
	Vec3 normal = Vec3::normalize(Vec3::crossProduct(w, h));
	if (Vec3::dotProduct(direction, normal) < 0) {
		if (Plane::lineIntersection(origin, direction, position, normal, t)) {
			Vec3 point = origin + t * direction;

			// check within width and height
			wt = Vec3::dotProduct(point - position, w) / Vec3::dotProduct(w, w);

			if (wt > -0.00001 && wt < 1.00001) {
				ht = Vec3::dotProduct(point - position, h) / Vec3::dotProduct(h, h);

				intersection = (ht > -0.00001 && ht < 1.00001);
			}
		}
	}
	return intersection;
}

bool Rectangle::lineCollision(Vec3 origin, Vec3 direction, CollisionData* collision_data) const {
	bool collided = false;
	float t, wt, ht;
	if (Rectangle::lineIntersection(origin, direction, position, w, h, t, wt, ht)) {
		collided = true;

		if (collision_data) {
			collision_data->collision_point = origin + t * direction;
			collision_data->normal = normal;
			collision_data->distance = t;
			collision_data->material_handle = material_handle;
			collision_data->tex_x = wt;
			collision_data->tex_y = ht;
		}
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
