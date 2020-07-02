#ifndef _NEXUS_D3D9_RENDER_UTIL_H_
#define _NEXUS_D3D9_RENDER_UTIL_H_
#include "d3d_ptr.h"

namespace nexus
{
	void load_shder_source(const nstring& file_name, std::string& shader_source);

	void d3d_copy_vertex_buffer(d3d_vb_ptr dest_vb, const vertex_stream* src_stream);
	
}//namespace nexus

#endif //_NEXUS_D3D9_RENDER_UTIL_H_