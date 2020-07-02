/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_WAVEFRONT_OBJ_IMPORTER_H_
#define _NEXUS_WAVEFRONT_OBJ_IMPORTER_H_
#include "../nengine/engine_define.h"
#include "../nengine/mesh/nmesh_importer.h"
#include "../nengine/renderer/render_define.h"
#include "static_mesh_base.h"

namespace nexus
{
	/**
	*	导入“.obj”格式的静态模型
	*/
	class wavefront_obj_importer :
		public nstatic_mesh_importer,
		public static_mesh_base
	{
	public:
		wavefront_obj_importer(void);
		virtual ~wavefront_obj_importer(void);

		virtual nstring get_file_type() const	{	return nstring(_T(".obj"));}
		virtual nstring get_desc() const		{	return nstring(_T("Wavefront OBJ"));}
		virtual nstatic_mesh_lod::ptr import_from_file(const nstring& full_path);
		virtual void clear()
		{
			m_vertex_buffer.clear();
			m_traits2index.clear();
			m_material_lib.clear();
		}

	private:		
		std::map<vertex_traits, size_t>	m_traits2index;
		std::vector<nstring>			m_material_lib;

		int get_material_index(const nstring& mtl_name);
		unsigned short add_vertex(const vertex_traits& traits, const vertex& vt);

		void add_mesh_section(nstatic_mesh_lod::ptr lod, nmesh_section::ptr sec);		
		
		nDECLARE_CLASS(wavefront_obj_importer)
	};

}//namespace nexus

#endif //_NEXUS_WAVEFRONT_OBJ_IMPORTER_H_