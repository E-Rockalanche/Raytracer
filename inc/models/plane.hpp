#ifndef PLANE_HPP
#define PLANE_HPP

#include "model.hpp"

class Plane : public Model {
public:
	Plane() {}

	Plane(Vec3 position, Vec3 w, Vec3 h, int material_handle);
	
	void init();

	virtual bool lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const;

	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 position,
		Vec3 normal, float& t);

	friend std::istream& operator>>(std::istream& in, Plane& plane);

	Vec3 normal, w, h;
	float constant; // ax + by + cz = d
};

#endif