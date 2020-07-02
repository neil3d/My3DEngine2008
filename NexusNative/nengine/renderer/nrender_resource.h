/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRENDER_RESOURCE_H_
#define _NRENDER_RESOURCE_H_
#include "ncore.h"

namespace nexus
{
	/**
	 *	渲染系统资源
	 *	@remark 被组合到nresource类中
	 *	@sell class nresource
	*/
	class nAPI nrender_resource :
		public nobject
	{
	public:
		nrender_resource(void)	{}
		virtual ~nrender_resource(void)	{}

		void release();

		nDECLARE_VIRTUAL_CLASS(nrender_resource)
	};

	template<typename T>
	struct render_res_array
	{
		typedef std::vector<T*> ArrayType;
		ArrayType	m_array;

		~render_res_array()
		{
			clear();
		}

		render_res_array()	{}

		bool empty() const	{	return m_array.empty(); }
		size_t size() const	{	return m_array.size();}

		void push_back(T* res_ptr)	{	m_array.push_back(res_ptr); }

		T* operator[](size_t i) const
		{
			nASSERT(i < m_array.size());
			return m_array[i];
		}

		void set_at(size_t i, T* res)
		{
			nASSERT(i < m_array.size());
			m_array[i] = res;
		}

		void clear()
		{
			for(ArrayType::iterator iter = m_array.begin();
				iter != m_array.end();
				++iter)
			{
				T* res_ptr = *iter;
				res_ptr->release();
			}
			m_array.clear();
		}

	private:// none copyable
		render_res_array(const render_res_array&);
		const render_res_array& operator = (const render_res_array&);
	};

	template<typename T>
	struct render_res_ptr
	{
		T*	m_res;

		~render_res_ptr()
		{
			if(m_res!=NULL)
				m_res->release();
		}
		render_res_ptr():m_res(NULL)	{}
		render_res_ptr(T* res):m_res(res) {}

		void reset()
		{
			if(m_res!=NULL)
				m_res->release();
			m_res = NULL;
		}

		void reset( T* obj )
		{
			reset();
			m_res = obj;
		}

		T* operator ->() const
		{	return m_res; }

		T& operator*() const
		{	return *m_res;}

		T* get() const
		{	return m_res; }

		T* release()
		{
			T* ret = m_res;
			m_res = NULL;
			return ret;
		}

		operator bool () const
		{	return m_res!=NULL; }


	private:// none copyable
		render_res_ptr(const render_res_ptr&);
		const render_res_ptr& operator = (const render_res_ptr&);
	};
}//namespace nexus
#endif //_NRENDER_RESOURCE_H_