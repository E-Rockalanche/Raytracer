#include "models.hpp"
#include <istream>
#include <iostream>

std::istream& operator>>(std::istream& in, Model*& model) {
	std::string type;

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
	} else if (type == "polygongroup") {
		PolygonGroup* group = new PolygonGroup();
		in >> *group;
		model = group;
	} else {
		model = NULL;
	}

	return in;
}