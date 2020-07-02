#ifndef _NEXUS_SPEED_TREE_RENDERER_H
#define _NEXUS_SPEED_TREE_RENDERER_H
#include "d3d_view_info.h"
#include "drawing_policy.h"

namespace nexus
{
	enum ESptMeshType
	{
		ESpt_Branch,
		ESpt_Frond,
		ESpt_Leafcard,
		ESpt_Leafmesh,
	};

	class nrender_mesh_lod_adapter
		: public nrender_mesh
	{
	public:
		nrender_mesh*	m_mesh;
		int				m_lod;

		nrender_mesh_lod_adapter(void):m_mesh(0),m_lod(0)
		{}

		virtual nvertex_factory* get_vertex_factory()
		{
			return m_mesh->get_vertex_factory();
		}
		virtual size_t get_num_section()	{	return 1;}	
		virtual nrender_mesh_section* get_section(size_t section_index)
		{
			(void)section_index;
			return m_mesh->get_section(m_lod);
		}
	};

	struct nspt_render_proxy
		: public nrender_proxy
	{
		const nspt_instance*	tree;		
		transparent_flag		trans_flag;
		nmaterial_base*			mtl;
		nrender_mesh			*mesh;
		mutable nrender_mesh_lod_adapter 
								mesh_adapter;
		enum ESptMeshType		mesh_type;
		int						leaf_index;		

		nspt_render_proxy():tree(NULL),mtl(NULL),mesh(NULL)
		{
			trans_flag.set(ETrans_Opaque);
			leaf_index = 0;
		}

		virtual const matrix44& get_world_matrix() const
		{		return tree->mat_world;		}

		virtual int get_render_lod() const
		{		return 0;	}

		virtual enum EDepthGroup get_depth_group() const
		{		return EDG_World;	}

		virtual nrender_mesh* get_render_mesh(int lod) const
		{		
			if( mesh )
				return mesh;
			else
				return &mesh_adapter;
		}

		virtual nmaterial_base* get_material(int lod, int mtl_id) const
		{
			return mtl;
		}

		virtual transparent_flag get_transparent_flag() const	{ return trans_flag;}
		virtual const box_sphere_bounds& get_bounds() const	{	return tree->bounds;}

		virtual bool accept_dynamic_light() const	{	return true;}
		virtual bool cast_dynamic_shadow() const	{	return true;}

		virtual hit_id get_hit_proxy_id() const		{	return tree->m_hit_id;}

		virtual void draw_setup_effect(nshading_effect* effect_ptr) const;
	};

	class nspeed_tree_renderer
	{
	public:
		nspeed_tree_renderer(void);
		virtual ~nspeed_tree_renderer(void);

		void clear();
		void attach_tree(const nspt_instance* tree);
		void draw_all_trees(const d3d_view_info* view, drawing_policy* dp);

	private:
		void draw_leaf(int index, const nspt_instance* spt, const d3d_view_info* view, drawing_policy* dp);
	private:
		nspt_render_proxy	m_render_obj;

		typedef std::vector<const nspt_instance*> st_tree_array;
		typedef std::map<nname, st_tree_array> st_tree_map;//base tree name <-> tree instance array
		st_tree_map	m_tree_map;

		boost::timer m_timer;
		double		m_last_time;
	};
}//namespace nexus
#endif //_NEXUS_SPEED_TREE_RENDERER_H