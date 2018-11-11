#ifndef MODEL_HPP
#define MODEL_HPP

#include <istream>
#include <vector>
#include "vec3.hpp"
#include "material.hpp"

struct CollisionData {
	Vec3 collision_point;
	Vec3 normal;
	int material_handle;
	float tex_x, tex_y;
	float distance;
};

class Model {
public:
	Model() { material_handle = 0; }

	Model(Vec3 position, int material_handle) : position(position),
		material_handle(material_handle) {}

	virtual ~Model() {}

	virtual bool lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const = 0;

	friend std::istream& operator>>(std::istream& in, Model*& model);

	Vec3 position;
	int material_handle;
};

#endif
