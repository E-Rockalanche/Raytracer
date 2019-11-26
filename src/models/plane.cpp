
#include "plane.hpp"
#include "material_handler.hpp"

#include <iostream>

Plane::Plane( const vector_t& position, const vector_t& w, const vector_t& h, int material_handle )
	: Model( position, material_handle ), w( w ), h( h )
{
	init();
}

void Plane::init()
{
	normal = vector_t::normalize( vector_t::cross_product( w, h ) );
	constant = vector_t::dot_product( position, normal );
}

// static
bool Plane::lineIntersection( const vector_t& origin, const vector_t& direction, const vector_t& position,
							  const vector_t& normal, scalar_t& t )
{
	bool intersection = false;
	scalar_t dir_dot = vector_t::dot_product( direction, normal );
	if ( dir_dot != 0 )
	{
		t = ( vector_t::dot_product( position - origin, normal ) ) / dir_dot;
		intersection = ( t > 0 );
	}
	return intersection;
}

bool Plane::lineCollision( const vector_t& origin, const vector_t& direction, CollisionData* collision_data ) const
{
	bool collided = false;
	scalar_t t;
	if ( Plane::lineIntersection( origin, direction, position, normal, t ) )
	{
		collided = true;

		if ( collision_data )
		{
			collision_data->collision_point = origin + t * direction;
			collision_data->normal = normal;
			collision_data->distance = t;
			collision_data->material_handle = material_handle;
			collision_data->tex_x = vector_t::dot_product( collision_data->collision_point - position, w )
									/ vector_t::dot_product( w, w );
			collision_data->tex_y = vector_t::dot_product( collision_data->collision_point - position, h )
									/ vector_t::dot_product( h, h );
		}
	}

	return collided;
}

std::istream& operator>>( std::istream& in, Plane& plane )
{
	std::string material_name;
	vector_t pos, w, h;
	in >> pos >> w >> h >> material_name;
	int handle = MaterialManager::getHandle( material_name );
	plane = Plane( pos, w, h, handle );
	return in;
}
