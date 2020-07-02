/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRESOURCE_STATIC_MESH_H_
#define _NRESOURCE_STATIC_MESH_H_

#include "nresource.h"
#include "../mesh/mesh_vertex_data_common.h"
#include "../mesh/mesh_lod.h"
#include "../mesh/simple_mesh.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../material/nmaterial.h"
#include "../renderer/nrender_proxy.h"
#include "../renderer/nrender_simple_mesh.h"

namespace nexus
{
	/**
	 *	静态模型资源对象
	*/
	class nAPI nresource_static_mesh :
		public nresource
	{
	public:
		typedef boost::intrusive_ptr<nresource_static_mesh> ptr;

		explicit nresource_static_mesh(const nstring& name_str);
		virtual ~nresource_static_mesh(void);

		virtual bool ready() const
		{
			// todo
			return true;
		}

		void import_lod(int lod, const nstring& full_path);
		void import_set_material(int lod, int mtl_id, nmaterial_base::ptr mtl_ptr);
		void generate_simple_mesh();

		const box_sphere_bounds& get_bounds() const		{	return m_bounding_box; }
		transparent_flag get_transparent_flag() const	{	return m_trans_flag;}
		size_t get_num_lod() const	{	return m_render_mesh_array.size(); }
		size_t get_num_section(size_t i) const;
		nrender_mesh* get_render_mesh(int lod) const;
		nrender_simple_mesh* get_render_simple_mesh() const	{	return m_render_simple_mesh.get(); }
		nmaterial_base* get_material(int lod, int mtl_id) const;		
		
		virtual void serialize(narchive& ar);

		static size_t get_num_importer();
		static nstring get_importer_type(size_t i);
		static nstring get_importer_desc(size_t i);
	private:
		box_sphere_bounds	m_bounding_box;
		transparent_flag	m_trans_flag;
		vector<nstatic_mesh_lod::ptr>					m_lod_array;
		nsimple_mesh::ptr								m_simple_mesh;

		render_res_array<nrender_static_mesh_indexed>	m_render_mesh_array;		
		render_res_ptr<nrender_simple_mesh>				m_render_simple_mesh;

		nDECLARE_NAMED_CLASS(nresource_static_mesh)
	};
}//namespace nexus

#endif //_NRESOURCE_STATIC_MESH_H_