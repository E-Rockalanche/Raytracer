
#include "polygon_model.hpp"
#include "triangle.hpp"
#include "material_handler.hpp"
#include "math_defines.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <limits>

bool PolygonModel::lineCollision( const vector_t& origin_, const vector_t& direction, CollisionData* collision_data ) const
{
	bool collided = false;
	scalar_t min_distance = std::numeric_limits<scalar_t>::max();

	auto origin = origin_ - position;

	vector_t vecs[3];
	int face_index = -1;
	const PolygonGroup* hit_group = NULL;

	for ( int g = 0; g < ( int )groups.size(); g++ )
	{

		const PolygonGroup& group = groups[g];

		if ( group.bounds.lineCollision( origin, direction, NULL ) )
		{
			int size = group.vertex_indices.size();
			for ( int f = 0; f + 2 < size; f += 3 )
			{
				vector_t v1, v2, v3;
				v1 = vertices[group.vertex_indices[f + 0]];
				v2 = vertices[group.vertex_indices[f + 1]];
				v3 = vertices[group.vertex_indices[f + 2]];

				scalar_t t;
				if ( Triangle::lineIntersection( origin, direction, v1, v2, v3, t ) )
				{
					if ( t < min_distance )
					{
						collided = true;

						min_distance = t;
						vecs[0] = v1;
						vecs[1] = v2;
						vecs[2] = v3;
						face_index = f;
						hit_group = &group;

						if ( collision_data )
						{
							collision_data->distance = t;
							collision_data->material_handle = group.material_handle;
						}
					}
				}
			}
		}
	}


	if ( collided && collision_data )
	{
		vector_t local_point = origin + collision_data->distance * direction;
		collision_data->collision_point = local_point + position;
		collision_data->normal = vector_t( 0, 0, 0 );
		collision_data->tex_x = 0;
		collision_data->tex_y = 0;

		vector_t tri_normal = vector_t::cross_product( vecs[1] - vecs[0], vecs[2] - vecs[0] );

		for ( int i = 0; i < 3; i++ )
		{
			int j = ( i + 1 ) % 3;
			int k = ( i + 2 ) % 3;

			vector_t side_dir = vector_t::cross_product( tri_normal, vecs[k] - vecs[j] );
			vector_t total_vec = vector_t::project( vecs[i] - vecs[j], side_dir );
			vector_t cur_vec = local_point - vecs[j];
			scalar_t weight = vector_t::dot_product( cur_vec, total_vec ) / vector_t::dot_product( total_vec, total_vec );

			int normal_index = hit_group->normal_indices[face_index + i];
			collision_data->normal += weight * normals[normal_index];

			int tex_coord_index = hit_group->tex_coord_indices[face_index + i] * 2;
			collision_data->tex_x += weight * tex_coords[tex_coord_index];
			collision_data->tex_y += weight * tex_coords[tex_coord_index + 1];
		}
	}

	return collided;
}

bool PolygonModel::loadObjectFile( const fs::path& filename )
{
	std::cout << "loading obj file: " << filename << '\n';

	bool ok = false;

	vertices.clear();
	normals.clear();
	tex_coords.clear();
	groups.clear();
	groups.resize( 1 );

	std::ifstream fin( filename.string().c_str() );
	if ( fin.is_open() )
	{

		PolygonModel::PolygonGroup* cur_group = &groups[0];

		vector_t maximums = vector_t( std::numeric_limits<typename vector_t::value_type>::min() );
		vector_t minimums = vector_t( std::numeric_limits<typename vector_t::value_type>::max() );

		while ( !fin.fail() )
		{
			std::string str;

			fin >> str;
			if ( str.size() > 0 )
			{
				if ( str == "mtllib" )
				{
					std::string material_filename;
					fin >> material_filename;
					bool loaded = MaterialManager::loadMaterialFile( filename.parent_path() / material_filename );
					if ( !loaded )
					{
						break;
					}
				}
				else if ( str == "g" )
				{
					// set boundins box of previous poly group
					cur_group->bounds = RecPrism( minimums, vector_t( maximums.x - minimums.x, 0, 0 ),
												  vector_t( 0, maximums.y - minimums.y, 0 ),
												  vector_t( 0, 0, maximums.z - minimums.z ), 0 );

					// create new poly group
					std::string group_name;
					fin >> group_name;
					groups.push_back( PolygonGroup() );
					cur_group = &groups.back();

					// reset bounds
					maximums = vector_t( std::numeric_limits<typename vector_t::value_type>::min() );
					minimums = vector_t( std::numeric_limits<typename vector_t::value_type>::max() );
				}
				else if ( str == "usemtl" )
				{
					std::string material_name;
					fin >> material_name;
					cur_group->material_handle = MaterialManager::getHandle( material_name );
				}
				else if ( str == "v" )
				{
					vector_t v;
					fin >> v;
					vertices.push_back( v );
				}
				else if ( str == "vn" )
				{
					vector_t n;
					fin >> n;
					normals.push_back( n );
				}
				else if ( str == "vt" )
				{
					scalar_t u, v;
					fin >> u >> v;
					tex_coords.push_back( u );
					tex_coords.push_back( v );
				}
				else if ( str == "f" )
				{
					for ( int v = 0; v < 3; v++ )
					{
						for ( int t = 0; t < 3; t++ )
						{
							if ( t > 0 )
							{
								char c = fin.peek();
								if ( c == '/' )
								{
									fin.get();
								}
								else
								{
									break;
								}
							}
							int index;
							fin >> index;
							index--;
							if ( t == 0 )
							{
								// vertex
								cur_group->vertex_indices.push_back( index );
								const vector_t& vec = vertices[index];
								for ( int i = 0; i < 3; i++ )
								{
									if ( vec[i] < minimums[i] )
									{
										minimums[i] = vec[i];
									}
									if ( vec[i] > maximums[i] )
									{
										maximums[i] = vec[i];
									}
								}
							}
							else if ( t == 1 )
							{
								// tex coord
								cur_group->tex_coord_indices.push_back( index );
							}
							else if ( t == 2 )
							{
								// normal
								cur_group->normal_indices.push_back( index );
							}
						}
					}
				}
				else
				{
					std::getline( fin, str );
				}
			}
		}

		ok = fin.eof();
		if ( !ok )
		{
			std::cout << "failed to load " << filename << '\n';
		}
		else
		{
			cur_group->bounds = RecPrism( minimums, vector_t( maximums.x - minimums.x, 0, 0 ),
										  vector_t( 0, maximums.y - minimums.y, 0 ),
										  vector_t( 0, 0, maximums.z - minimums.z ), 0 );
		}
		fin.close();
	}
	else
	{
		std::cout << "cannot open " << filename << '\n';
	}

	return ok;
}
