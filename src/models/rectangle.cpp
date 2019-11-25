
#include "rectangle.hpp"
#include "material_handler.hpp"

#include <iostream>

Rectangle::Rectangle( const vector_t& position, const vector_t& w, const vector_t& h, int material_handle )
	: Plane( position, w, h, material_handle )
{
	setDimensions( w, h );
}

void Rectangle::setDimensions( const vector_t& w, const vector_t& h )
{
	this->h = h - vector_t::project( h, w );
	this->w = w - vector_t::project( w, h );
}

bool Rectangle::lineIntersection( const vector_t& origin, const vector_t& direction, const vector_t& position,
								  const vector_t& w, const vector_t& h, scalar_t& t, scalar_t& wt, scalar_t& ht )
{
	bool intersection = false;
	vector_t normal = vector_t::normalize( vector_t::cross_product( w, h ) );
	if ( vector_t::dot_product( direction, normal ) < 0 )
	{
		if ( Plane::lineIntersection( origin, direction, position, normal, t ) )
		{
			vector_t point = origin + t * direction;

			// check within width and height
			wt = vector_t::dot_product( point - position, w ) / vector_t::dot_product( w, w );

			if ( wt > -0.00001 && wt < 1.00001 )
			{
				ht = vector_t::dot_product( point - position, h ) / vector_t::dot_product( h, h );

				intersection = ( ht > -0.00001 && ht < 1.00001 );
			}
		}
	}
	return intersection;
}

bool Rectangle::lineCollision( const vector_t& origin, const vector_t& direction, CollisionData* collision_data ) const
{
	bool collided = false;
	scalar_t t, wt, ht;
	if ( Rectangle::lineIntersection( origin, direction, position, w, h, t, wt, ht ) )
	{
		collided = true;

		if ( collision_data )
		{
			collision_data->collision_point = origin + t * direction;
			collision_data->normal = normal;
			collision_data->distance = t;
			collision_data->material_handle = material_handle;
			collision_data->tex_x = wt;
			collision_data->tex_y = ht;
		}
	}
	return collided;
}

std::istream& operator>>( std::istream& in, Rectangle& rec )
{
	std::string material_name;
	vector_t pos, w, h;
	in >> pos >> w >> h >> material_name;
	int handle = MaterialManager::getHandle( material_name );
	rec = Rectangle( pos, w, h, handle );
	return in;
}
