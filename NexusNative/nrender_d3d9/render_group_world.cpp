#include "StdAfx.h"
#include "render_group_world.h"
#include "drawing_policy.h"

namespace nexus
{
	render_group_world::render_group_world(void)
	{
	}

	render_group_world::~render_group_world(void)
	{
	}

	void render_group_world::clear()
	{
		for(size_t i=0; i<ETrans_Max; i++)
			m_atoms[i].clear();
	}

	void render_group_world::add_obj(const nrender_proxy* obj)
	{
		if(obj == NULL)
			return;
		for (size_t i=0; i<obj->get_num_mesh_section(); i++)
		{
			m_atoms[obj->get_transparent_type(i)].push_back( drawing_atom(obj,i) );
		}
	}

	void render_group_world::draw_atom_array(st_drawing_atom_array& atoms, const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		for(st_drawing_atom_array::iterator iter = atoms.begin();
			iter != atoms.end();
			++iter)
		{
			const drawing_atom& atom = *iter;
			if( filter )
			{
				if( !filter->should_draw(atom.obj) )
					continue;
			}
			dp->draw_atom(view, atom);
		}
	}

	void render_group_world::draw_all(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		for(size_t i=0; i<ETrans_Max; i++)
		{
			draw_atom_array(m_atoms[i], view, dp, filter);
		}
	}

	bool render_group_world::empty() const
	{
		for(size_t i=0; i<ETrans_Max; i++)
			if( !m_atoms[i].empty() )
				return false;

		return true;
	}

	void render_group_world::draw_opaque(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		draw_atom_array(m_atoms[ETrans_Opaque], view, dp, filter);
		draw_atom_array(m_atoms[ETrans_AlphaMasked], view, dp, filter);
	}

	void render_group_world::draw_unlit_trans(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter)
	{
		draw_atom_array(m_atoms[ETrans_UnlitTrans], view, dp, filter);
	}

	struct _cmp_opaque
	{
		const d3d_view_info* view;
		_cmp_opaque(const d3d_view_info* _view):view(_view)
		{}

		bool operator() (const drawing_atom& atom1, const drawing_atom& atom2)
		{
			const box_sphere_bounds& bounds1 = atom1.obj->get_bounds();
			const box_sphere_bounds& bounds2 = atom2.obj->get_bounds();

			return vec_distance_sq(view->eye_pos, bounds1.origin) <
				vec_distance_sq(view->eye_pos, bounds2.origin);
		}
	};

	struct _cmp_trans
	{
		const d3d_view_info* view;
		_cmp_trans(const d3d_view_info* _view):view(_view)
		{}

		bool operator() (const drawing_atom& atom1, const drawing_atom& atom2)
		{
			const box_sphere_bounds& bounds1 = atom1.obj->get_bounds();
			const box_sphere_bounds& bounds2 = atom2.obj->get_bounds();

			return vec_distance_sq(view->eye_pos, bounds1.origin) >
				vec_distance_sq(view->eye_pos, bounds2.origin);
		}
	};

	void render_group_world::sort_draw_order(const d3d_view_info* view)
	{
		// 将对象排序，以适合渲染需求：不透明物体从前向后，透明物体从后向前
		_cmp_opaque cmp_opaque(view);
		std::sort(m_atoms[ETrans_Opaque].begin(),
			m_atoms[ETrans_Opaque].end(),
			cmp_opaque);

		std::sort(m_atoms[ETrans_AlphaMasked].begin(),
			m_atoms[ETrans_AlphaMasked].end(),
			cmp_opaque);

		_cmp_trans cmp_trans(view);
		std::sort(m_atoms[ETrans_UnlitTrans].begin(),
			m_atoms[ETrans_UnlitTrans].end(),
			cmp_trans);
	}
}//namespace nexus