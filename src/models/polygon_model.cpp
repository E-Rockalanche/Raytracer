
#include <cfloat>
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include "models.hpp"
#include "material_handler.hpp"
#include "path.hpp"

void assert(bool condition, std::string message) {
	if (!condition) {
		throw std::runtime_error(message);
	}
}

bool PolygonModel::lineCollision(Vec3 origin, Vec3 direction, CollisionData* collision_data) const {

	bool collided = false;
	float min_distance = FLT_MAX;

	origin -= position;

	Vec3 vecs[3];
	int face_index = -1;
	const PolygonGroup* hit_group = NULL;

	for(int g = 0; g < (int)groups.size(); g++) {

		const PolygonGroup& group = groups[g];

		if (!group.use_bounds || group.bounds.lineCollision(origin, direction, NULL)) {
			for(int f = 0; (f+2) < (int)group.vertex_indices.size(); f += 3) {
				Vec3 v1, v2, v3;
				v1 = vertices[group.vertex_indices[f + 0]];
				v2 = vertices[group.vertex_indices[f + 1]];
				v3 = vertices[group.vertex_indices[f + 2]];

				float t;
				if (Triangle::lineIntersection(origin, direction, v1, v2, v3, t)) {
					if (t < min_distance) {
						collided = true;

						min_distance = t;
						vecs[0] = v1;
						vecs[1] = v2;
						vecs[2] = v3;
						face_index = f;
						hit_group = &group;
						
						if (collision_data) {
							collision_data->distance = t;
							collision_data->material_handle = group.material_handle;
						}
					}
				}
			}
		}
	}

	if (collided && collision_data) {
		collision_data->collision_point = origin + position + collision_data->distance * direction;
		collision_data->normal = Vec3(0, 0, 0);
		collision_data->tex_x = 0;
		collision_data->tex_y = 0;

		Vec3 tri_normal = Vec3::crossProduct(vecs[1] - vecs[0], vecs[2] - vecs[0]);

		for(int i = 0; i < 3; i++) {
			int j = (i+1)%3;
			int k = (i+2)%3;
			Vec3 side_dir = Vec3::crossProduct(tri_normal, vecs[k] - vecs[j]);
			Vec3 total_vec = Vec3::project(vecs[i] - vecs[j], side_dir);
			Vec3 cur_vec = collision_data->collision_point - vecs[j];
			float weight = Vec3::dotProduct(cur_vec, total_vec) / Vec3::dotProduct(total_vec, total_vec);

			assert(face_index+i < (int)hit_group->normal_indices.size(), "index "
				+std::to_string(face_index+i)+" out of normal indices bounds"
				+std::to_string(hit_group->normal_indices.size()));
			
			int normal_index = hit_group->normal_indices[face_index + i];

			assert(normal_index < (int)normals.size(), "index out of normal bounds");

			collision_data->normal += weight * normals[normal_index];

			assert(face_index + i < (int)hit_group->tex_coord_indices.size(), "index out of tex coord indices bounds");

			int tex_coord_index = hit_group->tex_coord_indices[face_index + i] * 2;

			assert(tex_coord_index+1 < (int)tex_coords.size(), "index out of tex coords bounds");

			collision_data->tex_x += weight * tex_coords[tex_coord_index];
			collision_data->tex_y += weight * tex_coords[tex_coord_index + 1];
		}

		if (Vec3::dotProduct(collision_data->normal, direction) > 0) {
			collided = false;
		}
	}

	return collided;
}

bool PolygonModel::loadObjectFile(std::string filename, std::string path) {
	parsePath(path + filename, path, filename);

	std::cout << "loading obj file: " << filename << '\n';

	bool ok = false;

	vertices.clear();
	normals.clear();
	tex_coords.clear();
	groups.clear();
	groups.resize(1);

	std::ifstream fin((path + filename).c_str());
	if (fin.is_open()) {

		PolygonModel::PolygonGroup* cur_group = &groups[0];
		cur_group->use_bounds = true;
		Vec3 maximums = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);
		Vec3 minimums = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);

		while(!fin.fail()) {
			std::string str;

			fin >> str;
			if (str.size() > 0) {
				if (str == "mtllib") {
					std::string material_filename;
					fin >> material_filename;
					bool loaded = MaterialHandler::loadMaterialFile(material_filename, path);
					if (!loaded) {
						break;
					}
				} else if (str == "g") {
					// set boundins box of previous poly group
					cur_group->bounds = RecPrism(minimums, Vec3(maximums.x - minimums.x, 0, 0),
						Vec3(0, maximums.y - minimums.y, 0),
						Vec3(0, 0, maximums.z - minimums.z), 0);

					// create new poly group
					std::string group_name;
					fin >> group_name;
					groups.push_back(PolygonGroup());
					cur_group = &groups.back();
					cur_group->use_bounds = true;

					// reset bounds
					maximums = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);
					minimums = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
				} else if (str == "usemtl") {
					std::string material_name;
					fin >> material_name;
					cur_group->material_handle = MaterialHandler::getHandle(material_name);
				} else if (str == "v") {
					Vec3 v;
					fin >> v;
					vertices.push_back(v);
				} else if (str == "vn") {
					Vec3 n;
					fin >> n;
					normals.push_back(n);
				} else if (str == "vt") {
					float u, v;
					fin >> u >> v;
					v = 1.0 - v;
					tex_coords.push_back(u);
					tex_coords.push_back(v);
				} else if (str == "f") {
					for(int v = 0; v < 3; v++) {
						for(int t = 0; t < 3; t++) {
							if (t > 0) {
								char c = fin.peek();
								if (c == '/') {
									fin.get();
								} else {
									break;
								}
							}
							int index;
							fin >> index;
							index--;
							if (t == 0) {
								// vertex
								cur_group->vertex_indices.push_back(index);
								const Vec3& vec = vertices[index];
								for(int i = 0; i < 3; i++) {
									if (vec[i] < minimums[i]) {
										minimums[i] = vec[i];
									}
									if (vec[i] > maximums[i]) {
										maximums[i] = vec[i];
									}
								}
							} else if (t == 1) {
								// tex coord
								cur_group->tex_coord_indices.push_back(index);
							} else if (t == 2) {
								// normal
								cur_group->normal_indices.push_back(index);
							}
						}
					}
				} else {
					std::getline(fin, str);
				}
			}
		}

		ok = fin.eof();
		if (!ok) {
			std::cout << "failed to load " << filename << '\n';
		} else {
			cur_group->bounds = RecPrism(minimums, Vec3(maximums.x - minimums.x, 0, 0),
				Vec3(0, maximums.y - minimums.y, 0),
				Vec3(0, 0, maximums.z - minimums.z), 0);
		}
		fin.close();
	} else {
		std::cout << "cannot open " << filename << '\n';
	}

	return ok;
}

int getIndex(int x, int y, int octave) {
	return (x*3 + y*5 + octave*7) % 256;
}

void PolygonModel::generateNoisePatch(Vec3 position, Vec3 w, Vec3 h,
		float divisions, float amplitude, int material_handle) {
	this->position = position;

	int width = divisions + 1;
	int height = divisions + 1;
	int size = width * height;

	vertices.resize(size);
	normals.resize(size);
	tex_coords.resize(size * 2);
	groups.clear();

	Vec3 normal = Vec3::normalize(Vec3::crossProduct(w, h));

	/*
	set vertices
	*/
	float randoms[256];
	for(int i = 0; i < 256; i++) {
		randoms[i] = amplitude * (2.0 * rand() / RAND_MAX - 1.0);
	}


	// debugging
	const char* gradient = " .,:;xX#";
	int gradient_length = 8;

	std::cout << '\n';
	for(int y = height-1; y >= 0; y--) {
		for(int x = 0; x < width; x++) {
			int index = x + y*width;
			float cur_amp = 0;
			for(int i = 1; i <= divisions; i *= 2) {
				int xx = x*i/divisions;
				int yy = y*i/divisions;
				float amp_tl, amp_tr, amp_bl, amp_br;

				amp_tl = randoms[getIndex(xx, yy, i)];
				amp_tr = randoms[getIndex(xx+1, yy, i)];
				amp_bl = randoms[getIndex(xx, yy+1, i)];
				amp_br = randoms[getIndex(xx+1, yy+1, i)];

				float xt = ((float)x * i) / divisions - xx;
				float yt = ((float)y * i) / divisions - yy;

				float amp_top = amp_tl + xt * (amp_tr - amp_tl);
				float amp_bottom = amp_bl + xt * (amp_br - amp_bl);

				cur_amp += (amp_top + yt * (amp_bottom - amp_top)) / i;
			}

			int gradient_index = gradient_length * (cur_amp / (2.0 * amplitude) + 0.5);
			std::cout << gradient[gradient_index] << ' ';

			vertices[index] = (float)x * w/divisions + (float)y * h/divisions + cur_amp * normal;
		}
		std::cout << '\n';
	}

	/*
	set normals
	*/
	const int x_dirs[] = {1, 0, -1, 0};
	const int y_dirs[] = {0, 1, 0, -1};

	#define random() (2.0 * rand()/RAND_MAX - 1.0)

	for(int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Vec3 cur_normal = Vec3::normalize(normal + random() * w/divisions/2.0 + random() * h/divisions/2.0);
			normals[x + y*width] = cur_normal;
			std::cout << "normal: " << cur_normal << '\n';
			/*

			int num_neighbours = 0;

			const Vec3& center = vertices[x + y*width];

			Vec3 vertex_norm;
			for(int d = 0; d < 4; d++) {
				int x1 = x + x_dirs[d];
				int y1 = y + y_dirs[d];
				if (x1 >= 0 && y1 >= 0 && x1 < width && y1 < height) {
					int x2 = x + x_dirs[(d+1)%4];
					int y2 = y + y_dirs[(d+1)%4];
					if (x2 >= 0 && y2 >= 0 && x2 < width && y2 < height) {
						num_neighbours++;
						vertex_norm += Vec3::crossProduct(vertices[x1 + y1*width] - center,
							vertices[x2 + y2*width] - center);
					}
				}
			}

			if (num_neighbours == 0) {
				vertex_norm = normal;
			}

			normals[x + y*width] = Vec3::normalize(vertex_norm / num_neighbours);
			*/
		}
		std::cout << '\n';
	}

	/*
	set indices
	*/
	groups.resize(1, PolygonGroup());
	PolygonGroup& group = groups[0];
	group.material_handle = material_handle;
	for(int x = 0; x < divisions; x++) {
		for(int y = 0; y < divisions; y++) {
			// triangle 1
			group.vertex_indices.push_back(x + y*width);
			group.vertex_indices.push_back(x+1 + (y+1)*width);
			group.vertex_indices.push_back(x + (y+1)*width);

			// triangle 2
			group.vertex_indices.push_back(x + y*width);
			group.vertex_indices.push_back(x+1 + y*width);
			group.vertex_indices.push_back(x+1 + (y+1)*width);
		}
	}

	group.normal_indices = std::vector<int>(group.vertex_indices);
	group.tex_coord_indices = std::vector<int>(group.vertex_indices);

	std::cout << "vertices: " << vertices.size() << '\n';
	std::cout << "normals: " << normals.size() << '\n';
	std::cout << "faces: " << group.vertex_indices.size()/3 << '\n';
}