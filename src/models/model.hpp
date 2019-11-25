#ifndef MODEL_HPP
#define MODEL_HPP

#include "material.hpp"
#include "math_defines.hpp"

#include <iostream>
#include <vector>

struct CollisionData
{
	vector_t collision_point{ 0 };
	vector_t normal{ 0 };
	int material_handle = -1;
	scalar_t tex_x = 0;
	scalar_t tex_y = 0;
	scalar_t distance = 0;
};

class Model
{
public:
	Model() = default;

	Model(const vector_t& position, int material_handle) : position(position),
		material_handle(material_handle) {}

	virtual ~Model() = default;

	virtual bool lineCollision(const vector_t& origin, const vector_t& direction,
		CollisionData* collision_data) const = 0;

	friend std::istream& operator>>(std::istream& in, Model*& model);

	vector_t position{ 0 };
	int material_handle = -1;
};

#endif
