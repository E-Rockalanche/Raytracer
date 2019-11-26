#ifndef POLYGON_MODEL_HPP
#define POLYGON_MODEL_HPP

#include "model.hpp"
#include "recprism.hpp"
#include "path.hpp"

class PolygonModel : public Model
{
public:
	PolygonModel() = default;

	virtual bool lineCollision( const vector_t& origin, const vector_t& direction,
								CollisionData* collision_data ) const;

	bool loadObjectFile( const fs::path& filename );

	struct PolygonGroup
	{
		std::vector<int> vertex_indices;
		std::vector<int> normal_indices;
		std::vector<int> tex_coord_indices;
		std::string material_name;
		std::string group_name;
		int material_handle = -1;

		RecPrism bounds;

		PolygonGroup() = default;
	};

	std::vector<vector_t> vertices;
	std::vector<vector_t> normals;
	std::vector<scalar_t> tex_coords;

	std::vector<PolygonGroup> groups;
};

#endif