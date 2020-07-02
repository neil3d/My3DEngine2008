#pragma once
#include "nspeed_tree_renderer.h"
#include "mesh_render.h"
#include "..\nengine\speed_tree\nspeed_tree_component.h"
#include "..\nengine\actor\nmesh_component.h"
#include "drawing_policy.h"
#include "..\nengine\actor\nfog_volume_component.h"
#include "nrender_fog.h"
#include "nrender_water.h"

//primitive 渲染管理容器，用于对primitive的类型进行分组，以便使用不同的策略渲染
template<class filter=drawing_filter>
class render_package : public render_package_base
{
public:
	render_package(void)	
	{ 
		for (unsigned int type_index = 0; type_index < EMax_Prim; type_index ++ )
		{
			m_meshs[type_index].reserve(4096);
			m_prims.reserve(4096);
		}
	}

	virtual ~render_package(void)	{}

	enum EPrimtive_Type
	{
		EOpaque_Prim,
		ETranslucent_Prim,
		EMax_Prim
	};

	virtual void add_primitive(nprimitive_component* prim)
	{
		prim->render(this);
	}

	virtual void add_mesh(nmesh_component* mesh)
	{
		if (!filter::should_draw(mesh))
		{
			return;
		}

		m_prims.push_back(mesh);
		//是否使用实例渲染
		int lod = mesh->get_render_lod();
		
		const vector<mesh_element>& mesh_elements = mesh->get_mesh_element(lod);

		for(size_t i=0; i<mesh_elements.size(); i++)
		{	
			const mesh_element* element = &mesh_elements[i];
			
			//--允许Components提交多个render mesh，所以写在循环内
			m_mesh_set.insert(element->mesh);

			if (element->mtl->get_blend_type() == EOpaque)
			{
				if (element->instance)
				{
					m_inst_render.add_mesh(element);
				} 
				else
				{
					m_meshs[EOpaque_Prim].push_back(element);	
				}				
			} 
			else if(element->mtl->is_translucent())
			{
				m_meshs[ETranslucent_Prim].push_back(element);	
			}				
		}
	}

	//speedtree比较特殊，使用自己的渲染流程
	virtual void add_spt_tree(nspeed_tree_component* tree)
	{
		if (filter::should_draw(tree))
		{
			m_spt_renderer.attach_tree(tree->get_spt_instance());
			m_prims.push_back(tree);
		}
	}

	virtual void add_fog(nfog_volume_component* fog)
	{
		m_fog_array.push_back(new nfog_volume_proxy(fog));
	}

	virtual void add_water(nwater_component* water_comp)
	{
		m_water_array.push_back(new nwater_render_proxy(water_comp));
	}

	virtual void render_env_effect(const nview_info* view,drawing_policy* dp)
	{
		for (size_t i = 0; i < m_fog_array.size(); i ++)
		{
			m_fog_array[i]->render(view,dp);
		}

		if (m_water_array.size() > 0)
		{
			fft_simulation(view->time);	
		}

		for (size_t i = 0; i < m_water_array.size(); i ++)
		{
			m_water_array[i]->render(view,dp);
		}
	}

	virtual void clear()
	{
		for (unsigned int type_index = 0; type_index < EMax_Prim; type_index ++ )
		{
			m_meshs[type_index].clear();
		}

		m_mesh_set.clear();
		m_spt_renderer.clear();
		m_inst_render.clear();
		m_fog_array.clear();
		m_water_array.clear();
		m_prims.clear();
	}

	virtual void draw_all(const nview_info* view,class drawing_policy* dp)
	{
		for (unsigned int type_index = 0; type_index < EMax_Prim; type_index ++ )
		{
			dp->draw(view, m_meshs[type_index]);
		}

		m_spt_renderer.draw_all_trees(view,dp,true);
		m_inst_render.draw_all(view,dp);
		render_env_effect(view,dp);
	}

	virtual void draw_for_hit_hash(const nview_info* view,class drawing_policy* dp)
	{
		for (unsigned int type_index = 0; type_index < EMax_Prim; type_index ++ )
		{
			dp->draw(view, m_meshs[type_index]);
		}

		m_spt_renderer.draw_all_trees(view,dp,false);
		m_inst_render.draw_all(view,dp);
	}

	virtual void draw_Opaque(const nview_info* view,class drawing_policy* dp)
	{
		dp->draw(view, m_meshs[EOpaque_Prim]);

		m_spt_renderer.draw_all_trees(view,dp,true);
		m_inst_render.draw_all(view,dp);
	}

	const vector<nprimitive_component*>& get_primitives() const
	{
		return m_prims;
	}

	virtual void draw_Translucent(const nview_info* view,class drawing_policy* dp)
	{
		dp->draw(view, m_meshs[ETranslucent_Prim]);
	}

	//进行一些渲染前的数据初始化工作
	virtual void begin(bool update_data = false)
	{
		m_spt_renderer.frame_begin(update_data);
		m_inst_render.init();

		for (std::set<nrender_mesh*>::iterator iter = m_mesh_set.begin(); iter != m_mesh_set.end(); ++iter)
		{
			(*iter)->pre_render();
		}
	}

	virtual void end()
	{
		for (std::set<nrender_mesh*>::iterator iter = m_mesh_set.begin(); iter != m_mesh_set.end(); ++iter)
		{
			(*iter)->post_render();
		}
	}

protected:
	vector<const mesh_element*> m_meshs[EMax_Prim];
	std::set<nrender_mesh*> m_mesh_set;
	vector<const nfog_volume_proxy*> m_fog_array;
	vector<const nwater_render_proxy*> m_water_array;
	vector<nprimitive_component*> m_prims;

	class mesh_instance_renderer m_inst_render;
	nspeed_tree_renderer	m_spt_renderer;
};