#ifndef _NEXUS_SINGLETON_H_
#define _NEXUS_SINGLETON_H_
#include <boost/thread/once.hpp>
#include <boost/scoped_ptr.hpp>

namespace nexus
{
	/**
	 *	Thread-safe lazy singleton template class 
	 *	@remark http://www.boostcookbook.com/Recipe:/1235044
	*/
	template<typename T>
	class singleton
	{
	public:
		static T* instance()
		{
			boost::call_once(init, m_flag);
			return m_inst.get();
		}

		static void init()
		{
			m_inst.reset(nNew T);
		}
	protected:
		singleton(void)	{	}
		~singleton(void){	}

	private:	// noncopyable
		singleton(const singleton&);
		const singleton& operator = (const singleton&);

	private:
		static boost::scoped_ptr<T>		m_inst;
		static boost::once_flag			m_flag;
	};

	template<class T> boost::scoped_ptr<T> singleton<T>::m_inst(0);
	template<class T> boost::once_flag singleton<T>::m_flag = BOOST_ONCE_INIT;
}//namespace nexus
#endif //_NEXUS_SINGLETON_H_