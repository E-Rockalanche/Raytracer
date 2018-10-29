#include "models.hpp"
#include <istream>

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
		Vec3 w, h;
		in >> w >> h;
		model = new Triangle(position, w, h, mat);
	} else {
		model = NULL;
	}

	return in;
}