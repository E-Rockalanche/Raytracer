
#include "material.hpp"

std::ostream& operator<<( std::ostream& out, const Material& m )
{
	out << "Ka "<< m.ambient << '\n';
	out << "Kd "<< m.diffuse << '\n';
	out << "Ks "<< m.specular << '\n';
	out << "Tf "<< m.transmission_filter << '\n';

	out << "Ns "<< m.specular_exponent << '\n';

	if ( !m.ambient_map_filename.empty() )
	{
		out << "map_Ka "<< m.ambient_map_filename << '\n';
	}

	if ( !m.diffuse_map_filename.empty() )
	{
		out << "map_Kd "<< m.diffuse_map_filename << '\n';
	}

	if ( !m.specular_map_filename.empty() )
	{
		out << "map_Ks "<< m.specular_map_filename << '\n';
	}

	out << "d "<< m.alpha << '\n';
	out << "Ni "<< m.refraction_index << '\n';

	return out;
}