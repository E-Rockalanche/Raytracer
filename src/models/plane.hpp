#ifndef PLANE_HPP
#define PLANE_HPP

#include "model.hpp"

class Plane : public Model
{
public:
	Plane() = default;

	Plane( const vector_t& position, const vector_t& w, const vector_t& h, int material_handle );

	void init();

	virtual bool lineCollision( const vector_t& origin, const vector_t& direction,
								CollisionData* collision_data ) const;

	static bool lineIntersection( const vector_t& origin, const vector_t& direction, const vector_t& position,
								  const vector_t& normal, scalar_t& t );

	friend std::istream& operator>>( std::istream& in, Plane& plane );

	vector_t normal{ 0 };
	vector_t w{ 0 };
	vector_t h{ 0 };
	scalar_t constant = 0; // ax + by + cz = d
};

#endif