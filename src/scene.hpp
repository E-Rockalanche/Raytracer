#ifndef SCENE_HPP
#define SCENE_HPP

#include "math_defines.hpp"
#include "model.hpp"
#include "light.hpp"
#include "path.hpp"
#include "pixel.hpp"

#include <vector>

class Scene
{
public:
	Scene() = default;
	void addModel( Model* model );
	void setCamera( const vector_t& position, const vector_t& at, const vector_t& up, scalar_t fov = 90 );
	void addLight( Light* light );
	bool loadScene( const fs::path& filename );
	void render( int width, int height, RGBPixel* buffer );
	void renderArea( int width, int height, RGBPixel* buffer, int xpos, int ypos, int areaWidth, int areaHeight );
	void renderPixel( int width, int height, RGBPixel* buffer, int x, int y );

	vector_t camera_position{ 0, 0, 0 };
	vector_t camera_at{ 0, 0, -1 };
	vector_t camera_up{ 0, 1, 0 };
	vector_t camera_right{ 1, 0, 0 };

	scalar_t fov = 90;
	scalar_t attenuation = 100;
	scalar_t far_distance = 100;

	std::vector<Model*> models;
	std::vector<Light*> lights;

private:
	int rec_depth = 0;
	vector_t attenuate( const vector_t& light, scalar_t distance );
	vector_t castRay( const vector_t& origin, const vector_t& direction, scalar_t total_distance = 0.0, scalar_t refraction_index = 1.0 );
};

#endif