#include "models.hpp"
#include <cfloat>

PolygonGroup::PolygonGroup(Vec3 position, std::vector<Vec3> vertices, std::vector<int> indices, Material material)
	: Model(position, material), vertices(vertices), indices(indices) {}

bool PolygonGroup::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;
	float min_distance = FLT_MAX;

	int size = indices.size();
	size -= size%3;
	for(int i = 0; i < size; i += 3) {
		Vec3 cur_vertices[3];
		for(int v = 0; v < 3; v++) {
			cur_vertices[v] = vertices[indices[i+v]];
		}
		Vec3 cur_normal = Vec3::normalize(Vec3::crossProduct(cur_vertices[1] - cur_vertices[0],
			cur_vertices[2] - cur_vertices[0]));

		float constant = Vec3::dotProduct(cur_normal, cur_vertices[0]);

		float dir_dot = Vec3::dotProduct(direction, cur_normal);
		if (dir_dot < 0) {
			float t = (constant - Vec3::dotProduct(origin, cur_normal)) / dir_dot;
			if (t > 0) {
				Vec3 point = origin + t * direction;

				// check if inside triangle
				bool inside = true;
				for(int i = 0; i < 3; i++) {
					int j = (i+1)%3;
					Vec3 side_dir = Vec3::crossProduct(cur_normal, cur_vertices[j] - cur_vertices[i]);
					Vec3 point_dir = point - cur_vertices[i];
					if (Vec3::dotProduct(side_dir, point_dir) < 0) {
						inside = false;
						break;
					}
				}

				if (inside && t < min_distance) {
					t = min_distance;
					assignPointer(collision_point, point);
					assignPointer(normal, cur_normal);
					assignPointer(distance, t);
					collided = true;
				}
			}
		}
	}

	return collided;
}