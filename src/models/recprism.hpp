#ifndef RECPRISM_HPP
#define RECPRISM_HPP

#include "rectangle.hpp"

class RecPrism : public Model
{
public:
	RecPrism() = default;

	RecPrism( const vector_t& position, const vector_t& w, const vector_t& h, const vector_t& d, int material_handle );

	virtual bool lineCollision( const vector_t& origin, const vector_t& direction,
								CollisionData* collision_data ) const;

	friend std::istream& operator>>( std::istream& in, RecPrism& recprism );

	static const int FACES = 6;
	Rectangle faces[ FACES ];
};

#endif