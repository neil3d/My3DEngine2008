#include "StdAfx.h"
#include "NEngine.h"
#include "GraphNavigationMap.h"

namespace NexusEngine
{
	GraphNavigationMap::GraphNavigationMap( NLevel^ level )
	{

		m_native_graph = new ngraph_navigation_map(level->NativePtr->get_navigation_map());
		m_paths = new path_result_cache();
	}

	GraphNavigationMap::~GraphNavigationMap(void)
	{
		if (m_native_graph)
		{
			delete m_native_graph;
			m_native_graph = NULL;
		}
		if(m_paths)
		{
			delete m_paths;
			m_paths = NULL;
		}
	}

	GraphNavigationMap::FindPatherResult GraphNavigationMap::FindPather( int startX, int startY, int endX, int endY, float step )
	{
		m_paths->clear_paths();
		return (GraphNavigationMap::FindPatherResult)m_native_graph->find_pather(npoint(startX,startY), npoint(endX,endY), step, m_paths->get_path_vector());
	}

	GraphNavigationMap::FindPatherResult GraphNavigationMap::FindPather( PathNode start, PathNode end, float step)
	{
		return FindPather( start.X, start.Y, end.X, end.Y,step);
	}
} // end of namespace NexusEngine
