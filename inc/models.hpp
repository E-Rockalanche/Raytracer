#ifndef MODELS_HPP
#define MODELS_HPP

#include <istream>
#include <vector>
#include "vec3.hpp"
#include "material.hpp"

#define assignPointer(variable, value) { if ((variable) != NULL) *(variable) = (value); }

class Model {
public:
	Model() { material_handle = 0; }
	Model(Vec3 position, int material_handle) : position(position), material_handle(material_handle) {}
	virtual ~Model() {}
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance) = 0;

	friend std::istream& operator>>(std::istream& in, Model*& model);

	Vec3 position;
	int material_handle;
};

class Sphere : public Model {
public:
	Sphere() {}
	Sphere(Vec3 position, float radius, int material_handle);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 position, float radius, float& t);
	friend std::istream& operator>>(std::istream& in, Sphere& sphere);

	float radius;
};

class Plane : public Model {
public:
	Plane() {}
	Plane(Vec3 position, Vec3 w, Vec3 h, int material_handle);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 position, Vec3 normal, float* t);
	void init();
	friend std::istream& operator>>(std::istream& in, Plane& plane);

	Vec3 normal, w, h;
	float constant; // ax + by + cz = d
};

class Triangle : public Plane {
public:
	Triangle() {}
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3, int material_handle);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 v1, Vec3 v2, Vec3 v3, float* t);
	friend std::istream& operator>>(std::istream& in, Triangle triangle);
};

class Rectangle : public Plane {
public:
	Rectangle() {}
	Rectangle(Vec3 position, Vec3 w, Vec3 h, int material_handle);
	setDimensions(Vec3 w, Vec3 h);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 position, Vec3 w, Vec3 h, float* t);
	friend std::istream& operator>>(std::istream& in, Rectangle& rectangle);
};

class RecPrism : public Model {
public:
	RecPrism() {}
	RecPrism(Vec3 position, Vec3 w, Vec3 h, Vec3 d, int material_handle);
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
	friend std::istream& operator>>(std::istream& in, RecPrism& recprism);

	static const int FACES = 6;
	Rectangle faces[FACES];
};

class PolygonModel : public Model {
public:
	PolygonModel() {}
	virtual bool lineCollision(Vec3 origin, Vec3 direction, Vec3* collision_point, Vec3* normal, float* distance);
	friend std::istream& operator>>(std::istream& in, PolygonModel& group);

	struct PolygonGroup {
		std::vector<int> vertex_indices;
		std::vector<int> normal_indices;
		std::vector<int> tex_coord_indices;
		std::string material_name;
		std::string group_name;
		int material_handle;

		PolygonGroup() {}
	};

	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<float> tex_coords;

	std::vector<PolygonGroup> groups;
};

#endif
