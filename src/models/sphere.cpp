
#include "sphere.hpp"
#include "material_handler.hpp"
#include "math_defines.hpp"

#define dout(message)

Sphere::Sphere( const vector_t& position, scalar_t radius, int material_handle )
	: Model( position, material_handle ), radius( radius ) {}

bool Sphere::lineIntersection( const vector_t& origin, const vector_t& direction, const vector_t& position,
							   scalar_t radius, scalar_t& t )
{
	bool intersected = false;
	vector_t dist = origin - position;
	scalar_t a = vector_t::dot_product( direction, direction );
	scalar_t b = vector_t::dot_product( dist, direction ) * 2;
	scalar_t c = vector_t::dot_product( dist, dist ) - radius * radius;

	if ( a != 0.0 )
	{
		scalar_t det = b * b - 4.0 * a * c;
		if ( det >= 0.0 )
		{
			scalar_t sqrt_det = std::sqrt( (double)det );
			t = ( -b - sqrt_det ) / ( 2.0 * a );

			if ( t >= 0 )
			{
				intersected = true;
			}
			else
			{
				t = ( -b + sqrt_det ) / ( 2.0 * a );
				if ( t >= 0 )
				{
					intersected = true;
				}
			}
		}
	}

	return intersected;
}

bool Sphere::lineCollision( const vector_t& origin, const vector_t& direction,
							CollisionData* collision_data ) const
{
	bool collided = false;
	scalar_t t;
	if ( Sphere::lineIntersection( origin, direction, position, radius, t ) )
	{
		collided = true;

		if ( collision_data )
		{
			collision_data->distance = t;
			collision_data->collision_point = origin + t * direction;
			collision_data->normal = ( collision_data->collision_point - position )
									 / radius;
			collision_data->material_handle = material_handle;

			const Material& mat = MaterialManager::getMaterial( material_handle );
			if ( mat.diffuse_tex_handle >= 0 )
			{
				collision_data->tex_y = std::acos( (double)vector_t::dot_product( collision_data->normal,
												   vector_t( 0, -1, 0 ) ) ) / PI;
				scalar_t x_dot = vector_t::dot_product( collision_data->normal, vector_t( 1, 0, 0 ) );
				scalar_t z_dot = vector_t::dot_product( collision_data->normal, vector_t( 0, 0, 1 ) );
				collision_data->tex_x = ( z_dot >= 0 ) ? ( 1 - ( std::acos( (double)x_dot ) / ( 2 * PI ) ) )
										: ( std::acos( (double)x_dot ) / ( 2 * PI ) );
			}
		}
	}
	return collided;
}

std::istream& operator>>( std::istream& in, Sphere& sphere )
{
	std::string material_name;
	vector_t pos;
	scalar_t radius;
	in >> pos >> radius >> material_name;
	int handle = MaterialManager::getHandle( material_name );
	sphere = Sphere( pos, radius, handle );
	return in;
}