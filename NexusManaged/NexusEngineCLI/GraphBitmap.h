#pragma once
#include "../NexusNative/ncore/ai/ngraph_bit_map.h"
#include "../NexusNative/ncore/ai/path_result_cache.h"

namespace NexusEngine
{
	ref class NLevel;

	/**
	* 对native code nexus::ngraph_bit_map的CLI包装 
	*/
	public ref class GraphBitmap
	{
	private:
		GraphBitmap(void) {}
	public:
		GraphBitmap(NLevel^ level);
		virtual ~GraphBitmap(void);

		enum class FindPatherResult
		{
			Solved			= nexus::ngraph_bit_map::EFPR_SOLVED,
			NoSolution		= nexus::ngraph_bit_map::EFPR_NO_SOLUTION,
			StartEndSame	= nexus::ngraph_bit_map::EFPR_START_END_SAME,
		};

		// nest path node class
		value struct PathNode 
		{
			int X; 
			int Y;

			PathNode(int x, int y)
			{
				this->X = x; this->Y = y;
			}
		};

		// 对ngraph_bit_map::find_pather的包装
		FindPatherResult FindPather(int startX, int startY, int endX, int endY);
		FindPatherResult FindPather(PathNode start, PathNode end);

		/// 路径点的数量
		property UInt32 PathNodeSize
		{
			UInt32 get()
			{
				return m_paths->get_num_paths();
			}
		}

		/// 迭代器接口
		/// 迭代器使用方式如：
		//PathNode node = FirstNode;
		//for(UInt32 i=0; i<PathNodeSize; ++i)
		//{
		//		PathNode[i];
		//}
		property PathNode default[int]
		{
			PathNode get( int index )
			{
				npoint pt = m_paths->get_path(index);
				return PathNode(pt.x, pt.y);
			}
		}


		void ResetPath()
		{
			m_native_graph->reset_pather();
			ClearPathResultCache();
		}

		void ClearPathResultCache()
		{
			m_paths->clear_paths();
		}

		property nexus::ngraph_bit_map* NativePtr
		{
			ngraph_bit_map* get()
			{
				return m_native_graph;
			}
		}
	private:
		nexus::ngraph_bit_map* m_native_graph;
		// 缓存找到的路径，在托管代码这一层使用迭代器模式避免STL容器的装换
		path_result_cache* m_paths;
	};

} // end of namespace NexusEngine