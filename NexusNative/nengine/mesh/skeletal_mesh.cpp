#include "StdAfx.h"
#include "skeletal_mesh.h"

namespace nexus
{
	nDEFINE_CLASS(nskeletal_mesh, nobject)

	nskeletal_mesh::nskeletal_mesh(void)
	{
	}

	nskeletal_mesh::~nskeletal_mesh(void)
	{
	}

	void nskeletal_mesh::serialize(narchive& ar)
	{
		nSERIALIZE(ar, m_vert_data);
		nSERIALIZE(ar, m_secton_array);
		nSERIALIZE(ar, m_bounding_box);

		vector<resource_location> m_mtl_loc_array;
		if(ar.is_loading())
		{
			nSERIALIZE(ar, m_mtl_loc_array);
			m_mtl_array.resize(m_mtl_loc_array.size());
			for(size_t i=0;i<m_mtl_loc_array.size();++i)
			{
				m_mtl_array[i]=nmtl_base::create_from_file(m_mtl_loc_array[i]);
			}			
		}
		else
		{
			m_mtl_loc_array.resize(m_mtl_array.size());
			for(size_t i=0;i<m_mtl_array.size();++i)
			{
				if(m_mtl_array[i]!=NULL)
				{
					m_mtl_loc_array[i]=resource_location(m_mtl_array[i]->get_name());
				}
			}
			nSERIALIZE(ar, m_mtl_loc_array);
		}
	}
}//namespace nexus