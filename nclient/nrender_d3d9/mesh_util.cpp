#include "StdAfx.h"
#include "mesh_util.h"

namespace nexus
{
	void _create_mesh_section_array(const vector<nmesh_section::ptr>& nsec_array, vector<d3d9_mesh_section::ptr>& d3d_array, D3DPOOL pool)
	{
		try
		{
			for(size_t i=0; i<nsec_array.size(); i++)
			{
				nmesh_section* mesh_sec = nsec_array[i].get();
				d3d9_mesh_section::ptr new_sec(nNew d3d9_mesh_section);
				new_sec->create(mesh_sec, pool);

				d3d_array.push_back(new_sec);				
			}
		}
		catch(nexception& e)
		{
			(void)e;

			//-- for exception safe
			d3d_array.clear();

			//-- “Ï≥£÷–¡¢
			throw;
		}
	}
}//namespace nexus