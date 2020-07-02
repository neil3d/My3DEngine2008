/**
 *	nexus network - nguard
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NGUARD_H_
#define _NNETWORK_NGUARD_H_

#include "ncommon.h"

namespace nexus {

	/**
	 *	nguardËøµÄ°ïÖúÀà
	 */
	template <typename tmutex>
	class nguard : private nnoncopyable
	{
	public:
		/** Constructor acquires the lock.
		 */
		nguard(tmutex& mutex) : m_mutex(mutex)
		{
			m_mutex.lock();
			m_locked = true;
		}

		/** Destructor releases the lock.
		 */
		~nguard()
		{
			if (m_locked)
			{
				m_mutex.unlock();
			}
		}

		/** Explicitly acquire the lock.
		 */
		void lock()
		{
			if (!m_locked)
			{
				m_mutex.lock();
				m_locked = true;
			}
		}

		/** Explicitly release the lock.
		 */
		void unlock()
		{
			if (m_locked)
			{
				m_mutex.unlock();
				m_locked = false;
			}
		}

		/** Test whether the lock is held.
		 */
		bool locked() const
		{
			return m_locked;
		}

		/** Get the underlying mutex.
		 */
		tmutex& get_mutex()
		{
			return m_mutex;
		}

	private:
		/** The underlying mutex.
		 */
		tmutex&	m_mutex;

		/** Whether the mutex is currently locked or unlocked.
		 */
		bool	m_locked;
	};

} // namespace nexus

#endif _NNETWORK_NGUARD_H_
