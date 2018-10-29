#include <cfloat>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "scene.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

Scene::Scene() {
	camera_at = Vec3(0, 0, -1);
	camera_up = Vec3(0, 1, 0);
	camera_right = Vec3(1, 0, 0);
	fov = 90;
	far_distance = 10000;
	rec_depth = 0;
}

void Scene::addModel(Model* model) {
	models.push_back(model);
}

void Scene::addLight(Light* light) {
	lights.push_back(light);
}

void Scene::setCamera(Vec3 position, Vec3 at, Vec3 up, float fov) {
	camera_position = position;

	camera_at = at - position;
	camera_at.normalize();

	camera_right = Vec3::crossProduct(camera_at, up);
	camera_right.normalize();

	camera_up = Vec3::crossProduct(camera_right, camera_at);
	camera_up.normalize();

	this->fov = fov;
}

void Scene::render(int width, int height, Pixel* buffer) {
	std::cout << "rendering\n";
	const int tell = 100000;
	
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int index = x + y*width;
			if ((index % tell == 0) && (index > 0)) {
				std::cout << 100.0 * index / (width * height) << "%\n";
			}
			
			float x_angle = ((float)x/(width-1) - 0.5) * fov/2 * M_PI/180;
			float y_fov = fov * height / width;
			float y_angle = (0.5 - (float)y/(height-1)) * y_fov/2 * M_PI/180;

			Vec3 direction = camera_at
				+ std::tan(x_angle) * camera_right
				+ std::tan(y_angle) * camera_up;

			Vec3 colour = castRay(camera_position, direction.normalize());
			uchar red = std::min(colour.x, 1.0f) * 255;
			uchar green = std::min(colour.y, 1.0f) * 255;
			uchar blue = std::min(colour.z, 1.0f) * 255;
			buffer[x + (height-1-y) * width] = Pixel(red, green, blue);
		}
	}
	
	std::cout << "finished\n";
}

#define dout(message) std::cout << std::string(rec_depth, ' ') << message << '\n'

// #define dout(message)

Vec3 Scene::castRay(Vec3 origin, Vec3 direction, float total_distance) {
	Vec3 colour = Vec3(0, 0, 0); // black
	
	if (total_distance < far_distance) {
		direction.normalize();
		
		float min_distance = FLT_MAX;
		Vec3 collision_point;
		Vec3 normal;
		Model* model = NULL;
		
		/*
		find the model the collides with the ray first
		*/
		for(int i = 0; i < (int)models.size(); i++) {
			Vec3 cur_collision_point;
			float cur_distance = FLT_MAX;
			Vec3 cur_normal;
			Model* cur_model = models[i];

			bool collided = cur_model->lineCollision(origin, direction,
				&cur_collision_point, &cur_normal, &cur_distance);

			if (collided && (cur_distance < min_distance)) {
				min_distance = cur_distance;
				collision_point = cur_collision_point;
				normal = cur_normal;
				model = cur_model;
			}
		}


		if (model != NULL && (total_distance + min_distance) <= far_distance) {
			total_distance += min_distance;
			normal.normalize();

			const Material& material = model->material;
			
			/*
			add illumination from lights
			*/
			for(int l = 0; l < (int)lights.size(); l++) {
				Light* light = lights[l];
				
				Vec3 light_dir = light->position - collision_point;
				float light_dist = light_dir.length();
				light_dir /= light_dist;
				
				colour += material.ka * light->colour * material.ambient;
				
				bool can_see_light = true;
				
				for(int m = 0; m < (int)models.size(); m++) {
					Model* cur_model = models[m];
				
					bool collided = cur_model->lineCollision(collision_point + light_dir/1000, light_dir, NULL, NULL, NULL);
				
					if (collided) {
						can_see_light = false;
						break;
					}
				}
			
				if (can_see_light) {
					Vec3 proj = Vec3::project(light_dir, normal);
					Vec3 specular_dir = light_dir - 2 * proj;
					
					float diffuse_dot = Vec3::dotProduct(light_dir, normal);
					if (diffuse_dot > 0) {
						colour += material.kd * diffuse_dot * light->colour
							* material.diffuse;// / (1 + light_dist*0.01);
					}

					float specular_dot = Vec3::dotProduct(specular_dir, direction);
					if (specular_dot > 0) {
						colour += material.ks * std::pow(specular_dot, material.smoothness)
							* light->colour * material.specular;// / (1 + light_dist*0.01);
					}
				}
			}

			
			/*
			add illumination from reflections off other models
			*/
			Vec3 proj = Vec3::project(direction, normal);
			Vec3 reflection = Vec3::normalize(direction - 2 * proj);
			
			Vec3 reflect_colour = castRay(collision_point + reflection/1000, reflection, total_distance);
			
			colour += material.ks * reflect_colour * material.specular;
			colour += material.kd * Vec3::dotProduct(reflection, normal) * reflect_colour * material.diffuse;

			/*
			add illumination from light on other side of model
			*/
			/*
			if (transparent_weight > 0) {
				float dot = Vec3::dotProduct(direction, normal);
				if (dot != 0) {
					float from_index = (dot < 0) ? refraction_index : (model->refraction_index);
					float to_index = (dot < 0) ? (model->refraction_index) : refraction_index;
					
					Vec3 refracted = Vec3::refract(direction, normal, from_index, to_index);
					Vec3 through_colour = castRay(collision_point + refracted/1000, refracted, total_distance, to_index);
					colour += transparent_weight * through_colour;
				}
			}
			*/
			
			// attenuation
			// colour /= (1.0 + min_distance * 0.01);
		}
	}

	return colour;
}
