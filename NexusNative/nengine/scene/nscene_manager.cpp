#include "StdAfx.h"
#include "nscene_manager.h"

namespace nexus
{
	nDEFINE_CLASS(nscene_manager, nobject)

	nscene_manager::nscene_manager(void):
	m_bshow_prim_octree(false),
	m_prim_count(0),
	m_light_count(0),
	m_prim_octrant( new prim_octree_type)
	{
	}

	nscene_manager::~nscene_manager(void)
	{
	}

	void nscene_manager::get_visible_primitive(const nfrustum& view_frustum,render_package_base* package)
	{
		// Iterate over the octree nodes containing the query point.
		nASSERT(m_prim_octrant);

		//START_PROFILE_COUNTER( _T("octree_scene") );
		for ( prim_octree_type::const_iterator pri_it( *m_prim_octrant );
			pri_it.has_pending_node(); pri_it.advance() )
		{
			const prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
			const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();
			box_sphere_bounds bound;

			if(  view_frustum.aabbox_partial_in_frustum( curr_bound.center,curr_bound.extent ))
			{
				const std::set<nprimitive_component*>& curr_elements = curr_node->get_curr_contents( );
				for ( std::set<nprimitive_component*>::const_iterator comp_it = curr_elements.begin();
					comp_it != curr_elements.end(); ++comp_it)
				{
					// 再进行一次视锥裁剪
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

	void nscene_manager::get_visible_light(const nfrustum& view_frustum,vector<nlight_component*>& out_lgts)
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

	void nscene_manager::draw( nrender_primitive_draw_interface* PDI )
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

	void nscene_manager::clear_all()
	{
		m_prim_octrant->remove_all();
		m_lgt_set.clear();

		m_prim_count = 0;
		m_light_count = 0;
	}

	void nscene_manager::add_primitive(nprimitive_component* prim)
	{
		m_prim_octrant->add_content( prim );

		for( std::set<nlight_component*>::iterator iter = m_lgt_set.begin();
			iter != m_lgt_set.end();
			++iter)
		{
			(*iter)->add_affect_prim(prim);
		}

	//	m_prims.push_back(prim);
		m_prim_count ++;
	}

	void nscene_manager::remove_primitive(nprimitive_component* prim)
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

	void nscene_manager::move_primitive(nprimitive_component* prim)
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

	void nscene_manager::add_light(nlight_component* lgt)
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
					const std::set<nprimitive_component*>& curr_elements = curr_node->get_curr_contents();
					for ( std::set<nprimitive_component*>::const_iterator comp_it = curr_elements.begin();
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
					const std::set<nprimitive_component*>& curr_elements = curr_node->get_curr_contents();
					for ( std::set<nprimitive_component*>::const_iterator comp_it = curr_elements.begin();
						comp_it != curr_elements.end(); ++comp_it)
					{
						nprimitive_component* prim = *comp_it; 
						spot_light->add_affect_prim(prim);
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

	void nscene_manager::remove_light(nlight_component* lgt)
	{
		m_lgt_set.erase(lgt);
		m_light_count --;
	}

	void nscene_manager::move_light(nlight_component* lgt)
	{
		remove_light(lgt);
		add_light(lgt);
	}	

	void nscene_manager::on_event(nevent_base& evt)
	{
		if( evt.m_event_id != NSCENE_EVENT_ID )
			return;

		nscene_event& scn_evt = static_cast<nscene_event&>(evt);

		switch(scn_evt.m_action)
		{
		case ESA_AddPrimitive:
			add_primitive(scn_evt.m_prim);
			break;
		case ESA_RemovePrimitive:
			remove_primitive(scn_evt.m_prim);
			break;
		case ESA_MovePrimitive:
			move_primitive(scn_evt.m_prim);
			break;
		case ESA_AddLight:
			add_light(scn_evt.m_light);
			break;
		case ESA_RemoveLight:
			remove_light(scn_evt.m_light);
			break;
		case ESA_MoveLight:
			move_light(scn_evt.m_light);
			break;
		default:
			nASSERT( 0 && "unknown scene event action");
			break;
		}
	}
}//namespace nexus