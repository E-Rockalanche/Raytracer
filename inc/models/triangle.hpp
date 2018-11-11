#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "plane.hpp"

class Triangle : public Plane {
public:
	Triangle() {}

	Triangle(Vec3 v1, Vec3 v2, Vec3 v3, int material_handle);

	virtual bool lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const;

	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 v1, Vec3 v2,
		Vec3 v3, float& t);

	friend std::istream& operator>>(std::istream& in, Triangle& triangle);
};

#endif