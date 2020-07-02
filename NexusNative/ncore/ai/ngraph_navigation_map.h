#ifndef _NEXUS_CORE_GRAPH_NAVIGATION_MAP_H_
#define _NEXUS_CORE_GRAPH_NAVIGATION_MAP_H_
#include "../ncore.h"
#include "../game_map/navigation_map.h"
#include "micropather.h"
using namespace micropather;

namespace nexus
{
	/**
	* 定义基于micropather的位图寻路图
	*/
	class nCoreAPI ngraph_navigation_map 
		: public Graph
	{
	private:
		ngraph_navigation_map(){}

		DISALLOW_COPY_AND_ASSIGN(ngraph_navigation_map);
	public:
		typedef std::vector<npoint> path_result_type;

		enum EFindPatherResult
		{
			EFPR_SOLVED,
			EFPR_NO_SOLUTION,
			EFPR_START_END_SAME,
		};

		explicit ngraph_navigation_map(navigation_map::ptr bit_map);
		virtual ~ngraph_navigation_map();

		void destroy();

		// 寻路
		// param: start 寻路起始点的格子坐标
		// param: end 寻路终止点的格子坐标
		// param: step_height 高度差大于stepoffest认为是不可通过
		// results: 返回找到的路径点
		EFindPatherResult find_pather(const npoint& start, const npoint& end, float step_height, path_result_type& results);

		MP_UPTR check_sum() { return m_pather->Checksum(); }

		void reset_pather() { m_pather->Reset(); }

		bool passable(size_t x, size_t y)
		{
			return m_nav_map->get_tile_walkable(x, y);
		}

		// micropather node 和坐标之间的转换
		void node2tile(void* node, int* x, int* y)
		{
			int index = (int)node;
			*y = index / m_nav_map->get_raw_width();
			*x = index - *y * m_nav_map->get_raw_width();
		}

		void* tile2node(int x, int y)
		{
			return (void*)( y * m_nav_map->get_raw_width() + x);
		}

		// 重载Graph接口,使用启发式计算公式
		virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd );

		virtual void AdjacentCost( void* node, std::vector< StateCost > *neighbors );

		virtual void PrintStateInfo( void* node );

	private:
		// micropather
		MicroPather* m_pather;
		navigation_map::ptr m_nav_map;

		// 定义格子高度差大于一定值以后就不能通过
		float m_step_height;
	};

}// end of namespace nexus

#endif // end of _NEXUS_CORE_GRAPH_NAVIGATION_MAP_H_