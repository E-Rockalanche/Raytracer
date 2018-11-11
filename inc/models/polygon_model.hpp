#ifndef POLYGON_MODEL_HPP
#define POLYGON_MODEL_HPP

#include "model.hpp"
#include "recprism.hpp"

class PolygonModel : public Model {
public:
	PolygonModel() {}

	virtual bool lineCollision(Vec3 origin, Vec3 direction,
		CollisionData* collision_data) const;

	bool loadObjectFile(std::string filename, std::string path);

	struct PolygonGroup {
		std::vector<int> vertex_indices;
		std::vector<int> normal_indices;
		std::vector<int> tex_coord_indices;
		std::string material_name;
		std::string group_name;
		int material_handle;

		RecPrism bounds;

		PolygonGroup() {}
	};

	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<float> tex_coords;

	std::vector<PolygonGroup> groups;
};

#endif