
#include "scene.hpp"

#include "debug.hpp"
#include "material_handler.hpp"
#include "texture.hpp"
#include "texture_handler.hpp"
#include "polygon_model.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdexcept>

void Scene::addModel( Model* model )
{
	models.push_back( model );
}

void Scene::addLight( Light* light )
{
	lights.push_back( light );
}

void Scene::setCamera( const vector_t& position, const vector_t& at, const vector_t& up, scalar_t fov )
{
	camera_position = position;

	camera_at = at - position;
	camera_at.normalize();

	camera_right = vector_t::cross_product( camera_at, up );
	camera_right.normalize();

	camera_up = vector_t::cross_product( camera_right, camera_at );
	camera_up.normalize();

	this->fov = fov;
}

bool Scene::loadScene( const fs::path& filename )
{
	bool ok = false;
	std::ifstream fin( filename.string().c_str() );
	if ( fin.is_open() )
	{
		while ( !fin.fail() )
		{
			std::string str;
			fin >> str;
			if ( str.size() > 0 )
			{
				if ( str == "model" )
				{
					Model* model;
					fin >> model;
					addModel( model );
				}
				else if ( str == "polygonmodel" )
				{
					PolygonModel* model = new PolygonModel();
					vector_t pos;
					std::string obj_filename;
					fin >> pos >> obj_filename;
					if ( model->loadObjectFile( filename.parent_path() / obj_filename ) )
					{
						model->position = pos;
						addModel( model );
					}
					else
					{
						std::cout << "could not load polygonmodel\n";
						delete model;
						break;
					}
				}
				else if ( str == "mtllib" )
				{
					std::string mat_filename;
					fin >> mat_filename;
					bool loaded_mat = MaterialManager::loadMaterialFile( filename.parent_path() / mat_filename );
					if ( !loaded_mat )
					{
						ok = false;
						break;
					}
				}
				else if ( str == "light" )
				{
					vector_t pos;
					colour_t colour;
					fin >> pos >> colour;
					Light* light = new Light( pos, colour );
					addLight( light );
				}
				else if ( str == "camera" )
				{
					vector_t pos, at, up;
					scalar_t fov;
					fin >> pos >> at >> up >> fov;
					setCamera( pos, at, up, fov );
				}
				else if ( str == "attenuation" )
				{
					fin >> attenuation;
				}
				else if ( str == "far" )
				{
					fin >> far_distance;
				}
				else
				{
					if ( str[0] != '#' )
					{
						std::cout << "unknown token: " << str << '\n';
					}
					getline( fin, str );
				}
			}
		}

		ok = fin.eof();
		if ( !ok )
		{
			std::cout << "failed to load " << filename << '\n';
		}
	}
	else
	{
		std::cout << "cannot open " << filename << '\n';
	}

	return ok;
}

void Scene::render( int width, int height, RGBPixel* buffer )
{
	std::cout << "rendering\n";

	for ( int y = 0; y < height; y++ )
	{
		for ( int x = 0; x < width; x++ )
		{
			renderPixel( width, height, buffer, x, y );
		}
	}

	std::cout << "finished\n";
}

void Scene::renderPixel( int width, int height, RGBPixel* buffer, int xpos, int ypos )
{
	scalar_t x = xpos;
	scalar_t y = ypos;
	scalar_t x_angle = ( x / ( scalar_t )( width - 1 ) - 0.5 ) * fov / 2 * PI / 180;
	scalar_t y_fov = fov * (scalar_t)height / (scalar_t)width;
	scalar_t y_angle = ( 0.5 - y / ( scalar_t )( height - 1 ) ) * y_fov / 2 * PI / 180;

	vector_t direction = camera_at
		+ std::tan( (double)x_angle ) * camera_right
		+ std::tan( (double)y_angle ) * camera_up;

	direction.normalize();
	vector_t colour = castRay( camera_position, direction );

	uint8_t red = std::min( ( int )( colour.x * 255.0 ), 255 );
	uint8_t green = std::min( ( int )( colour.y * 255.0 ), 255 );
	uint8_t blue = std::min( ( int )( colour.z * 255.0 ), 255 );

	buffer[ xpos + ypos * width ] = RGBPixel( red, green, blue );
}

vector_t Scene::attenuate( const vector_t& colour, scalar_t distance )
{
	return colour / ( 1 + distance * distance / attenuation );
}

vector_t Scene::castRay( const vector_t& origin, const vector_t& raw_direction, scalar_t total_distance, scalar_t refraction_index )
{
	vector_t colour = vector_t( 0, 0, 0 ); // black

	if ( total_distance >= far_distance )
		return colour;

	auto direction = raw_direction;
	direction.normalize();

	CollisionData collision_data;
	collision_data.distance = std::numeric_limits<scalar_t>::max();
	Model* model = NULL;

	/*
	find the model the collides with the ray first
	*/
	for ( Model* cur_model : models )
	{
		CollisionData cur_data;
		cur_data.distance = std::numeric_limits<scalar_t>::max();

		if ( cur_model == NULL )
		{
			throw std::runtime_error( "cur model is NULL" );
		}

		bool collided = cur_model->lineCollision( origin, direction,
						&cur_data );

		if ( collided && ( cur_data.distance < collision_data.distance ) )
		{
			collision_data = cur_data;
			model = cur_model;
		}
	}


	if ( model == NULL || ( total_distance + collision_data.distance ) > far_distance )
		return colour;
	
	total_distance += collision_data.distance;

	const Material& material = MaterialManager::getMaterial( collision_data.material_handle );

	vector_t diffuse = material.diffuse;
	vector_t ambient = material.ambient;
	vector_t specular = material.specular;
	scalar_t alpha = material.alpha;

	/*
	get texture colours if applicable
	*/
	if ( material.ambient_tex_handle >= 0 )
	{
		const Texture& tex = TextureHandler::getTexture( material.ambient_tex_handle );
		math::vec4 tex_colour = tex.sampleColour( collision_data.tex_x, collision_data.tex_y, Texture::NEAREST );
		ambient = vector_t( tex_colour.x, tex_colour.y, tex_colour.z );
	}

	if ( material.diffuse_tex_handle >= 0 )
	{
		const Texture& tex = TextureHandler::getTexture( material.diffuse_tex_handle );
		math::vec4 tex_colour = tex.sampleColour( collision_data.tex_x, collision_data.tex_y, Texture::NEAREST );
		diffuse = vector_t( tex_colour.x, tex_colour.y, tex_colour.z );
		alpha *= tex_colour.w;
	}

	if ( material.specular_tex_handle >= 0 )
	{
		const Texture& tex = TextureHandler::getTexture( material.specular_tex_handle );
		math::vec4 tex_colour = tex.sampleColour( collision_data.tex_x, collision_data.tex_y, Texture::NEAREST );
		specular = vector_t( tex_colour.x, tex_colour.y, tex_colour.z );
	}

	if ( vector_t::dot_product( collision_data.normal, direction ) < 0 )
	{
		/*
		add illumination from lights
		*/
		if ( alpha > 0 )
		{
			for ( int l = 0; l < ( int )lights.size(); l++ )
			{
				Light* light = lights[ l ];
				vector_t light_colour = light->colour;
				vector_t light_dir = light->position - collision_data.collision_point;
				scalar_t light_distance = light_dir.length();
				light_dir /= light_distance;

				/*
				ambient
				*/
				colour += alpha * colour_t::multiply( light->colour, ambient );

				bool can_see_light = true;

				for ( Model* cur_model : models )
				{
					CollisionData light_collision;
					bool collided = cur_model->lineCollision( collision_data.collision_point
									+ light_dir / 1000, light_dir, &light_collision );

					if ( collided && ( light_collision.distance < light_distance ) )
					{
						const Material& mat = MaterialManager::getMaterial( light_collision.material_handle );
						if ( mat.alpha == 1 )
						{
							can_see_light = false;
							break;
						}
						else if ( mat.alpha > 0 )
						{
							light_colour = ( 1 - mat.alpha ) * colour_t::multiply( light_colour, mat.transmission_filter );
						}
					}
				}

				if ( can_see_light )
				{
					vector_t proj = vector_t::project( light_dir, collision_data.normal );
					vector_t specular_dir = light_dir - 2 * proj;

					vector_t attenuated_light = attenuate( light_colour, light_distance );

					scalar_t diffuse_dot = vector_t::dot_product( light_dir, collision_data.normal );
					if ( diffuse_dot > 0 )
					{
						colour += alpha * diffuse_dot * colour_t::multiply( attenuated_light, diffuse );
					}

					scalar_t specular_dot = vector_t::dot_product( specular_dir, direction );
					if ( specular_dot > 0 )
					{
						colour += alpha * std::pow( (double)specular_dot, (double)material.specular_exponent )
								  * colour_t::multiply( attenuated_light, specular );
					}
				}
			}


			/*
			add illumination from reflections off other models
			*/
			vector_t proj = vector_t::project( direction, collision_data.normal );
			vector_t reflection = vector_t::normalize( direction - 2 * proj );

			vector_t reflect_colour = castRay( collision_data.collision_point + reflection / 1000,
											   reflection, total_distance, refraction_index );

			colour += alpha * colour_t::multiply( reflect_colour, specular );

			/*
			// do we need diffuse reflected light?
			colour += vector_t::dot_product(reflection, collision_data.normal)
			    * reflect_colour
			    * material.diffuse;
			*/
		}// alpha > 0
	}

	if ( alpha < 1 )
	{
		/*
		add illumination through model
		*/
		scalar_t dot = vector_t::dot_product( direction, collision_data.normal );
		if ( dot != 0 )
		{
			bool going_in = dot < 0;

			scalar_t from_index = going_in ? scalar_t( 1 ) : material.refraction_index;
			scalar_t to_index = going_in ? material.refraction_index : scalar_t( 1 );

			vector_t refracted = vector_t::refract( direction, collision_data.normal,
													from_index, to_index );

			vector_t through_colour = castRay( collision_data.collision_point
											   + refracted / 1000, refracted, total_distance, to_index );

			colour += ( 1 - alpha ) * colour_t::multiply( material.transmission_filter, through_colour );
		}
	}

	colour = attenuate( colour, collision_data.distance );


	return colour;
}
