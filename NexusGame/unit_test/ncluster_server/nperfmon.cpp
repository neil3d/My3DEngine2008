#include "nperfmon.h"

#include <windows.h>
#include <iostream>

namespace nexus {

	nperfmon::nperfmon() : m_total_send_bytes(0), m_total_recv_bytes(0), m_total_send_packet(0), m_total_recv_packet(0),
		m_send_bytes(0), m_recv_bytes(0), m_send_packet(0), m_recv_packet(0), m_send_error(0), m_push_error(0), m_msg_error(0), 
		m_avg_lag(0), m_max_lag(0), m_lag_1000(0), m_lag_1500(0), m_lag_2000(0), m_tick_msg(0), m_tick_msg_proc_time(0), m_max_tick_msg(0), m_tick(0), m_session(0), m_connect(0), m_msg_overflow(0), m_elaspe_time(0)
	{
	}

	void nperfmon::send(uint32 size)
	{
		m_total_send_bytes += size;
		++m_total_send_packet;

		m_send_bytes += size;
		++m_send_packet;
	}

	void nperfmon::recv(uint32 size)
	{
		m_total_recv_bytes += size;
		++m_total_recv_packet;

		m_recv_bytes += size;
		++m_recv_packet;
	}

	void nperfmon::send_error()
	{
		++m_send_error;
	}

	void nperfmon::push_error()
	{
		++m_push_error;
	}

	void nperfmon::msg_error()
	{
		++m_msg_error;
	}


	void nperfmon::lag(uint32 lag)
	{
		if (lag > m_max_lag)
		{
			m_max_lag = lag;
		}

		if (lag > 1000)
		{
			++m_lag_1000;
		}

		if (lag > 1500)
		{
			++m_lag_1500;
		}

		if (lag > 2000)
		{
			++m_lag_2000;
		}

		m_avg_lag += lag;
	}

	void nperfmon::msg_proc(uint32 msg_count, uint32 proc_time)
	{
		m_tick_msg += msg_count;
		m_tick_msg_proc_time += proc_time;
		++m_tick;

		if (msg_count > m_max_tick_msg)
		{
			m_max_tick_msg = msg_count;
		}
	}

	void nperfmon::msg_overflow()
	{
		++m_msg_overflow;
	}

	void nperfmon::session_mod(bool plus)
	{
		if (plus)
		{
			::InterlockedIncrement((LPLONG)&m_session);
		}
		else
		{
			::InterlockedDecrement((LPLONG)&m_session);
		}
	}

	void nperfmon::connect()
	{
		++m_connect;
	}

	void nperfmon::update(uint32 diff)
	{
		m_elaspe_time += diff;

		uint32 i = 0;
	
		if (m_elaspe_time > 1000)
		{
			system("cls");

			std::cout << "send bytes = " << m_total_send_bytes << std::endl;
			std::cout << "recv bytes = " << m_total_recv_bytes << std::endl << std::endl;

			std::cout << "send packet = " << m_total_send_packet << std::endl;
			std::cout << "recv packet = " << m_total_recv_packet << std::endl << std::endl;

			std::cout << "send bytes /s = " << uint32(float(m_send_bytes * 1000) / float(m_elaspe_time)) << std::endl;
			std::cout << "recv bytes /s = " << uint32(float(m_recv_bytes * 1000) / float(m_elaspe_time)) << std::endl << std::endl;

			std::cout << "send packet /s = " << uint32(float(m_send_packet * 1000) / float(m_elaspe_time)) << std::endl;
			std::cout << "recv packet /s = " << uint32(float(m_recv_packet * 1000) / float(m_elaspe_time)) << std::endl << std::endl;

			std::cout << "send error = " << m_send_error << std::endl;
			std::cout << "push error = " << m_push_error << std::endl;
			std::cout << "msg error = " << m_msg_error << std::endl << std::endl;

				std::cout << "max lag = " << m_max_lag << std::endl;
				std::cout << "avg lag = " << uint32(float(m_avg_lag * 1000) / float(m_elaspe_time * m_recv_packet)) << std::endl << std::endl;
			std::cout << "lag > 1000 = " << m_lag_1000 << std::endl;
			std::cout << "lag > 1500 = " << m_lag_1500 << std::endl;
			std::cout << "lag > 2000 = " << m_lag_2000 << std::endl;

			if (0 == m_tick)
			{
				std::cout << "tick msg = " << 0 << std::endl;
				std::cout << "tick time = " << 0 << std::endl;
			}
			else
			{
				std::cout << "tick msg = " << m_tick_msg / m_tick << std::endl;
				std::cout << "tick time = " << m_tick_msg_proc_time / m_tick << std::endl;
			}

			std::cout << "max tick msg = " << m_max_tick_msg << std::endl;

			std::cout << "msg overflow = " << m_msg_overflow << std::endl << std::endl;

			std::cout << "connect = " << m_connect << std::endl;
			std::cout << "session num = " << m_session << std::endl;

			m_send_bytes	= 0;
			m_recv_bytes	= 0;
			m_send_packet	= 0;
			m_recv_packet	= 0;
			m_elaspe_time	= 0;
			m_avg_lag		= 0;
			m_max_lag		= 0;

			m_tick_msg		= 0;
			m_tick_msg_proc_time = 0;
			m_tick			= 0;

			if (++i == 20)
			{
				m_max_lag		= 0;
				m_max_tick_msg = 0;
				m_lag_1000		= 0;
			}
		}
	}

	nperfmon g_nperfmon;

} // namespace nexus