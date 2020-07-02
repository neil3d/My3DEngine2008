/**
*	Nexus Engine
*
*	Author: zewoo
*	Date:	2010-03-27
*/

#ifndef _NEXUS_ENGINE_NAV_MAP_ACTOR_H_
#define _NEXUS_ENGINE_NAV_MAP_ACTOR_H_
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../renderer/nrender_resource.h"
#include "nav_map_component.h"
#include "../../ncore/game_map/navigation_map.h"

namespace nexus
{
	class navigation_map;

	class nAPI nav_map_actor :
		public nactor
	{
	public:
		typedef shared_ptr<nav_map_actor> ptr;

		nav_map_actor(const nstring& name_str);
		virtual ~nav_map_actor(void);

		// 创建nav_map_actor对象成功后调用
		void create(navigation_map::ptr nav_map);

		// 导入高度图数据
		void import_heightmap(const nstring& img_file_name);

		// 获取 Height Map
		nheight_map16* get_height_map() const { return m_nav_map->get_height_map(); }
		nbit_map* get_walkable_map() const { return m_nav_map->get_walkable_map(); }
		navigation_map::ptr get_navigation_map() const { return m_nav_map; }

		// 不支持rotation
		virtual void move(const vector3& pos, const vector3& rot, const vector3& scale);
		void move(const vector3& pos, const vector3& scale);

		// 射线检测
		virtual bool line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type);
		/**	测试一个点是在地面以上还是以下
		@return 1 == up, -1 == low */
		int classify_side(const vector3& wpt);

		bool empty() { return (!m_nav_map && !m_nav_map.get()); }

		// 导航图发生变化
		void post_heightmap_change(const nrect& region);

		// 设置裁剪距离
		float get_cull_dist() const { return m_cull_dist; }
		void set_cull_dist(float value)  {  m_cull_dist = value; }

		// 设置导航图顶点渲染颜色
		const color4f& get_height_nav_color() const { return m_height_nav_color; }
		void set_height_nav_color(const color4f& value) { m_height_nav_color = value; }

		const color4f& get_walkable_nav_color() const { return m_walkable_nav_color; }
		void set_walkable_nav_color(const color4f& value) { m_walkable_nav_color = value; }

		bool get_walkable_map_visibale() const { return m_walkable_map_visibale; }
		void set_walkable_map_visibale(bool value) { m_walkable_map_visibale = value; }

	private:
		void recreate_chunks();

	private:
		navigation_map::ptr m_nav_map;

		// 导航图顶点渲染颜色
		// 高度图颜色
		color4f		m_height_nav_color;
		// 可行走区域颜色
		bool		m_walkable_map_visibale;
		color4f		m_walkable_nav_color;

		// 单个用来渲染的形块的大小
		size_t				m_chunk_size;
		// 裁剪距离
		float				m_cull_dist;

		//-- 渲染数据，这里保存一个副本方便代码使用
		typedef std::vector<nav_map_component::ptr> st_chunk_array;
		st_chunk_array		m_chunks;

		nDECLARE_NAMED_CLASS(nav_map_actor)
	};

} // end of namespace nexus


#endif // end of _NEXUS_ENGINE_NAV_MAP_ACTOR_H_