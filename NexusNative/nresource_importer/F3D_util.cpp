#include "StdAfx.h"
#include "F3D_util.h"

namespace nexus
{
	nmesh_section::ptr convert_f3d_sub_mesh(shared_ptr<F3D::LodSubMesh> fm)
	{		
		nASSERT(!fm->baseFaces.empty());

		nmesh_section::ptr	msec(nNew nmesh_section);
		msec->m_primitive_type = EDraw_TriangleList;		
		msec->m_material_id = fm->mtlID;
		msec->m_base_vertex_index = fm->baseIndex;
		std::vector<WORD>& ib = msec->m_index_buffer.data;
		ib.resize(fm->baseFaces.size()*3);

		//-- Ö»¿½±´lod0£¬ºöÂÔÆäËûlod
		size_t x = 0;
		for(size_t y=0; y<fm->baseFaces.size(); y++)
		{
			const F3D::TriFace& face = fm->baseFaces[y];
			ib[x++] = face.index[2];
			ib[x++] = face.index[1];
			ib[x++] = face.index[0];
		}
		return msec;
	}
}//namespace nexus