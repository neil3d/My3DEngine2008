/**
 *	nexus network - nmutex
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Feb, 2010
 */

#ifndef _NNETWORK_NMUTEX_H_
#define _NNETWORK_NMUTEX_H_

#include "ncommon.h"

namespace nexus {

	/**
	 *	nmutex互斥锁
	 */
	class nNET_API nmutex : private nnoncopyable
	{
	public:
		/**
		 * Initializes a mutex class, with InitializeCriticalSection
		 */
		NINLINE nmutex()
		{
			::InitializeCriticalSection(&m_cs);
		}

		/**
		 * Deletes the associated critical section
		 */
		NINLINE ~nmutex()
		{
			::DeleteCriticalSection(&m_cs);
		}

		/**
		 *  Acquires this mutex. If it cannot be acquired immediately, it will block.
		 */
		NINLINE void lock()
		{
			::EnterCriticalSection(&m_cs);
		}

		/**
		 * Releases this mutex. No error checking performed
		 */
		NINLINE void unlock()
		{
			::LeaveCriticalSection(&m_cs);
		}

		/**
		 * Attempts to acquire this mutex. If it cannot be acquired (held by another thread) it will return false.
		 * @return false if cannot be acquired, true if it was acquired.
		 */
		NINLINE bool try_lock()
		{
			return (::TryEnterCriticalSection(&m_cs) == TRUE ? true : false);
		}

	private:
		/** 
		 * Critical section used for system calls
		 */
		::CRITICAL_SECTION	m_cs;
	};

	/**
	 *	nfast_mutex通过原子计数实现的快速互斥锁
	 */
	class nNET_API nfast_mutex : private nnoncopyable
	{
	public:
		NINLINE nfast_mutex() : m_lock(0) {}
		NINLINE ~nfast_mutex() {}

		NINLINE void lock()
		{
			while (::InterlockedCompareExchange((LPLONG)&m_lock, 1, 0) != 0)
			{
				::Sleep(0);
			}
		}

		NINLINE void unlock()
		{
			::InterlockedExchange((LPLONG)(&m_lock), 0);
		}

		NINLINE bool try_lock()
		{
			return ::InterlockedCompareExchange((LPLONG)(&m_lock), 1, 0) == 0;	
		}

	private:
		volatile LONG	m_lock;
	};

	/**
	 *	nfast_mutex通过原子计数实现的递归快速互斥锁
	 */
	class nNET_API nfast_recursive_mutex : private nnoncopyable
	{
	public:
		NINLINE nfast_recursive_mutex() : m_lock(0), m_recursive_count(0) {}
		NINLINE ~nfast_recursive_mutex() {}

		NINLINE void lock()
		{
			DWORD thread_id = ::GetCurrentThreadId();

			if(thread_id == (DWORD)m_lock)
			{
				++m_recursive_count;
				return;
			}

			while (::InterlockedCompareExchange((LPLONG)&m_lock, thread_id, 0) != 0)
			{
				::Sleep(0);
			}

			++m_recursive_count;
		}

		NINLINE void unlock()
		{
			if (0 == (--m_recursive_count))
			{
				::InterlockedExchange((LPLONG)&m_lock, 0);
			}
		}

		NINLINE bool try_lock()
		{
			DWORD thread_id = ::GetCurrentThreadId();

			if (thread_id == (DWORD)m_lock)
			{
				++m_recursive_count;
				return true;
			}

			DWORD owner = ::InterlockedCompareExchange((LPLONG)&m_lock, thread_id, 0);

			if (0 == owner)
			{
				++m_recursive_count;
				return true;
			}

			return false;
		}

	private:
#pragma pack(push,8)
		volatile LONG m_lock;
#pragma pack(pop)
		DWORD m_recursive_count;
	};

	/**
	 *	nnull_mutex空锁
	 */
	class nNET_API nnull_mutex : private nnoncopyable
	{
	public:
		NINLINE nnull_mutex() {}
		NINLINE ~nnull_mutex() {}

		NINLINE void lock() {}
		NINLINE void unlock() {}
		NINLINE bool try_lock() { return true; }
	};

} // namespace nexus

#endif _NNETWORK_NMUTEX_H_
