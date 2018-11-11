#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "model.hpp"

class Sphere : public Model {
public:
	Sphere() {}

	Sphere(Vec3 position, float radius, int material_handle);
	
	virtual bool lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const;

	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 position,
		float radius, float& t);

	friend std::istream& operator>>(std::istream& in, Sphere& sphere);

	float radius;
};

#endif