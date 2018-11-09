#define _USE_MATH_DEFINES
#include <cmath>
#include "models.hpp"
#include "material_handler.hpp"

#define dout(message)

Sphere::Sphere(Vec3 position, float radius, int material_handle)
	: Model(position, material_handle), radius(radius) {}

bool Sphere::lineIntersection(Vec3 origin, Vec3 direction, Vec3 position,
		float radius, float& t) {
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

			if (t >= 0) {
				intersected = true;
			} else {
				t = (-b + sqrt_det) / (2.0 * a);
				if (t >= 0) {
					intersected = true;
				}
			}
		}
	}

	return intersected;
}

bool Sphere::lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const {
	bool collided = false;
	float t;
	if (Sphere::lineIntersection(origin, direction, position, radius, t)) {
		collided = true;

		if (collision_data) {
			collision_data->distance = t;
			collision_data->collision_point = origin + t * direction;
			collision_data->normal = (collision_data->collision_point - position)
				/ radius;
			collision_data->material_handle = material_handle;

			const Material& mat = MaterialHandler::getMaterial(material_handle);
			if (mat.diffuse_tex_handle >= 0) {
				collision_data->tex_y = std::acos(Vec3::dotProduct(collision_data->normal,
					Vec3(0, 1, 0))) / M_PI;
				float x_dot = Vec3::dotProduct(collision_data->normal, Vec3(1, 0, 0));
				float z_dot = Vec3::dotProduct(collision_data->normal, Vec3(0, 0, 1));
				collision_data->tex_x = (z_dot >= 0) ? (std::acos(x_dot) / (2*M_PI))
					: (1 - (std::acos(x_dot) / (2*M_PI)));
			}
		}
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