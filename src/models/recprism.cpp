#include <cfloat>
#include "models.hpp"
#include "material_handler.hpp"

RecPrism::RecPrism(Vec3 position, Vec3 w, Vec3 h, Vec3 d, int material_handle)
		: Model(position, material_handle) {
	faces[0] = Rectangle(position + d, w, h, material_handle);
	faces[1] = Rectangle(position + w, h, d, material_handle);
	faces[2] = Rectangle(position, h, w, material_handle);
	faces[3] = Rectangle(position, d, h, material_handle);
	faces[4] = Rectangle(position + h, d, w, material_handle);
	faces[5] = Rectangle(position, w, d, material_handle);
}

bool RecPrism::lineCollision(Vec3 origin, Vec3 direction, CollisionData* collision_data) const {
	bool collided = false;
	float min_distance = FLT_MAX;

	for(int i = 0; i < FACES; i++) {
		CollisionData cur_data;
		bool cur_collided = faces[i].lineCollision(origin, direction, &cur_data);
		if (cur_collided && (cur_data.distance < min_distance)) {
			collided = true;
			min_distance = cur_data.distance;

			if (collision_data) {
				collision_data->collision_point = cur_data.collision_point;
				collision_data->normal = cur_data.normal;
				collision_data->distance = cur_data.distance;
				collision_data->material_handle = cur_data.material_handle;
			}
		}
	}

	return collided;
}

std::istream& operator>>(std::istream& in, RecPrism& prism) {
	std::string material_name;
	Vec3 pos, w, h, d;
	in >> pos >> w >> h >> d >> material_name;
	int handle = MaterialHandler::getHandle(material_name);
	prism = RecPrism(pos, w, h, d, handle);
	return in;
}