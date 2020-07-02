#include "stdafx.h"
#include "ngraph_navigation_map.h"

namespace nexus
{
	ngraph_navigation_map::ngraph_navigation_map( navigation_map::ptr nav_map )
	{
		m_step_height=0.f;

		m_nav_map = nav_map;

		m_pather = new MicroPather( this, (m_nav_map->get_raw_width() * m_nav_map->get_raw_height()));
	}

	ngraph_navigation_map::~ngraph_navigation_map()
	{
		destroy();
	}

	void ngraph_navigation_map::destroy()
	{
		if(m_pather)
		{
			delete m_pather;
			m_pather = NULL;
		}
	}

	ngraph_navigation_map::EFindPatherResult ngraph_navigation_map::find_pather( const npoint& start, const npoint& end,float step_height, path_result_type& results)
	{
		m_step_height = step_height;
		EFindPatherResult result = EFPR_NO_SOLUTION;
		results.clear();
		if( passable(end.x, end.y) )
		{
			float total_cost;

			typedef std::vector<void*> micropather_type;
			micropather_type paths;
			result = (EFindPatherResult)m_pather->Solve( tile2node( start.x, start.y), tile2node( end.x, end.y), &paths, &total_cost );

			for (micropather_type::iterator it = paths.begin(); it != paths.end(); ++it)
			{
				npoint pt;
				node2tile((*it), &pt.x, &pt.y);
				results.push_back(pt);
			}
			// debug
			//nLog_Info( _T("pather returned %d \r\n"), result );
		}
		return result;
	}

	float ngraph_navigation_map::LeastCostEstimate( void* nodeStart, void* nodeEnd )
	{
		int xStart, yStart, xEnd, yEnd;
		node2tile( nodeStart, &xStart, &yStart );
		node2tile( nodeEnd, &xEnd, &yEnd );

		/* Compute the minimum path cost using distance measurement. It is possible
		to compute the exact minimum path using the fact that you can move only 
		on a straight line or on a diagonal, and this will yield a better result.
		*/
		int dx = xStart - xEnd;
		int dy = yStart - yEnd;
		//return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
		return (float)(abs(dx) + abs(dy));
	}

	void ngraph_navigation_map::AdjacentCost( void* node, std::vector< StateCost > *neighbors )
	{
		int x, y;
		const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

		node2tile( node, &x, &y );
		vector3 scale = m_nav_map->get_scale();
		int raw_height = m_nav_map->get_raw_height(x, y);

		for( int i=0; i<8; ++i )
		{
			int nx = x + dx[i];
			int ny = y + dy[i];

			bool pass = (nx > 0 && ny > 0) && passable( (size_t)nx, (size_t)ny );
			int raw_step = m_nav_map->get_raw_height(nx, ny) - raw_height;
			if ( pass && raw_step * scale.y <= m_step_height ) 
			{
				// Normal floor
				StateCost nodeCost = { tile2node( nx, ny ), cost[i] + raw_step * 0.1f };
				neighbors->push_back( nodeCost );
			}
			else 
			{
				// Normal floor
				StateCost nodeCost = { tile2node( nx, ny ), FLT_MAX };
				neighbors->push_back( nodeCost );
			}
		}
	}

	void ngraph_navigation_map::PrintStateInfo( void* node )
	{
		int x, y;
		node2tile( node, &x, &y );
		printf( "(%u,%u)", x, y );
	}


} // end of namespace nexus