/**
 *	nexus nperfmon_test - nworld_object
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	May, 2010
 */

#ifndef _NPERFMON_TEST_NWORLD_OBJECT_H_
#define _NPERFMON_TEST_NWORLD_OBJECT_H_

#include "ncommon.h"
#include "nobject_define.h"
#include "ngameframework.h"
#include "base_define.h"
#include "math/vector.h"

using namespace gameframework;

namespace nexus {

	class nmap;

	class nworld_object : private nnoncopyable
	{
	public:
		nworld_object();
		virtual ~nworld_object();

		bool init(uint32 map_id, uint32 instance_id, const vector3& pos, const vector3& rot);
		virtual void destroy();
		virtual void update(uint32 elapse);

		uint64 get_id() const					{ return m_guid; }
		uint8 get_type() const					{ return GET_TYPE(m_guid); }
		uint32 get_map_id() const				{ return m_map_id; }
		uint32 get_instance_id() const			{ return m_instance_id; }
		nmap* get_map() const					{ return m_map_ptr; }
		const vector3& get_position() const		{ return m_controller_ptr->get_position(); }
		const vector3& get_rotation() const		{ return m_controller_ptr->get_rotation(); }
		const vector3& get_acceleration() const	{ return m_controller_ptr->get_acceleration(); }
		uint8 get_movement_type() const			{ return m_controller_ptr->get_current_movement_type(); }
		uint8 get_movement_flag() const			{ return gameframework::nsavedmove::compressed_controller_flags(m_controller_ptr.get()); }

		void set_id(uint64 guid)				{ m_guid = guid; }
		void set_map_id(uint32 map_id)			{ m_map_id = map_id; }
		void set_instance_id(uint32 instance_id){ m_instance_id = instance_id; }
		void set_map(nmap* map_ptr)				{ m_map_ptr = map_ptr; }
		void set_position(const vector3& pos) 	{ m_controller_ptr->set_position(pos); }
		void set_rotation(const vector3& rot) 	{ m_controller_ptr->set_rotation(rot); }
		void set_acceleration(const vector3& ac){ m_controller_ptr->set_acceleration(ac); }
		void set_movement_type(uint8 move_type) { m_controller_ptr->set_current_movement_type(gameframework::EMovementType(move_type)); }

		uint16 move(const vector3& pos, const vector3& rotation, const vector3& acceleration, uint32 time_stamp, uint8 flag);

	protected:
		ncharacter_controller::ptr	m_controller_ptr;

	private:
		uint64						m_guid;								// 对象guid
		uint32						m_map_id;							// 地图类型id
		uint32						m_instance_id;						// 副本实例id
		nmap*						m_map_ptr;							// unit所属地图
	};

} // namespace nexus

#endif // _NPERFMON_TEST_NWORLD_OBJECT_H_
