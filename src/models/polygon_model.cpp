
#include <cfloat>
#include <fstream>
#include <string>
#include <iostream>
#include "models.hpp"
#include "material_handler.hpp"
#include "path.hpp"

bool PolygonModel::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point,
		Vec3* normal, int* material_handle, float* distance) const {
	bool collided = false;
	float min_distance = FLT_MAX;

	origin -= position;

	for(int g = 0; g < (int)groups.size(); g++) {
		const PolygonGroup& group = groups[g];

		if (group.bounds.lineCollision(origin, direction)) {
			int size = group.vertex_indices.size();
			size -= size%3;
			for(int f = 0; f < size-2; f += 3) {
				Vec3 v1, v2, v3;
				v1 = vertices[group.vertex_indices[f + 0]];
				v2 = vertices[group.vertex_indices[f + 1]];
				v3 = vertices[group.vertex_indices[f + 2]];

				float t;
				if (Triangle::lineIntersection(origin, direction, v1, v2, v3, t)) {
					if (t < min_distance) {
						min_distance = t;
						assignPointer(collision_point, origin + t * direction);
						assignPointer(normal, Vec3::crossProduct(v2 - v1, v3 - v1));
						assignPointer(distance, t);
						assignPointer(material_handle, group.material_handle);
						collided = true;
					}
				}
			}
		}
	}

	return collided;
}

bool PolygonModel::loadObjectFile(std::string filename, std::string path) {
	std::cout << "loading obj file: " << filename << '\n';

	std::string file, subpath;
	parsePath(filename, subpath, file);
	path += subpath;
	filename = file;

	std::cout << "filename: " << filename << ", path: " << path << '\n';

	bool ok = false;

	vertices.clear();
	normals.clear();
	tex_coords.clear();
	groups.clear();
	groups.resize(1);

	std::ifstream fin((path + filename).c_str());
	if (fin.is_open()) {

		PolygonModel::PolygonGroup* cur_group = &groups[0];

		Vec3 maximums = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);
		Vec3 minimums = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);

		while(!fin.fail()) {
			std::string str;

			fin >> str;
			if (str.size() > 0) {
				if (str == "mtllib") {
					std::string material_filename;
					fin >> material_filename;
					bool loaded = MaterialHandler::loadMaterialFile(path + material_filename);
					if (!loaded) {
						break;
					}
				} else if (str == "g") {
					cur_group->bounds = RecPrism(minimums, Vec3(maximums.x - minimums.x, 0, 0),
						Vec3(0, maximums.y - minimums.y, 0),
						Vec3(0, 0, maximums.z - minimums.z), 0);

					std::string group_name;
					fin >> group_name;
					groups.push_back(PolygonGroup());
					cur_group = &groups.back();

					maximums = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);
					minimums = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
				} else if (str == "usemtl") {
					std::string material_name;
					fin >> material_name;
					cur_group->material_handle = MaterialHandler::getHandle(material_name);
				} else if (str == "v") {
					Vec3 v;
					fin >> v;
					for(int i = 0; i < 3; i++) {
						if (v[i] < minimums[i]) {
							minimums[i] = v[i];
						}
						if (v[i] > maximums[i]) {
							maximums[i] = v[i];
						}
					}
					vertices.push_back(v);
				} else if (str == "vn") {
					Vec3 n;
					fin >> n;
					normals.push_back(n);
				} else if (str == "vt") {
					float u, v;
					fin >> u >> v;
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
							switch(t) {
								case 0: // vertex
									cur_group->vertex_indices.push_back(index);
									break;

								case 1: // tex coord
									cur_group->tex_coord_indices.push_back(index);
									break;

								case 2: // normal
									cur_group->normal_indices.push_back(index);
									break;
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