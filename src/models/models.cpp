#include <istream>
#include <iostream>
#include "models.hpp"

std::istream& operator>>(std::istream& in, Model*& model) {
	std::string type;
	model = NULL;

	in >> type;

	if (type == "sphere") {
		Sphere* sphere = new Sphere();
		in >> *sphere;
		model = sphere;
	} else if (type == "plane") {
		Plane* plane = new Plane();
		in >> *plane;
		model = plane;
	} else if (type == "rectangle") {
		Rectangle* rec = new Rectangle();
		in >> *rec;
		model = rec;
	} else if (type == "triangle") {
		Triangle* tri = new Triangle();
		in >> *tri;
		model = tri;
	} else if (type == "recprism") {
		RecPrism* prism = new RecPrism();
		in >> *prism;
		model = prism;
	}

	return in;
}