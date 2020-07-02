/**
 *	nexus ngame - nworld_object
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NWORLD_OBJECT_H_
#define _NGAME_NWORLD_OBJECT_H_

#include "ncommon.h"
#include "nobject_define.h"
#include "nevent_processor.h"
#include "nmove_controller.h"
#include "base_define.h"
#include "math/vector.h"

namespace nexus {

	class nmap;

	enum EAoiFlag
	{
		EAF_None	= 0x00,
		EAF_Old		= 0x01,
		EAF_New		= 0x02,
		EAF_All		= 0x03,
	};

	class nworld_object : private nnoncopyable
	{
	public:
		nworld_object();
		virtual ~nworld_object();

		bool init(uint32 map_id, uint32 instance_id, const vector3& pos, const vector3& rot);
		virtual void destroy();
		virtual void update(uint32 elapse);

		nevent_processor& get_event_processor()	{ return m_event_processor; }
		void send_event(const void* event_ptr, uint32 size)							{ m_event_processor.send_event(event_ptr, size); }
		void post_event(uint32 expire_time, const void* data_ptr, uint32 size)		{ m_event_processor.post_event(expire_time, data_ptr, size); }

		uint64 get_id() const					{ return m_guid; };
		uint8 get_type() const					{ return GET_TYPE(m_guid); }
		uint32 get_map_id() const				{ return m_map_id; }
		uint32 get_instance_id() const			{ return m_instance_id; }
		nmap* get_map() const					{ return m_map_ptr; }
		const vector3& get_position() const		{ return m_move_controller.get_position(); }
		const vector3& get_rotation() const		{ return m_move_controller.get_rotation(); }
		const vector3& get_acceleration() const	{ return m_move_controller.get_acceleration(); }
		uint8 get_movement_type() const			{ return m_move_controller.get_current_movement_type(); }
		uint8 get_movement_flag() const			{ return gameframework::nsavedmove::compressed_controller_flags((gameframework::ncharacter_controller*)&m_move_controller, m_custom_flag); }
		uint32 get_tile_id() const				{ return m_tile_id; }
		const vector3& get_tile_pos() const		{ return m_tile_pos; }

		void set_id(uint64 guid)				{ m_guid = guid; }
		void set_map_id(uint32 map_id)			{ m_map_id = map_id; }
		void set_instance_id(uint32 instance_id){ m_instance_id = instance_id; }
		void set_map(nmap* map_ptr)				{ m_map_ptr = map_ptr; }
		void set_position(const vector3& pos) 	{ m_move_controller.set_position(pos); }
		void set_rotation(const vector3& rot) 	{ m_move_controller.set_rotation(rot); }
		void set_acceleration(const vector3& ac){ m_move_controller.set_acceleration(ac); }
		void set_movement_type(uint8 move_type) { m_move_controller.set_current_movement_type(gameframework::EMovementType(move_type)); }
		void set_tile_id(uint32 tile_id)		{ m_tile_id = tile_id; }
		void set_tile_pos(const vector3& pos)	{ m_tile_pos = pos; }

		virtual void make_enter_msg(void* data_ptr, uint32& size) const	= 0;
		void make_move_msg(void* data_ptr, uint32& size) const;
		void make_leave_msg(void* data_ptr, uint32& size) const;

		virtual bool can_seen(nworld_object* obj_ptr)		{ return true; }

		void send_message_to_aoi(void* msg_ptr, uint32 size, bool to_self = true);

		virtual float get_object_size() const				{ return 70.0f; }
		float get_distance(nworld_object const* obj_ptr) const;
		float get_distance(const vector3& pos) const;
		bool is_in_dist(nworld_object const* obj_ptr, float dist) const;
		bool is_in_dist(const vector3& pos, float dist) const;
		bool is_in_range(nworld_object const* obj_ptr, float min_range, float max_range) const;
		bool is_in_range(const vector3& pos, float min_range, float max_range) const;
		bool line_of_sight(nworld_object const* obj_ptr) const;
		bool line_of_sight(const vector3& pos) const;

		bool is_visible() const					{ return m_visible; }
		void set_visible(bool visible)			{ m_visible = visible; }
		void set_aoi_flag(uint8 flag)			{ m_aoi_flag |= flag; }
		void clear_aoi_flag()					{ m_aoi_flag = 0;}
		uint8 get_aoi_flag() const				{ return m_aoi_flag; }

		uint16 move(const vector3& pos, const vector3& rotation, const vector3& acceleration, float time_stamp, uint8 flag);
		virtual uint16 check_move(bool only_turn)			{ return true; }

		void stop_move();
		void set_orient_to(const vector3& pos);

		void _on_pos_change(const vector3& pos);

	private:
		uint16 _can_move(const vector3& pos, const vector3& rotation, const vector3& acceleration, float time_stamp, uint8 flag, bool move, bool turn);

	private:
		uint64						m_guid;								// id
		uint32						m_map_id;							// 地图类型id
		uint32						m_instance_id;						// 副本实例id
		nmap*						m_map_ptr;							// unit所属地图

		nmove_controller			m_move_controller;					// 移动控制器
		bool						m_client_move;						// 客户端请求移动
		float						m_time_stamp;						// 客户端请求移动的时间戳
		uint8						m_custom_flag;						// 移动特殊标志位

		uint8						m_aoi_flag;							// 计算aoi出入的帮助flag
		bool						m_visible;							// 是否可见
		uint32						m_tile_id;							// 所在tile id不用保存坐标映射，浮点误差会导致删除失败crush

		vector3						m_tile_pos;							// 对象在aoi tile中的坐标, 用于同步广播

		nevent_processor			m_event_processor;					// 事件处理器
	};

} // namespace nexus

#endif // _NGAME_NWORLD_OBJECT_H_
