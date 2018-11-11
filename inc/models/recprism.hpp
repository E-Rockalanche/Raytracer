#ifndef RECPRISM_HPP
#define RECPRISM_HPP

#include "rectangle.hpp"

class RecPrism : public Model {
public:
	RecPrism() {}

	RecPrism(Vec3 position, Vec3 w, Vec3 h, Vec3 d, int material_handle);

	virtual bool lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const;

	friend std::istream& operator>>(std::istream& in, RecPrism& recprism);

	static const int FACES = 6;
	Rectangle faces[FACES];
};

#endif