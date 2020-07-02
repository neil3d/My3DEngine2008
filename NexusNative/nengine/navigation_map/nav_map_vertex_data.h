/**
*	Nexus Engine
*
*	Author: zewoo
*	Date:	2010-03-27
*/

#ifndef _NEXUS_ENGINE_NAV_MAP_VERTEX_DATA_H_
#define _NEXUS_ENGINE_NAV_MAP_VERTEX_DATA_H_
#include "../mesh/mesh_vertex_data.h"

namespace nexus
{

	/**
	* 渲染导航图用的顶点数据格式
	* @remark 位置，顶点颜色
	*/
	class nav_map_vertex_data :
		public nmesh_vertex_data
	{
	public:
		nav_map_vertex_data(void);
		virtual ~nav_map_vertex_data(void);

		void init(size_t num_vert);

		//-- 重载基类成员
		virtual size_t get_num_stream() const { return 2; }
		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		virtual vertex_stream* get_stream(size_t stream_index);

		//-- 导航图的顶点不需要序列化
		virtual void serialize(narchive& ar);

	private:
		// 顶点颜色
		vertex_stream	m_diffuse_stream;
	};

} // end of namespace Nexus

#endif // end of _NEXUS_ENGINE_NAV_MAP_VERTEX_DATA_H_