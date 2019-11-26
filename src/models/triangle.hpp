#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "plane.hpp"
#include "math_defines.hpp"

class Triangle : public Plane {
public:
	Triangle() = default;

	Triangle( const vector_t& v1, const vector_t& v2, const vector_t& v3, int material_handle );

	virtual bool lineCollision( const vector_t& origin, const vector_t& direction,
		CollisionData* collision_data) const;

	static bool lineIntersection( const vector_t& origin, const vector_t& direction, const vector_t& v1, const vector_t& v2, const vector_t& v3, scalar_t& t);

	friend std::istream& operator>>(std::istream& in, Triangle& triangle);
};

#endif