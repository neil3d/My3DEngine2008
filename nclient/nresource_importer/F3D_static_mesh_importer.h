#pragma once
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../nengine/renderer/render_define.h"
#include "static_mesh_base.h"

namespace nexus
{
	class F3D_static_mesh_importer :
		public nstatic_mesh_importer,
		public static_mesh_base
	{
	public:
		F3D_static_mesh_importer(void);
		virtual ~F3D_static_mesh_importer(void);

		virtual nstring get_file_type() const	{	return nstring(_T(".fsm"));}
		virtual nstring get_desc() const		{	return nstring(_T("F3D Static Mesh"));}
		virtual nstatic_mesh_lod::ptr import_from_file(const nstring& full_path);
		virtual void clear()
		{
			m_vertex_buffer.clear();
		}

		nDECLARE_CLASS(F3D_static_mesh_importer)
	};
}//nexus