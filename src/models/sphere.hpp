#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "model.hpp"
#include "math_defines.hpp"

class Sphere : public Model {
public:
	Sphere() = default;

	Sphere(const vector_t& position, scalar_t radius, int material_handle);
	
	virtual bool lineCollision(const vector_t& origin, const vector_t& direction,
		CollisionData* collision_data) const;

	static bool lineIntersection(const vector_t& origin, const vector_t& direction, const vector_t& position,
		scalar_t radius, scalar_t& t);

	friend std::istream& operator>>(std::istream& in, Sphere& sphere);

	scalar_t radius = 1;
};

#endif