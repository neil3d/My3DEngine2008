#ifndef _NPERFMON_H_
#define _NPERFMON_H_

#include "ntype_def.h"

namespace nexus {

	class nperfmon
	{
	public:
		nperfmon();
		virtual ~nperfmon() {}

		void send(uint32 size);
		void recv(uint32 size);

		void send_error();
		void push_error();
		void msg_error();

		void lag(uint32 lag);
		void msg_proc(uint32 msg_count, uint32 proc_time);

		void msg_overflow();

		void session_mod(bool plus);
		void connect();

		void update(uint32 diff);

	private:
		volatile uint64		m_total_send_bytes;
		volatile uint64		m_total_recv_bytes;
		volatile uint64		m_total_send_packet;
		volatile uint64		m_total_recv_packet;
		volatile uint32		m_send_bytes;
		volatile uint32		m_recv_bytes;
		volatile uint32		m_send_packet;
		volatile uint32		m_recv_packet;
		volatile uint32		m_send_error;
		volatile uint32		m_push_error;
		volatile uint32		m_msg_error;

		volatile uint32		m_avg_lag;
		volatile uint32		m_max_lag;
		volatile uint32		m_lag_1000;
		volatile uint32		m_lag_1500;
		volatile uint32		m_lag_2000;

		volatile uint32		m_tick_msg;
		volatile uint32		m_tick_msg_proc_time;
		volatile uint32		m_max_tick_msg;
		volatile uint32		m_tick;

		volatile uint32		m_session;
		uint32				m_connect;

		volatile uint32		m_msg_overflow;

		uint32				m_elaspe_time;
	};

	extern nperfmon g_nperfmon;

} // namespace nexus

#endif //_NPERFMON_H_