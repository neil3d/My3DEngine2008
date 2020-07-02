#include "StdAfx.h"
#include "noctree_scene.h"

namespace nexus
{
	nDEFINE_CLASS(noctree_scene, nscene_manager)


	noctree_scene::noctree_scene(void) :
		m_bshow_prim_octree(false),
		m_prim_octrant( new prim_octree_type)
	{
	}

	noctree_scene::~noctree_scene(void)
	{
	}

	void noctree_scene::render_visible(const view_info& view)
	{
		m_frustum = view.frustum;

		// Iterate over the octree nodes containing the query point.
		nASSERT(m_prim_octrant);
		for ( prim_octree_type::const_iterator pri_it( *m_prim_octrant );
			pri_it.has_pending_node(); pri_it.advance() )
		{
			const prim_octree_type::octreenode_ptr	curr_node	= pri_it.get_curr_node();
			const octree_node_bounds&				curr_bound	= pri_it.get_curr_boud();

			if( m_frustum.aabbox_partial_in_frustum( curr_bound.center,curr_bound.extent ) )
			{
				render_octreenode( curr_node, view );
			}
			else
			{
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
		
		for( std::set<nlight_component*>::iterator iter = m_light_set.begin();
			iter != m_light_set.end();
			++iter)
		{
			nlight_component* lgt = *iter;

			if(lgt->get_owner()->get_visible())
			{
				lgt->render( view );
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

	void noctree_scene::render_octreenode( prim_octree_type::octreenode_ptr pNode, const view_info& view )
	{
		std::set<nprimitive_component*> curr_elements = pNode->get_curr_contents( );
		for ( std::set<nprimitive_component*>::iterator comp_it = curr_elements.begin();
			comp_it != curr_elements.end(); ++comp_it)
		{
			// 再进行一次视锥裁剪
			nprimitive_component* prim = *comp_it; 
			if( prim 
				&& prim->get_owner() 
				&& prim->get_owner()->get_visible()
				&& m_frustum.intersect_bounds(prim->get_bounds()) 
				)
			{
				prim->render(view);
			}
		}

		// for children
		for ( int i=0; i< 8; ++i )
		{
			if( pNode->has_child(i) )
			{
				render_octreenode( pNode->get_child(i), view );
			}
		}
	}

	void noctree_scene::clear_all()
	{
		m_prim_octrant->remove_all();
		//m_light_octrant.remove_all();
	}

	void noctree_scene::add_primitive(nprimitive_component* prim)
	{
		m_prim_octrant->add_content( prim );
	}

	void noctree_scene::remove_primitive(nprimitive_component* prim)
	{
		m_prim_octrant->remove_content( prim );

	}

	void noctree_scene::move_primitive(nprimitive_component* prim)
	{
		m_prim_octrant->move_content( prim );
	}

	void noctree_scene::add_light(nlight_component* lgt)
	{
		m_light_set.insert( lgt );
	}

	void noctree_scene::remove_light(nlight_component* lgt)
	{
		m_light_set.erase( lgt );
	}

	void noctree_scene::move_light(nlight_component* /*lgt*/)
	{
		// @todo:
		//m_light_set.move_content( lgt );
	}

	
}//namespace nexus