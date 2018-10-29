#ifndef MODELS_HPP
#define MODELS_HPP

#include <istream>
#include <vector>
#include "vec3.hpp"
#include "material.hpp"

#define assignPointer(variable, value) { if ((variable) != NULL) *(variable) = (value); }

class Model {
public:
	Model() {}
	Model(Vec3 position, Material material) : position(position), material(material) {}
	virtual ~Model() {}
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) = 0;

	friend std::istream& operator>>(std::istream& in, Model*& model);

	Vec3 position;
	Material material;
};

class Sphere : public Model {
public:
	Sphere() {}
	Sphere(Vec3 position, float radius, Material material);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);

	float radius;
};

class Plane : public Model {
public:
	Plane() {}
	Plane(Vec3 position, Vec3 w, Vec3 h, Material material);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);

	Vec3 normal, w, h;
	float constant; // ax + by + cz = d
};

class Triangle : public Plane {
public:
	Triangle() {}
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Material material);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
};

class Rectangle : public Plane {
public:
	Rectangle() {}
	Rectangle(Vec3 position, Vec3 w, Vec3 h, Material material);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
};

class RecPrism : public Model {
public:
	RecPrism() {}
	RecPrism(Vec3 position, Vec3 w, Vec3 h, Vec3 d, Material material);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);

	static const int FACES = 6;
	Rectangle faces[FACES];
};

class PolygonGroup : public Model {
public:
	PolygonGroup() {}
	PolygonGroup(Vec3 position, std::vector<Vec3> vertices, std::vector<int> indices, Material material);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);

	std::vector<Vec3> vertices;
	std::vector<int> indices;
};

#endif
