#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "plane.hpp"
#include "math_defines.hpp"

class Rectangle : public Plane
{
public:
	Rectangle() = default;

	Rectangle( const vector_t& position, const vector_t& w, const vector_t& h, int material_handle );

	void setDimensions( const vector_t& w, const vector_t& h );

	virtual bool lineCollision( const vector_t& origin, const vector_t& direction,
								CollisionData* collision_data ) const;

	static bool lineIntersection( const vector_t& origin, const vector_t& direction, const vector_t& position,
								  const vector_t& w, const vector_t& h, scalar_t& t, scalar_t& wt, scalar_t& ht );

	friend std::istream& operator>>( std::istream& in, Rectangle& rectangle );
};

#endif