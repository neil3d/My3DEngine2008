/**
*	Core of Nexus Engine
*
*	Author: zewoo
*	Date:	2010.3 @raycolor.net
*/

#ifndef _NEXUS_CORE_NAVIGATION_MAP_H_
#define _NEXUS_CORE_NAVIGATION_MAP_H_
#include "../ncore.h"
#include "../height_map/nheight_map.h"
#include "../height_map/nbit_map.h"
#include "../math/ncheck_result.h"

namespace nexus
{
	/**	\class navigation_map
		\brief Level导航信息,每个游戏中Level应该保存一个
	*/
	class nCoreAPI navigation_map
	{
	public:
		typedef boost::shared_ptr<navigation_map> ptr;

		navigation_map(){};
		virtual ~navigation_map(){};

		// 克隆导航图，该函数会new一个导航图返回
		static navigation_map::ptr clone(navigation_map* src_nav_map)
		{
			navigation_map::ptr new_nav_map(new navigation_map);
			new_nav_map->create(src_nav_map->get_raw_width(),src_nav_map->get_raw_height(), src_nav_map->get_location(), src_nav_map->get_scale());
			new_nav_map->blit(src_nav_map, 0, 0);

			return new_nav_map;
		}

		/** 创建导航图
			\param : w指定导航图宽度 h高度 loc导航图位置，scale缩放
		*/
		virtual void create(size_t w, size_t h, const vector3& loc, const vector3& scale);

		//-- 射线检测算法
		// param start: 射线检测的起始点（世界坐标）
		// param start: 射线检测的终止点（世界坐标）
		// param walkable_check: 可否行走区域检测，true：检测是否可行走图，false：不检测是否可行走图，只检测高度图
		// param out_hit: 输出检测到碰撞信息
		// return false:两点之间能顺利通过没有阻挡，true: 两点之间有阻挡
		virtual bool line_of_sight(const vector3& start, const vector3& end, bool walkable_check, ncheck_result& out_hit);
		/**	测试一个点是在地面以上还是以下
		@return 1 == 在导航图上方, -1 == 在导航图下方 */
		int classify_side(const vector3& wpt);
		int classify_side(const npoint& tile_pt, float test_height);

		/** 使用画线算法判断是否可直线到达,只检测是否可行走位图
		\ param start_pt:	起始格子坐标
		\ param end_pt:		终止格子坐标
		\ param near_pos:	输出一个距离最近的点
		\ return 到达一个最近点或终点会返回true
		*/
		bool can_walkable_to(const npoint& start_pt,const npoint& end_pt, npoint& out_near_pos);

		//-- 从src_map中copy高度信息，可以指定开始copy的偏移
		virtual void blit(navigation_map* src_map, size_t x_offest, size_t y_offest);

		//-- 改变导航图的大小，但是保留原来的高度图信息
		virtual void resize(size_t w, size_t h);

		//-- 世界坐标到格子坐标装换
		npoint world2tile(float wx, float wz) const;
		
		npoint safe_world2tile(float x,float z) const;

		//-- 格子坐标到世界坐标转换
		vector3 tile2world(int tx, int tz) const;

		//-- 通过世界坐标获取高度
		float get_world_height(float wx, float wz) const;

		//-- 通过格子坐标获取高度
		float get_tile_height(int tx, int tz) const;

		//-- 通过格子坐标获取对应的原始高度图高度
		int get_raw_height(int tx, int tz) const
		{
			if( m_height_map.is_valid() 
				&& m_height_map.is_valid_index(tx,tz) ) 
				return m_height_map.get_value(tx,tz);
			else
				return 0;
		}

		bool is_valid_index(int tx, int tz) const
		{
			return m_height_map.is_valid_index(tx,tz);
		}

		//-- 获取高度图的宽度和高度
		int get_raw_width() const { return m_height_map.get_width(); }
		int get_raw_height() const { return m_height_map.get_height(); }

		// 设置高度图格子高度
		void set_raw_height(int x, int y, short value) { m_height_map.set_value(x,y,value); }
		void set_height(int x, int y, float value);

		nheight_map16* get_height_map() { return &m_height_map; }
		nbit_map* get_walkable_map() { return &m_walkable_map; }

		//-- 对应的格子是否可行走
		bool get_tile_walkable(int tx, int tz) const;
		void set_tile_walkable(int tx, int tz, bool value) { m_walkable_map.set_value(tx,tz,value); };
		//-- 通过世界坐标获取是否可行走
		bool get_world_walkable(float wx, float wz) const;

		// 得到某个格子的法线
		vector3 get_tile_normal(int x, int z) const;

		//-- 设置和获取缩放值
		const vector3& set_scale(const vector3& value)	{ m_scale = value; return m_scale; }
		const vector3& get_scale() const { return m_scale; }

		//-- 设置和起始位置缩放值
		const vector3& set_location(const vector3& value)	{ m_location = value; return m_location; }
		const vector3& get_location() const { return m_location; }

		//--  序列化
		virtual void serialize( nfile_system* fs,const nstring& pkg_name,const nstring& file_name, enum EFileMode mode );

	private:
		//-- 导航图的位置，位置决定导航图的偏移
		vector3 m_location;
		//-- 导航图的缩放，缩放x，z决定了导航图格子大小，y决定高度缩放
		vector3 m_scale;
		//-- 高度图信息
		nheight_map16	m_height_map;
		//-- 可否行走,true:代表可行走，false:代表不可行走
		nbit_map	m_walkable_map;
	};

}//namespace nexus

#endif // end of _NEXUS_CORE_NAVIGATION_MAP_H_