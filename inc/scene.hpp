#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "models.hpp"
#include "light.hpp"
#include "pixel.hpp"
#include "vec3.hpp"

class Scene {
public:
	Scene();
	void addModel(Model* model);
	void setCamera(Vec3 position, Vec3 at, Vec3 up, float fov = 90);
	void addLight(Light* light);
	bool loadScene(std::string filename, std::string path);
	void render(int width, int height, Pixel* buffer);

	Vec3 camera_position;
	Vec3 camera_at;
	Vec3 camera_up;
	Vec3 camera_right;
	float fov;
	float far_distance;
	float attenuation;

	std::vector<Model*> models;
	std::vector<Light*> lights;
	
private:
	int rec_depth;
	Vec3 attenuate(const Vec3& light, float distance);
	Vec3 castRay(Vec3 origin, Vec3 direction, float total_distance = 0.0, float refraction_index = 1.0);
};

#endif