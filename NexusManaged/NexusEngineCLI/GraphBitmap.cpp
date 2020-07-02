#include "StdAfx.h"
#include "NEngine.h"
#include "GraphBitmap.h"

namespace NexusEngine
{
	GraphBitmap::GraphBitmap( NLevel^ level )
	{

		m_native_graph = new ngraph_bit_map(level->NativePtr->get_navigation_map()->get_walkable_map());
		m_paths = new path_result_cache();
	}

	GraphBitmap::~GraphBitmap(void)
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

	GraphBitmap::FindPatherResult GraphBitmap::FindPather( int startX, int startY, int endX, int endY )
	{
		m_paths->clear_paths();
		return (GraphBitmap::FindPatherResult)m_native_graph->find_pather(npoint(startX,startY), npoint(endX,endY), m_paths->get_path_vector());
	}

	GraphBitmap::FindPatherResult GraphBitmap::FindPather( PathNode start, PathNode end )
	{
		return FindPather( start.X, start.Y, end.X, end.Y);
	}
} // end of namespace NexusEngine
