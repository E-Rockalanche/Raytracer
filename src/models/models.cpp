#include "models.hpp"
#include <istream>
#include <iostream>

std::istream& operator>>(std::istream& in, Model*& model) {
	std::string type;
	Vec3 position;
	Material mat;

	in >> type >> position >> mat;

	if (type == "sphere") {
		float radius;
		in >> radius;
		model = new Sphere(position, radius, mat);
	} else if (type == "recprism") {
		Vec3 w, h, d;
		in >> w >> h >> d;
		model = new RecPrism(position, w, h, d, mat);
	} else if (type == "rectangle") {
		Vec3 w, h;
		in >> w >> h;
		model = new Rectangle(position, w, h, mat);
	} else if (type == "plane") {
		Vec3 w, h;
		in >> w >> h;
		model = new Plane(position, w, h, mat);
	} else if (type == "triangle") {
		Vec3 v2, v3;
		in >> v2 >> v3;
		model = new Triangle(position, v2, v3, mat);
	} else if (type == "polygongroup") {
		std::string filename;
		in >> filename;
		PolygonGroup* pg = new PolygonGroup();
		bool ok = pg->loadFile(filename.c_str());
		if (!ok) {
			std::cout << "failed to load " << filename << '\n';
			delete pg;
			pg = NULL;
		} else {
			pg->position = position;
			pg->material = mat;
		}
		model = pg;
	} else {
		model = NULL;
	}

	return in;
}