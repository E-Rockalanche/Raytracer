#include <iostream>
#include "triangle.hpp"
#include "material_handler.hpp"
#include "math_defines.hpp"

Triangle::Triangle( const vector_t& v1, const vector_t& v2, const vector_t& v3, int material_handle )
	: Plane( v1, v2 - v1, v3 - v1, material_handle ) {}

// static
bool Triangle::lineIntersection( const vector_t& origin, const vector_t& direction, const vector_t& v1, const vector_t& v2,
								 const vector_t& v3, scalar_t& t )
{
	bool intersected = false;
	vector_t normal = vector_t::cross_product( v2 - v1, v3 - v1 );
	if ( vector_t::dot_product( direction, normal ) < 0 )
	{
		if ( Plane::lineIntersection( origin, direction, v1, normal, t ) )
		{
			vector_t point = origin + t * direction;

			// check if inside triangle
			intersected = true;
			vector_t vertices[3] = { v1, v2, v3 };
			for ( int i = 0; i < 3; i++ )
			{
				int j = ( i + 1 ) % 3;
				vector_t side_dir = vector_t::cross_product( normal, vertices[j] - vertices[i] );
				vector_t point_dir = point - vertices[i];
				if ( vector_t::dot_product( side_dir, point_dir ) < 0.0 )
				{
					intersected = false;
					break;
				}
			}
		}
	}
	return intersected;
}

bool Triangle::lineCollision( const vector_t& origin, const vector_t& direction, CollisionData* collision_data ) const
{
	bool collided = false;
	scalar_t t;
	if ( Triangle::lineIntersection( origin, direction, position, position + w, position + h, t ) )
	{
		collided = true;

		if ( collision_data )
		{
			collision_data->collision_point = origin + t * direction;
			collision_data->normal = normal;
			collision_data->distance = t;
			collision_data->material_handle = material_handle;
		}
	}
	return collided;
}

std::istream& operator>>( std::istream& in, Triangle& tri )
{
	std::string material_name;
	vector_t v1, v2, v3;
	in >> v1 >> v2 >> v3 >> material_name;

	int handle = MaterialManager::getHandle( material_name );
	tri = Triangle( v1, v2, v3, handle );

	return in;
}