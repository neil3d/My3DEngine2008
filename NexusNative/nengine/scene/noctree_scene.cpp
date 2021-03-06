#include "StdAfx.h"
#include "timer/nprofile.h"
#include "noctree_scene.h"
#include "../renderer/nrenderer_base.h"

namespace nexus
{
	nDEFINE_CLASS(noctree_scene, nscene_manager)

	noctree_scene::noctree_scene(void) :
		m_bshow_prim_octree(false),
		m_prim_count(0),
		m_light_count(0),
		m_prim_octrant( new prim_octree_type)
	{
	}

	noctree_scene::~noctree_scene(void)
	{
	}

	void noctree_scene::get_visible_primitive(const nfrustum& view_frustum,render_package_base* package)
	{
		// Iterate over the octree nodes containing the query point.
		nASSERT(m_prim_octrant);
		
		//START_PROFILE_COUNTER( _T("octree_scene") );
		m_cull_prim =0;
		for ( prim_octree_type::const_iterator pri_it( *m_prim_octrant );
			pri_it.has_pending_node(); pri_it.advance() )
		{
			const prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
			const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();
			box_sphere_bounds bound;

			if(  view_frustum.aabbox_partial_in_frustum( curr_bound.center,curr_bound.extent ))
			{
				std::set<nprimitive_component*> curr_elements = curr_node->get_curr_contents( );
				for ( std::set<nprimitive_component*>::iterator comp_it = curr_elements.begin();
					comp_it != curr_elements.end(); ++comp_it)
				{
					// 再进行一次视锥裁剪
					m_cull_prim++;
					nprimitive_component* prim = *comp_it; 
					if( prim->get_visible())
					{
						if(view_frustum.intersect_bounds(prim->get_bounds()))
						{
							prim->render(package);
						}
					}
				}

				// push children
				for ( int i=0; i< 8; ++i )
				{
					if( curr_node->has_child(i) )
					{
						pri_it.push_children( i );
					}
				}
			}
		}
		//END_PROFILE_COUNTER( _T("octree_scene") );
	}

	void noctree_scene::get_visible_light(const nfrustum& view_frustum,vector<nlight_component*>& out_lgts)
	{
		for( std::set<nlight_component*>::iterator iter = m_lgt_set.begin();
			iter != m_lgt_set.end();
			++iter)
		{
			nlight_component* lgt = *iter;

			if( lgt->get_visible())
			{		
				if(view_frustum.intersect_bounds(lgt->get_bounds()))
				{
					out_lgts.push_back(lgt);
				}
			}
		}
	}

	void noctree_scene::draw( nrender_primitive_draw_interface* PDI )
	{
		if( m_bshow_prim_octree )
		{
			color4f wire_color = color4f(1, 1, 0, 1);
			PDI->begin_line_batch();
			// Iterate over the octree nodes containing the query point.
			nASSERT(m_prim_octrant);
			for ( prim_octree_type::const_iterator pri_it( *m_prim_octrant );
				pri_it.has_pending_node(); pri_it.advance() )
			{
				const prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
				const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();

				// draw current octree node bounds
				vector3	b[2],p,q;
				b[0]= curr_bound.center - vector3(curr_bound.extent,curr_bound.extent,curr_bound.extent);
				b[1]= curr_bound.center + vector3(curr_bound.extent,curr_bound.extent,curr_bound.extent);

				for( int i=0; i<2; i++ ) for( int j=0; j<2; j++ )
				{
					p.x=b[i].x; q.x=b[i].x;
					p.y=b[j].y; q.y=b[j].y;
					p.z=b[0].z; q.z=b[1].z;
					PDI->draw_line(p,q,wire_color);

					p.y=b[i].y; q.y=b[i].y;
					p.z=b[j].z; q.z=b[j].z;
					p.x=b[0].x; q.x=b[1].x;
					PDI->draw_line(p,q,wire_color);

					p.z=b[i].z; q.z=b[i].z;
					p.x=b[j].x; q.x=b[j].x;
					p.y=b[0].y; q.y=b[1].y;
					PDI->draw_line(p,q,wire_color);
				}

				// push children
				for ( int i=0; i< 8; ++i )
				{
					if( curr_node->has_child(i) )
					{
						pri_it.push_children( i );
					}
				}
			}
			PDI->end_line_batch();
		}
	}

	void noctree_scene::clear_all()
	{
		m_prim_octrant->remove_all();
		m_lgt_set.clear();

		m_prim_count = 0;
		m_light_count = 0;
	}

	void noctree_scene::add_primitive(nprimitive_component* prim)
	{
		m_prim_octrant->add_content( prim );

		for( std::set<nlight_component*>::iterator iter = m_lgt_set.begin();
			iter != m_lgt_set.end();
			++iter)
		{
			(*iter)->add_affect_prim(prim);
		}

		m_prim_count ++;
	}

	void noctree_scene::remove_primitive(nprimitive_component* prim)
	{
		m_prim_octrant->remove_content( prim );

		for( std::set<nlight_component*>::iterator iter = m_lgt_set.begin();
			iter != m_lgt_set.end();
			++iter)
		{
			(*iter)->remove_affect_prim(prim);
		}

		m_light_count --;
	}

	void noctree_scene::move_primitive(nprimitive_component* prim)
	{
		m_prim_octrant->move_content( prim );

		for( std::set<nlight_component*>::iterator iter = m_lgt_set.begin();
			iter != m_lgt_set.end();
			++iter)
		{
			nlight_component* lgt = *iter;
			lgt->remove_affect_prim(prim);
			lgt->add_affect_prim(prim);
		}
	}

	void noctree_scene::add_light(nlight_component* lgt)
	{
		m_lgt_set.insert(lgt);
		m_light_count ++;

		lgt->clear();
		if (lgt->m_type == ELight_Point)
		{
			npoint_light_component* point_light = (npoint_light_component*)lgt;
			for ( prim_octree_type::const_iterator pri_it( *m_prim_octrant );
					pri_it.has_pending_node(); pri_it.advance() )
			{
				const prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
				const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();

				if( curr_bound.extent*sqrtf(3) + point_light->get_radius() >vec_distance(curr_bound.center , point_light->get_bounds().origin) )
				{
					std::set<nprimitive_component*> curr_elements = curr_node->get_curr_contents();
					for ( std::set<nprimitive_component*>::iterator comp_it = curr_elements.begin();
						comp_it != curr_elements.end(); ++comp_it)
					{
						nprimitive_component* prim = *comp_it; 
						point_light->add_affect_prim(prim);
					}

					// push children
					for ( int i=0; i< 8; ++i )
					{
						if( curr_node->has_child(i) )
						{
							pri_it.push_children( i );
						}
					}
				}
			}
		}
		else if (lgt->m_type == Elight_Spot)
		{
			nspot_light_component* spot_light = (nspot_light_component*)lgt;
			for ( prim_octree_type::const_iterator pri_it( *m_prim_octrant );
				pri_it.has_pending_node(); pri_it.advance() )
			{
				const prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
				const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();

				const nfrustum& view_frustum = spot_light->view_info.frustum;
				if(  view_frustum.aabbox_partial_in_frustum( curr_bound.center,curr_bound.extent ))
				{
					std::set<nprimitive_component*> curr_elements = curr_node->get_curr_contents();
					for ( std::set<nprimitive_component*>::iterator comp_it = curr_elements.begin();
						comp_it != curr_elements.end(); ++comp_it)
					{
						nprimitive_component* prim = *comp_it; 
						if (prim->get_visible()
							&&prim->accept_dynamic_light()
							&&view_frustum.intersect_bounds(prim->get_bounds()))
						{
							spot_light->add_affect_prim(prim);
						}
					}

					// push children
					for ( int i=0; i< 8; ++i )
					{
						if( curr_node->has_child(i) )
						{
							pri_it.push_children( i );
						}
					}
				}
			}
		}
	}

	void noctree_scene::remove_light(nlight_component* lgt)
	{
		m_lgt_set.erase(lgt);
		m_light_count --;
	}

	void noctree_scene::move_light(nlight_component* lgt)
	{
		remove_light(lgt);
		add_light(lgt);
	}	
}//namespace nexus