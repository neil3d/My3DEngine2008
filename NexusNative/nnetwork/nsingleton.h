/**
 *	nexus network - nsingleton
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Mar, 2010
 */

#ifndef _NNETWORK_NSINGLETON_H_
#define _NNETWORK_NSINGLETON_H_

#include "nmutex.h"
#include "nguard.h"

#include <boost/scoped_ptr.hpp>

namespace nexus {

	/**
	 *	nsingleton thread safe
	 */
	template<typename T>
	class nsingleton
	{
	public:
		NINLINE static T& instance()
		{
			if (!m_instance_ptr)
			{
				// double-checked Locking pattern
				nguard<nfast_mutex> guard(m_mutex);

				if (!m_instance_ptr)
				{
					m_instance_ptr.reset(new T);
				}
			}

			return *m_instance_ptr;
		}

	protected:
		nsingleton(void) {}
		~nsingleton(void) {}

	private:
		// noncopyable
		nsingleton(const nsingleton&);
		const nsingleton& operator=(const nsingleton&);

	private:
		static nfast_mutex				m_mutex;
		static boost::scoped_ptr<T>		m_instance_ptr;
	};

	template<typename T> nfast_mutex nsingleton<T>::m_mutex;
	template<typename T> boost::scoped_ptr<T> nsingleton<T>::m_instance_ptr(NULL);

} // namespace nexus

#endif _NNETWORK_NSINGLETON_H_
