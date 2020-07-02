#include "StdAfx.h"
#include "drawing_policy.h"
#include "d3d9_vertex_factory.h"
#include "shading_effect.h"
#include "d3d_device_manager.h"
#include "d3d9_mesh_section.h"
#include "nrenderer_d3d9.h"
#include "../nengine/actor/nmesh_component.h"
#include "util.h"
#include "../nengine/util/hud_info.h"

namespace nexus
{
	drawing_policy::drawing_policy(void):m_tech("techDefault")
	{
		m_policy_tech = EDP_Num;
	}

	void drawing_policy::set_technique(d3d9_shading_effect* effect_ptr)
	{
		effect_ptr->get_d3d_effect()->SetTechnique(m_tech.c_str());
	}

	void drawing_policy::default_create_type(dp_type_list& type_list, const nstring& dp_name)
	{
		drawing_policy_type::ptr new_type(
			nNew drawing_policy_type(dp_name)
			);

		new_type->load_shader_file();
		type_list.add_drawing_policy_type(new_type);
	}

	void drawing_policy::setup_effect( d3d9_shading_effect* effect_ptr, const nprimitive_component* prim_comp )
	{}

	void drawing_policy::draw(const nview_info* view,const vector<const mesh_element*>& mesh_elements)
	{
		typedef map<d3d9_shading_effect*,vector<const mesh_element*> > effect_map_type;
		effect_map_type  effect_map;

		for (size_t i = 0; i < mesh_elements.size(); i ++)
		{
			/*	g_hud_info.draw_call_count++;
			continue;*/	
			nmtl_base* mtl = mesh_elements[i]->mtl;	

			if (mtl->get_blend_type() == ETranslucent
				&& (m_policy_tech == EDP_PreDepth
				|| m_policy_tech== EDP_Directional_Light
				|| m_policy_tech== EDP_Spot_Light
				|| m_policy_tech== EDP_Point_Light))
			{
				continue;
			}

			d3d9_mesh_section* mesh_sec = static_cast<d3d9_mesh_section*>(mesh_elements[i]->sec);		
			effect_cache& cache = mesh_sec->cache_effect[get_policy_tech()];
			d3d9_shading_effect*  effect_ptr = (d3d9_shading_effect* )cache.effect;

			//-- 找到或者创建材质Pass Shader需要的effect		
			if(!effect_ptr||cache.mtl != mtl)
			{	
				nrender_mesh* mesh = mesh_elements[i]->mesh;
				
				d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>( mesh->get_vertex_factory());		
				nASSERT(vf != NULL);
	
				nmtl_tech_shader::ptr mtl_shader = mtl->get_shader();
				nASSERT(mtl_shader != NULL);

				effect_ptr = shading_effect_lib::instance()->acquire_shader(get_type(), vf->get_type(), 
					mtl_shader.get(), mesh->get_shader_mod(),mtl->get_shader_mod());
				cache.effect = effect_ptr;
				cache.mtl = mtl;
			}

			nASSERT(effect_ptr);

			effect_map_type::iterator it = effect_map.find(effect_ptr);
			if (it != effect_map.end())
			{
				it->second.push_back(mesh_elements[i]);
			}
			else
			{
				effect_map.insert(make_pair(effect_ptr, vector<const mesh_element*>(1,mesh_elements[i])));
			}
		}

		for( effect_map_type::const_iterator it = effect_map.begin();
			it != effect_map.end();
			++it)
		{
			d3d9_shading_effect*  effect_ptr = it->first;
			set_technique(effect_ptr);
			effect_ptr->begin();
			for (size_t i = 0; i < it->second.size(); i ++)
			{
				const mesh_element* element = it->second[i];

				nrender_mesh* mesh = element->mesh;
				nmtl_base* mtl = element->mtl;
				d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>( mesh->get_vertex_factory());		

				d3d9_mesh_section* mesh_sec = static_cast<d3d9_mesh_section*>(element->sec);		

				nmtl_tech_shader::ptr mtl_shader = mtl->get_shader();

				//-- 为渲染设定vertex buffer和index buffer到device
				if (!vf->draw_set_full())
				{
					continue;
				}

				mesh_sec->draw_set_index_buffer();

				//-- 渲染
				setup_effect(effect_ptr, element->comp);
				mesh->draw_setup_effect(effect_ptr);
				mtl->setup_effect(effect_ptr);	// 材质在effect begin之后

				effect_ptr->set_system_parameter(view,element->comp? element->comp->get_world_matrix():matrix44::identity);
				effect_ptr->get_d3d_effect()->CommitChanges();
				mesh_sec->draw_call(vf->get_num_vert());

				vf->draw_reset();
			}
			
			effect_ptr->end();
		}
	}
	
	void drawing_policy::draw(const nview_info* view,const mesh_element& element)
	{
		//g_hud_info.draw_call_count++;
		//return;

		nrender_mesh* mesh = element.mesh;
		nmtl_base* mtl = element.mtl;
		d3d9_vertex_factory* vf = static_cast<d3d9_vertex_factory*>( mesh->get_vertex_factory());		
		nASSERT(vf != NULL);

		d3d9_mesh_section* mesh_sec = static_cast<d3d9_mesh_section*>(element.sec);		

		nmtl_tech_shader::ptr mtl_shader = mtl->get_shader();
		nASSERT(mtl_shader != NULL);

		effect_cache& cache = mesh_sec->cache_effect[get_policy_tech()];
		d3d9_shading_effect*  effect_ptr = (d3d9_shading_effect* )cache.effect;
		//-- 找到或者创建材质Pass Shader需要的effect		
		if(!effect_ptr||cache.mtl != mtl)
		{
			effect_ptr = shading_effect_lib::instance()->acquire_shader(get_type(), vf->get_type(), 
				mtl_shader.get(), mesh->get_shader_mod(),mtl->get_shader_mod());
			cache.effect = effect_ptr;
			cache.mtl = mtl;
		}

		nASSERT(effect_ptr);

		//-- 为渲染设定vertex buffer和index buffer到device
		vf->draw_set_full();

		mesh_sec->draw_set_index_buffer();
		//-- 渲染
		set_technique(effect_ptr);
		setup_effect(effect_ptr, element.comp);
		mesh->draw_setup_effect(effect_ptr);
		mtl->setup_effect(effect_ptr);	// 材质在effect begin之后
		//
		effect_ptr->set_system_parameter(view,element.comp? element.comp->get_world_matrix():matrix44::identity);
		effect_ptr->begin();
		mesh_sec->draw_call(vf->get_num_vert());
		effect_ptr->end();

		vf->draw_reset();
	}
}//namespace nexus