/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_RESOURCE_H_
#define _NEXUS_RESOURCE_H_
#include "ncore.h"

namespace nexus
{
	struct resource_location
	{
		nstring	pkg_name;
		nstring	file_name;

		bool is_valid() const
		{	
			return !pkg_name.empty()
				&& !file_name.empty();
		}
		resource_location()	{}
		// pkg:file
		explicit resource_location(const nstring& str)
		{
			nstring::size_type f = str.find(_T(':'));
			if( f != nstring::npos )
			{
				pkg_name = str.substr(0, f);
				file_name = str.substr(f+1);
			}
		}

		resource_location(const nstring& pkg, const nstring& file):pkg_name(pkg),file_name(file)
		{}

		nstring to_string() const
		{
			nstring ret = pkg_name;
			ret += _T(':');
			ret += file_name;
			return ret;
		}
	};

	template<>
	inline narchive& nserialize(narchive& ar, resource_location& obj, const TCHAR* obj_name)
	{
		nstring class_name(_T("resource_location"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, obj.pkg_name, _T("pkg_name"));
		nserialize(ar, obj.file_name, _T("file_name"));
		ar.object_end();

		return ar;
	}

	/**
	 *	引擎资源基类
	 *	@remark 一个资源通常对应一个磁盘文件,例如贴图, mesh等
	 *	@remark 【线程安全】
	*/
	class nAPI nresource :
		public nobject
	{
	public:
		typedef boost::intrusive_ptr<nresource> ptr;

		explicit nresource(const nstring& name_str) : m_ref_count(0), m_name(name_str)
		{}
		virtual ~nresource(void)
		{}

		const resource_location& get_location() const	{	return m_location; }
		const nname& get_name() const					{	return m_name;}
		const nstring& get_name_str() const				{	return m_name.name_str;}
		void add_ref()	{	m_ref_count++; }
		void release();

		virtual void load_from_file(const resource_location& loc)
		{
			m_location = loc;
			do_serialize_io(loc, EFileRead);
		}
		virtual void save_to_file(const resource_location& loc)
		{
			do_serialize_io(loc, EFileWrite);
		}

		virtual bool ready() const	{	return false; }

		virtual size_t get_system_memory_size() const	{	return 0; }
		virtual size_t get_render_memory_size() const	{	return 0; }

		virtual void serialize(narchive& ar);

	protected:
		void do_serialize_io(const resource_location& loc, enum EFileMode fmode);

		int		m_ref_count;
		nname	m_name;
		resource_location	m_location;

		nDECLARE_VIRTUAL_CLASS(nresource)
	};
}//namespace nexus

namespace boost
{
	inline void intrusive_ptr_add_ref( nexus::nresource* res )
	{
		res->add_ref();
	}

	inline void intrusive_ptr_release( nexus::nresource* res )
	{
		res->release();
	}
}

#endif //_NEXUS_RESOURCE_H_