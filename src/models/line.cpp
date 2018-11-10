#include "models.hpp"

Line::Line(Vec3 position, Vec3 v, int material_handle)
	: Model(position, material_handle), v(v) {}

bool Line::closestPoints(Vec3 po, Vec3 p, Vec3 q0,
		Vec3 q, float& t, float& s) {
	/*
	     t       s
	| <P, P> -<Q, P> | <P0-Q0, P> |
	| <P, Q> -<Q, Q> | <P0-Q0, Q> |
	*/

	bool not_parallel = false;

	float p_sqr = Vec3::dotProduct(p, p);
	float pq = Vec3::dotProduct(p, q);
	float q_sqr = Vec3::dotProduct(q, q);
	float p_dot = Vec3::dotProduct(p, po - q0);
	float q_dot = Vec3::dotProduct(q, po - q0);

	float det = pq*pq - p_sqr * q_sqr;
	if (det != 0) {
		not_parallel = true;
		t = (q_dot * qp - p_dot * q_sqr) / det;
		s = (p_sqr * q_dot - pq * p_dot) / det;
	}

	return not_parallel;
}