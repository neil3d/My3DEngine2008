#ifndef _NEXUS_CORE_EVENT_HANDLER_H_
#define _NEXUS_CORE_EVENT_HANDLER_H_
#include <vector>

namespace nexus
{
	/**
	* 使用c++模拟.NET event_delegate机制
	*/
	template<typename _return, typename _arg>
	class nevent_handler
	{
	private:
		// 指定签名基类
		class signature_base
		{
		public:
			virtual ~signature_base() {};
			virtual _return operator()(_arg) = 0;
		};

	public:
		// 绑定类成员函数的事件签名
		template<typename _class>
		class T : public signature_base
		{
			typedef _return (_class::*func)(_arg);

		private:
			// 对象地址
			_class* m_object;
			// 成员函数指针
			func	m_func;

		public:
			T(_class* _object,func _func)
				: m_object(_object),m_func(_func)
			{
			}

			virtual _return operator()(_arg e)
			{
				return (m_object->*m_func)(e);
			}
		};

	public:
		// 静态函数或者全局函数绑定的事件签名
		class S : public signature_base
		{
			typedef _return (*func)(_arg);

		private:
			func m_func;

		public:
			S(func _func) : m_func(_func) { };

			virtual _return operator()(_arg e)
			{
				return m_func(e);
			}
		};

	private:
		// 不允许拷贝构造
		nevent_handler( const nevent_handler&);
		nevent_handler& operator=(const nevent_handler&);

	public:
		nevent_handler() {}

		~nevent_handler()
		{
			event_vector::iterator end = m_ptrs.end();
			for( event_vector::iterator it = m_ptrs.begin();
				it != end; ++it )
			{
				delete (*it);
			}
		}

		nevent_handler& operator+=(signature_base* signature)
		{
			m_ptrs.push_back(signature);
			return *this;
		}

		// 回调，触发所有事件签名
		_return operator() (_arg e)
		{
			event_vector::iterator end = m_ptrs.end();
			for (event_vector::iterator it = m_ptrs.begin();
				it != end; ++it )
			{
				if( (it + 1) == end )
				{
					return (*(*it))(e);
				}
				else
				{
					(*(*it))(e);
				}
			}
		}

	private:
		// 已经签署的事件组
		typedef std::vector<signature_base*> event_vector;
		event_vector m_ptrs;
	};

}
#endif //_NEXUS_CORE_EVENT_HANDLER_H_