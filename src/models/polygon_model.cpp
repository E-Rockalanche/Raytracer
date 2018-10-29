#include "models.hpp"
#include "material_handler.hpp"
#include <cfloat>
#include <fstream>
#include <string>
#include <iostream>

bool PolygonModel::lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) {
	bool collided = false;
	float min_distance = FLT_MAX;

	origin -= position;

	for(int g = 0; g < groups; g++) {
		const PolygonGroup& group = groups[g];

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
					collided = true;
				}
			}
		}
	}

	return collided;
}

bool PolygonModel::loadObjectFile(std::string filename) {
	bool ok = false;

	std::ifstream fin(filename.c_str());
	if (fin.is_open()) {
		model.vertices.clear();
		model.normals.clear();
		model.tex_coords.clear();
		model.groups.clear();
		model.groups.resize(1);

		PolygonModel::PolygonGroup* cur_group = model.groups[0];

		while(!in.fail()) {
			std::string str;
			in >> str;
			if (str.size() > 0) {
				if (str == "mtllib") {
					std::string material_filename;
					in >> material_filename;
					MaterialHandler::loadMaterial(material_filename);
				} else if (str == "g") {
					std::string group_name;
					in >> group_name;
					model.groups.push_back(PolygonGroup());
					cur_group = &model.groups.back();
				} else if (str == "usemtl") {
					std::string material_name;
					in >> material_name;
					cur_group->material_handle = MaterialHandler::getHandle(material_name);
				} else if (str == "v") {
					Vec3 v;
					in >> v;
					model.vertices.push_back(v);
				} else if (str == "vn") {
					Vec3 n;
					in >> n;
					model.normals.push_back(n);
				} else if (str == "vt") {
					float u, v;
					in >> u >> v;
					model.tex_cords.push_back(u);
					model.tex_cords.push_back(v);
				} else if (str == "f") {
					for(int v = 0; v < 3; v++) {
						for(int t = 0; t < 3; t++) {
							if (t > 0) {
								char c = in.peek();
								if (c == '/') {
									in.get();
								} else {
									break;
								}
							}
							int index;
							in >> index;
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
					std::getline(in, str);
				}
			}
		}

		ok = fin.eof();
		if (!ok) {
			std::cout << "failed to load " << filename << '\n';
		}
		fin.close();
	} else {
		std::cout << "cannot open " << filename << '\n';
	}

	return ok;
}