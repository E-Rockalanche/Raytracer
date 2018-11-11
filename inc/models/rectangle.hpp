#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "plane.hpp"

class Rectangle : public Plane {
public:
	Rectangle() {}

	Rectangle(Vec3 position, Vec3 w, Vec3 h, int material_handle);

	void setDimensions(Vec3 w, Vec3 h);

	virtual bool lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const;

	static bool lineIntersection(Vec3 origin, Vec3 direction, Vec3 position,
		Vec3 w, Vec3 h, float& t, float& wt, float& ht);

	friend std::istream& operator>>(std::istream& in, Rectangle& rectangle);
};

#endif