#include <cfloat>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <fstream>

#include "scene.hpp"
#include "material_handler.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

Scene::Scene() {
	camera_at = Vec3(0, 0, -1);
	camera_up = Vec3(0, 1, 0);
	camera_right = Vec3(1, 0, 0);
	fov = 90;
	far_distance = 1000;
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

bool Scene::loadScene(std::string filename, std::string path) {
	bool ok = false;
	std::ifstream fin((path + filename).c_str());
	if (fin.is_open()) {
		while(!fin.fail()) {
			std::string str;
			fin >> str;
			if (str == "model") {
				Model* model;
				fin >> model;
				addModel(model);
			} else if (str == "polygonmodel") {
				PolygonModel* model = new PolygonModel();
				Vec3 pos;
				std::string obj_filename;
				fin >> pos >> obj_filename;
				if (model->loadObjectFile(obj_filename, path)) {
					model->position = pos;
					addModel(model);
				} else {
					std::cout << "could not load polygonmodel\n";
					delete model;
					break;
				}
			} else if (str == "material") {
				std::string mat_filename;
				fin >> mat_filename;
				bool loaded_mat = MaterialHandler::loadMaterialFile(path + mat_filename);
				if (!loaded_mat) {
					ok = false;
					break;
				}
			} else if (str == "light") {
				Vec3 pos, colour;
				fin >> pos >> colour;
				Light* light = new Light(pos, colour);
				addLight(light);
			} else if (str == "camera") {
				Vec3 pos, at, up;
				float fov;
				fin >> pos >> at >> up >> fov;
				setCamera(pos, at, up, fov);
			} else {
				if (str[0] != '#') {
					std::cout << "unknown token: " << str << '\n';
				}
				getline(fin, str);
			}
		}

		ok = fin.eof();
		if (!ok) {
			std::cout << "failed to load " << filename << '\n';
		}
	} else {
		std::cout << "cannot open " << filename << '\n';
	}

	return ok;
}

void Scene::render(int width, int height, Pixel* buffer) {
	std::cout << "rendering\n";
	
	std::time_t t1 = time(NULL);
	
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			std::time_t t2 = time(NULL);
			if (t2 >= t1 + 5) {
				t1 = t2;
				std::cout << 100.0 * (x + y*width) / (width*height) << "%\n";
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

Vec3 attenuate(Vec3 colour, float distance) {
	return colour / (1 + distance/1000);
}

Vec3 Scene::castRay(Vec3 origin, Vec3 direction, float total_distance) {
	Vec3 colour = Vec3(0, 0, 0); // black
	
	if (total_distance < far_distance) {
		direction.normalize();
		
		float min_distance = FLT_MAX;
		Vec3 collision_point;
		Vec3 normal;
		Model* model = NULL;

		int material_handle;
		
		/*
		find the model the collides with the ray first
		*/
		for(int i = 0; i < (int)models.size(); i++) {
			Vec3 cur_collision_point;
			float cur_distance = FLT_MAX;
			Vec3 cur_normal;
			Model* cur_model = models[i];
			int cur_material_handle;

			bool collided = cur_model->lineCollision(origin, direction,
				&cur_collision_point, &cur_normal, &cur_material_handle, &cur_distance);

			if (collided && (cur_distance < min_distance)) {
				min_distance = cur_distance;
				collision_point = cur_collision_point;
				normal = cur_normal;
				model = cur_model;
				material_handle = cur_material_handle;
			}
		}


		if (model != NULL && (total_distance + min_distance) <= far_distance) {
			total_distance += min_distance;
			normal.normalize();

			const Material& material = MaterialHandler::getMaterial(material_handle);
			
			/*
			add illumination from lights
			*/
			for(int l = 0; l < (int)lights.size(); l++) {
				Light* light = lights[l];
				
				Vec3 light_dir = light->position - collision_point;
				float light_dist = light_dir.length();
				light_dir /= light_dist;
				
				colour += light->colour * material.ambient;
				
				bool can_see_light = true;
				
				for(int m = 0; m < (int)models.size(); m++) {
					Model* cur_model = models[m];
				
					bool collided = cur_model->lineCollision(collision_point
						+ light_dir/1000, light_dir);
				
					if (collided) {
						can_see_light = false;
						break;
					}
				}
			
				if (can_see_light) {
					Vec3 proj = Vec3::project(light_dir, normal);
					Vec3 specular_dir = light_dir - 2 * proj;

					Vec3 attenuated_light = attenuate(light->colour, light_dist);
					
					float diffuse_dot = Vec3::dotProduct(light_dir, normal);
					if (diffuse_dot > 0) {
						colour += diffuse_dot * attenuated_light * material.diffuse;// / (1 + light_dist*0.01);
					}

					float specular_dot = Vec3::dotProduct(specular_dir, direction);
					if (specular_dot > 0) {
						colour += std::pow(specular_dot, material.specular_exponent)
							* attenuated_light * material.specular;// / (1 + light_dist*0.01);
					}
				}
			}

			
			/*
			add illumination from reflections off other models
			*/
			Vec3 proj = Vec3::project(direction, normal);
			Vec3 reflection = Vec3::normalize(direction - 2 * proj);
			
			Vec3 reflect_colour = castRay(collision_point + reflection/1000,
				reflection, total_distance);
			
			colour += reflect_colour * material.specular;
			colour += Vec3::dotProduct(reflection, normal)
				* reflect_colour
				* material.diffuse;

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
			
			colour = attenuate(colour, min_distance);
		}
	}

	return colour;
}
