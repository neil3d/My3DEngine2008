#include "StdAfx.h"
#include "terrain_water.h"
#include "../framework/nengine.h"
#include "nterrain_actor.h"
#include "../resource/nresource_manager.h"


namespace nexus
{
	nDEFINE_NAMED_CLASS(nterrain_water, nprimitive_component)

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_water_vertex_data
	////////////////////////////////////////////////////////////////////////////
	nstring nterrain_water_vertex_data::get_vertex_type_name() const
	{
		return _T("terrain_water_vertex");
	}

	nstring nterrain_water_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_terrain_water");
	}

	void nterrain_water_vertex_data::init(size_t chunk_num_rows, size_t chunk_num_cols)
	{
		size_t num_vert = chunk_num_rows*chunk_num_cols;

		// alloc buffer
		init_pos_stream(num_vert);

		// fill the vertex buffer with the 'normal' vertices.
		vector3* pos_vert = reinterpret_cast<vector3*>( m_pos_stream.get_data() );
		const float inv_scale_x = 1.0f / (chunk_num_cols-1.0f);
		const float inv_scale_z = 1.0f / (chunk_num_rows-1.0f);

		float s, t;
		for (size_t i = 0; i < chunk_num_rows; i++) 
		{
			t = i * inv_scale_z;
			for (size_t j = 0; j < chunk_num_cols; j++) 
			{
				s = j * inv_scale_x;

				pos_vert->x = s;
				pos_vert->y = 1.0f;
				pos_vert->z = t;
				pos_vert++;
			}
		}// end of for
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_water_render_mesh
	////////////////////////////////////////////////////////////////////////////
	nterrain_water_render_mesh::nterrain_water_render_mesh(void):m_water(NULL)
	{}

	nterrain_water_render_mesh::~nterrain_water_render_mesh(void)
	{}

	void nterrain_water_render_mesh::create(const nrect& rc, nrender_static_mesh_indexed* share_mesh, const index_buffer16& tri_list, nterrain_water* water_ptr)
	{
		m_water = water_ptr;
		m_share_mesh = share_mesh;
		m_section_id = share_mesh->append_section(EDraw_TriangleList, &tri_list, 0);

		nterrain_actor* trn =  water_ptr->get_terrain_actor();

		float trn_w = trn->get_map_width();
		float trn_h = trn->get_map_height();

		m_quad_bias.x = rc.left / trn_w;
		m_quad_bias.y = rc.top / trn_h;
		m_quad_scale.x = rc.get_width() / trn_w;
		m_quad_scale.y = rc.get_height() / trn_h;

		m_texel_size.x = 1.0f/m_quad_scale.x*rc.get_width();
		m_texel_size.y = 1.0f/m_quad_scale.y*rc.get_height();
	}

	void nterrain_water_render_mesh::draw_setup_effect(nshading_effect* effect_ptr)
	{
		effect_ptr->set_float("TRN_WATER_HEIGHT", float(m_water->get_water_height()) );
		effect_ptr->set_vector("TRN_WATER_QUAD_BIAS", m_quad_bias);
		effect_ptr->set_vector("TRN_WATER_QUAD_Scale", m_quad_scale);
		effect_ptr->set_vector("TRN_WATER_TexelSize", m_texel_size);

		effect_ptr->set_texture("TRN_HeightMap", m_water->get_heightmap_tex());
	}

	////////////////////////////////////////////////////////////////////////////
	//	class water_render_proxy
	////////////////////////////////////////////////////////////////////////////
	const matrix44& water_render_proxy::get_world_matrix() const
	{
		return owner->get_world_matrix();
	}
	
	nmaterial_base* water_render_proxy::get_material(int lod, int mtl_id) const	
	{	
		return owner->get_material(lod, mtl_id);
	}
	
	transparent_flag water_render_proxy::get_transparent_flag() const
	{
		return owner->get_transparent_flag(); 
	}

	hit_id water_render_proxy::get_hit_proxy_id() const		
	{	
		return owner->get_owner()->get_name().name_crc;
	}

	////////////////////////////////////////////////////////////////////////////
	//	class water_quad_node
	////////////////////////////////////////////////////////////////////////////
	water_quad_node::water_quad_node(void):is_leaf(false)
	{}

	void water_quad_node::create(nrect rc, nterrain_water* owner, size_t chunk_size)
	{
		zone = rc;

		if( rc.get_width() <= chunk_size
			|| rc.get_height() <= chunk_size )
		{
			//-- 已经切分的足够小，这是一个leaf node			
			is_leaf = true;

			//-- 检测地形高度，为低于水平面的格子建立mesh
			nterrain_actor* trn = owner->get_terrain_actor();
			vector3 trn_scale = trn->get_scale();
			index_buffer16	water_tri_list;

			const int water_bias = 16;
			int water_h = owner->get_water_height()+water_bias;
			int w = rc.get_width();
			int h = rc.get_height();
			int c = chunk_size;
			int step = 4;

			for(int y=0; y<h; y+=step)
			{
				for(int x=0; x<w; x+=step)
				{
					if( x+step >= c
						|| y+step >= c)
						continue;

					int ground_h = trn->get_raw_height(x+rc.left, y+rc.top);
					if( ground_h < water_h )
					{
						unsigned short quad[4]=
						{
							x+y*c,
							x+step + y*c,
							x+step + (y+step)*c,
							x + (y+step)*c,
						};

						// 逆时针建立两个三角形
						water_tri_list.append_index( quad[0] );
						water_tri_list.append_index( quad[3] );
						water_tri_list.append_index( quad[2] );

						water_tri_list.append_index( quad[2] );
						water_tri_list.append_index( quad[1] );
						water_tri_list.append_index( quad[0] );
					}
				}
			}// end of for()

			
			if( water_tri_list.get_index_count() > 0 )
			{
				//-- 创建渲染对象
				render_obj.reset( new water_render_proxy );
				render_obj->owner = owner;

				const float WATER_AMP = 8;

				//-- 计算包裹体
				vector3 box_min(rc.left, -WATER_AMP, rc.top);
				vector3 box_max(rc.right, WATER_AMP, rc.bottom);

				box_min = box_min * owner->get_world_matrix();
				box_max = box_max * owner->get_world_matrix();

				render_obj->bounds.origin = (box_max+box_min)*0.5f;
				render_obj->bounds.box_extent = box_max-box_min;
				render_obj->bounds.sphere_radius = vec_length( render_obj->bounds.box_extent );

				//-- 在共享mesh中为自己添加一个section
				nterrain_water_render_mesh& chunk_mesh = render_obj->chunk_mesh;
				chunk_mesh.create(rc, owner->get_share_mesh(), water_tri_list, owner);
				
			}
		}
		else
		{
			//-- 继续切分
			int half_w = rc.get_width()/2;
			int half_h = rc.get_height()/2;

			/*
				_______________
				|	0	|	1	|
				|_______|_______|
				|	3	|	2	|
				|_______|_______|
			*/
			nrect child_rc[4] ;

			child_rc[0] = nrect(rc.left, rc.top, rc.left+half_w, rc.top+half_h);
			child_rc[1] = nrect(rc.left+half_w, rc.top, rc.right, rc.top+half_h);
			child_rc[2] = nrect(rc.left, rc.top+half_h, rc.left+half_w, rc.bottom);
			child_rc[3] = nrect(rc.left+half_w, rc.top+half_h, rc.right, rc.bottom);

			for(int i=0; i<4; i++)
			{
				children[i].reset( new water_quad_node );
				children[i]->create(child_rc[i], owner, chunk_size);
			}
			
		}// end of else
	}

	void water_quad_node::render(const nviewport& view)
	{
		//-- do frustum cull 


		//--
		if( is_leaf )
		{
			if( render_obj )
			{
				nrenderer_base* rnd = nengine::instance()->get_renderer();
				rnd->frame_object(render_obj.get());
			}
		}
		else
		{
			for(int i=0; i<4; i++)
				children[i]->render(view);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	//	class nterrain_water
	////////////////////////////////////////////////////////////////////////////
	nterrain_water::nterrain_water(const nstring& name_str):nprimitive_component(name_str),
		m_heightmap(NULL), m_heightmap_tex(NULL), m_water_height(0)
	{
		m_trans_flag.set(ETrans_Opaque);
		set_accept_dynamic_light(false);
		set_cast_dynamic_shadow(false);
		m_chunk_size = 0;
	}

	nterrain_water::~nterrain_water(void)
	{
	}

	nterrain_actor* nterrain_water::get_terrain_actor()
	{
		return ndynamic_cast<nterrain_actor>( get_owner() );
	}

	void nterrain_water::get_water_bounds(const nrect& rc, box_sphere_bounds& out_bounds)
	{
		const float WATER_AMP = 8;

		//-- 计算包裹体
		vector3 box_min(rc.left, -WATER_AMP, rc.top);
		vector3 box_max(rc.right, WATER_AMP, rc.bottom);

		box_min = box_min * get_world_matrix();
		box_max = box_max * get_world_matrix();

		out_bounds.origin = (box_max+box_min)*0.5f;
		out_bounds.box_extent = box_max-box_min;
		out_bounds.sphere_radius = vec_length( out_bounds.box_extent );
	}

	void nterrain_water::create(int height, size_t chunk_size)
	{
		m_water_height = height;
		m_chunk_size = chunk_size;

		nterrain_actor* trn = ndynamic_cast<nterrain_actor>( get_owner() );
		//--
		m_heightmap = trn->get_height_map();
		m_heightmap_tex = trn->get_height_map_texture();

		//-- create share mesh
		index_buffer16 dummy_ib;
		dummy_ib.append_index(0);
		dummy_ib.append_index(1);
		dummy_ib.append_index(2);

		nterrain_water_vertex_data vert_data;
		vert_data.init(chunk_size, chunk_size);

		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		m_share_mesh.reset( rres_mgr->alloc_static_mesh_indexed() );
		m_share_mesh->create(EDraw_TriangleList, &vert_data, &dummy_ib, 0);

		//-- create quad tree
		m_root_node.reset( new water_quad_node );

		nrect root_rc;
		root_rc.left = root_rc.top = 0;
		root_rc.right = m_heightmap->get_width();
		root_rc.bottom = m_heightmap->get_height();

		m_root_node->create(root_rc, this, chunk_size);

		//--
		get_water_bounds(nrect(0,0, m_heightmap->get_width(), m_heightmap->get_height()),
			m_bounds);
	}

	nmaterial_base* nterrain_water::get_material(int lod, int mtl_id)	
	{	
		if( m_lod_mtl.empty() 
			|| lod >= m_lod_mtl.size() )
			return nresource_manager::instance()->get_default_material();

		return m_lod_mtl[0].get();
	}

	const matrix44& nterrain_water::get_world_matrix() const
	{	
		return const_cast<nterrain_water*>(this)->get_owner()->get_world_matrix(); 
	}

	void nterrain_water::render(const nviewport& view)
	{
		if( view.show_flags&EShow_InSceneCapture )
			return;

		m_root_node->render(view);
	}

	void nterrain_water::create_material_basic(const resource_location& texture_loc)
	{
		shared_ptr<nwater_mtl_basic> mtl( nNew nwater_mtl_basic(_T("terrain_water_mtl_lod_0")) );
		mtl->create(texture_loc);

		//--
		m_lod_mtl.push_back( mtl );
	}

	void nterrain_water::create_material_natural(int render_target_w, int render_target_h,
		const resource_location& detail_map, const resource_location& bump_map)
	{
		shared_ptr<nwater_mtl_natural> mtl( nNew nwater_mtl_natural(_T("terrain_water_mtl_lod_1")) );
		mtl->create(this, render_target_w, render_target_h, detail_map, bump_map);

		//--
		m_lod_mtl.push_back( mtl );
	}

	float nterrain_water::get_water_height_scaled()
	{
		return m_water_height * get_terrain_actor()->get_scale().y;
	}

	void nterrain_water::serialize(narchive& ar)
	{
		nprimitive_component::serialize(ar);

		nSERIALIZE(ar, m_water_height);
		nSERIALIZE(ar, m_chunk_size);
		nSERIALIZE(ar, m_trans_flag);
		nSERIALIZE(ar, m_lod_mtl);
	}

	void nterrain_water::_level_loaded(nactor* owner)
	{
		nprimitive_component::_level_loaded(owner);

		this->create(m_water_height, m_chunk_size);

		for (size_t i=0; i<m_lod_mtl.size(); i++)
		{
			m_lod_mtl[i]->_level_loaded(this);
		}
	}

	void nterrain_water::_destroy()
	{
		nprimitive_component::_destroy();

		m_heightmap = NULL;
		m_heightmap_tex = NULL;
		m_share_mesh.reset();
		m_root_node.reset();
		m_lod_mtl.clear();
	}
}//namespace nexus