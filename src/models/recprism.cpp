#include <cfloat>
#include "recprism.hpp"
#include "material_handler.hpp"

RecPrism::RecPrism( const vector_t& position, const vector_t& w, const vector_t& h, const vector_t& d, int material_handle )
	: Model( position, material_handle )
{
	faces[0] = Rectangle( position + d, w, h, material_handle );
	faces[1] = Rectangle( position + w, h, d, material_handle );
	faces[2] = Rectangle( position, h, w, material_handle );
	faces[3] = Rectangle( position, d, h, material_handle );
	faces[4] = Rectangle( position + h, d, w, material_handle );
	faces[5] = Rectangle( position, w, d, material_handle );
}

bool RecPrism::lineCollision( const vector_t& origin, const vector_t& direction, CollisionData* collision_data ) const
{
	bool collided = false;
	scalar_t min_distance = FLT_MAX;

	for ( int i = 0; i < FACES; i++ )
	{
		CollisionData cur_data;
		bool cur_collided = faces[i].lineCollision( origin, direction, &cur_data );
		if ( cur_collided && ( cur_data.distance < min_distance ) )
		{
			collided = true;
			min_distance = cur_data.distance;

			if ( collision_data )
			{
				collision_data->collision_point = cur_data.collision_point;
				collision_data->normal = cur_data.normal;
				collision_data->distance = cur_data.distance;
				collision_data->material_handle = cur_data.material_handle;
			}
		}
	}

	return collided;
}

std::istream& operator>>( std::istream& in, RecPrism& prism )
{
	std::string material_name;
	vector_t pos, w, h, d;
	in >> pos >> w >> h >> d >> material_name;
	int handle = MaterialManager::getHandle( material_name );
	prism = RecPrism( pos, w, h, d, handle );
	return in;
}