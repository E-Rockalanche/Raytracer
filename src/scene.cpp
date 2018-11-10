#include <cfloat>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <cmath>

#include "vec4.hpp"
#include "scene.hpp"
#include "material_handler.hpp"
#include "texture.hpp"
#include "texture_handler.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

Scene::Scene() {
	camera_at = Vec3(0, 0, -1);
	camera_up = Vec3(0, 1, 0);
	camera_right = Vec3(1, 0, 0);
	fov = 90;
	rec_depth = 0;
	attenuation = 100;
	far_distance = 100;
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
			if (str.size() > 0) {
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
				} else if (str == "mtllib") {
					std::string mat_filename;
					fin >> mat_filename;
					bool loaded_mat = MaterialHandler::loadMaterialFile(mat_filename, path);
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
				} else if (str == "attenuation") {
					fin >> attenuation;
				} else if (str == "far") {
					fin >> far_distance;
				} else {
					if (str[0] != '#') {
						std::cout << "unknown token: " << str << '\n';
					}
					getline(fin, str);
				}
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

			uchar red = std::min((int)(colour.x * 255.0), 255);
			uchar green = std::min((int)(colour.y * 255.0), 255);
			uchar blue = std::min((int)(colour.z * 255.0), 255);

			buffer[x + (height-1-y) * width] = Pixel(red, green, blue);
		}
	}
	
	std::cout << "finished\n";
}

Vec3 Scene::attenuate(const Vec3& colour, float distance) {
	return colour / (1 + distance*distance/attenuation);
}

Vec3 Scene::castRay(Vec3 origin, Vec3 direction, float total_distance, float refraction_index) {
	Vec3 colour = Vec3(0, 0, 0); // black
	
	if (total_distance < far_distance) {
		direction.normalize();

		CollisionData collision_data;
		collision_data.distance = FLT_MAX;
		Model* model = NULL;
		
		/*
		find the model the collides with the ray first
		*/
		for(int i = 0; i < (int)models.size(); i++) {
			CollisionData cur_data;
			cur_data.distance = FLT_MAX;

			Model* cur_model = models[i];

			if (cur_model == NULL) {
				throw std::runtime_error("cur model is NULL");
			}

			bool collided = cur_model->lineCollision(origin, direction,
				&cur_data);

			if (collided && (cur_data.distance < collision_data.distance)) {
				collision_data = cur_data;
				model = cur_model;
			}
		}


		if (model != NULL && (total_distance + collision_data.distance) <= far_distance) {
			total_distance += collision_data.distance;

			const Material& material = MaterialHandler::getMaterial(collision_data.material_handle);

			Vec3 diffuse = material.diffuse;
			Vec3 ambient = material.ambient;
			Vec3 specular = material.specular;
			float alpha = material.alpha;

			/*
			get texture colours if applicable
			*/
			if (material.ambient_tex_handle >= 0) {
				const Texture& tex = TextureHandler::getTexture(material.ambient_tex_handle);
				Vec4 tex_colour = tex.sampleColour(collision_data.tex_x,
					collision_data.tex_y, Texture::NEAREST);
				ambient *= Vec3(tex_colour.x, tex_colour.y, tex_colour.z);
			}

			if (material.diffuse_tex_handle >= 0) {
				const Texture& tex = TextureHandler::getTexture(material.diffuse_tex_handle);
				Vec4 tex_colour = tex.sampleColour(collision_data.tex_x,
					collision_data.tex_y, Texture::NEAREST);
				diffuse *= Vec3(tex_colour.x, tex_colour.y, tex_colour.z);
				alpha *= tex_colour.w;
			}

			if (material.specular_tex_handle >= 0) {
				const Texture& tex = TextureHandler::getTexture(material.specular_tex_handle);
				Vec4 tex_colour = tex.sampleColour(collision_data.tex_x,
					collision_data.tex_y, Texture::NEAREST);
				specular *= Vec3(tex_colour.x, tex_colour.y, tex_colour.z);
			}
			
			if (Vec3::dotProduct(collision_data.normal, direction) < 0) {
				/*
				add illumination from lights
				*/
				if (alpha > 0) {
					for(int l = 0; l < (int)lights.size(); l++) {
						Light* light = lights[l];
						Vec3 light_colour = light->colour;
						Vec3 light_dir = light->position - collision_data.collision_point;
						float light_distance = light_dir.length();
						light_dir /= light_distance;

						/*
						ambient
						*/
						colour += alpha * light->colour * ambient;
						
						bool can_see_light = true;
						
						for(int m = 0; m < (int)models.size(); m++) {
							Model* cur_model = models[m];
							CollisionData light_collision;
							bool collided = cur_model->lineCollision(collision_data.collision_point
								+ light_dir/1000, light_dir, &light_collision);
						
							if (collided && (light_collision.distance < light_distance)) {
								const Material& mat = MaterialHandler::getMaterial(light_collision.material_handle);
								if (mat.alpha == 1) {
									can_see_light = false;
									break;
								} else if (mat.alpha > 0) {
									light_colour *= (1 - mat.alpha) * mat.transmission_filter;
								}
							}
						}
					
						if (can_see_light) {
							Vec3 proj = Vec3::project(light_dir, collision_data.normal);
							Vec3 specular_dir = light_dir - 2 * proj;

							Vec3 attenuated_light = attenuate(light_colour, light_distance);
							
							float diffuse_dot = Vec3::dotProduct(light_dir, collision_data.normal);
							if (diffuse_dot > 0) {
								colour += alpha * diffuse_dot * attenuated_light * diffuse;
							}

							float specular_dot = Vec3::dotProduct(specular_dir, direction);
							if (specular_dot > 0) {
								colour += alpha * std::pow(specular_dot, material.specular_exponent)
									* attenuated_light * specular;
							}
						}
					}

				
					/*
					add illumination from reflections off other models
					*/
					Vec3 proj = Vec3::project(direction, collision_data.normal);
					Vec3 reflection = Vec3::normalize(direction - 2 * proj);
					
					Vec3 reflect_colour = castRay(collision_data.collision_point + reflection/1000,
						reflection, total_distance, refraction_index);
					
					colour += alpha * reflect_colour * specular;
					
					/*
					// do we need diffuse reflected light?
					colour += Vec3::dotProduct(reflection, collision_data.normal)
						* reflect_colour
						* material.diffuse;
					*/
				}// alpha > 0
			}

			if (alpha < 1) {
				/*
				add illumination through model
				*/
				float dot = Vec3::dotProduct(direction, collision_data.normal);
				if (dot != 0) {
					bool going_in = dot < 0;

					float from_index = going_in ? 1.0 : material.refraction_index;
					float to_index = going_in ? material.refraction_index : 1.0;

					Vec3 refracted = Vec3::refract(direction, collision_data.normal,
						from_index, to_index);

					Vec3 through_colour = castRay(collision_data.collision_point
						+ refracted/1000, refracted, total_distance, to_index);

					colour += (1 - alpha) * material.transmission_filter * through_colour;
				}
			}
			
			colour = attenuate(colour, collision_data.distance);
		}
	}

	return colour;
}
