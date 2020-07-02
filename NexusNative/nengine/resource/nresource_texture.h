/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RESOURCE_TEXTURE_H_
#define _NEXUS_RESOURCE_TEXTURE_H_
#include "nresource.h"
#include "../renderer/nrender_texture.h"

namespace nexus
{
	/**
	 *	贴图资源对象基类
	*/
	class nresource_texture
		: public nresource
	{
	public:
		typedef boost::intrusive_ptr<nresource_texture> ptr;

		explicit nresource_texture(const nstring& name_str) : nresource(name_str)	{}
		virtual ~nresource_texture(void)	{}

		nrender_texture* get_render_texture()	{	return m_render_tex.get();}
	
	protected:
		render_res_ptr<nrender_texture>	m_render_tex;
	};

	/**
	 *	2D贴图资源类
	*/
	class nresource_texture_2d :
		public nresource_texture
	{
	public:
		typedef boost::intrusive_ptr<nresource_texture_2d> ptr;

		explicit nresource_texture_2d(const nstring& name_str);
		virtual ~nresource_texture_2d(void);

		virtual void load_from_file(const resource_location& loc);		
		virtual bool ready() const
		{
			// todo
			return true;
		}
		
	};
}//namespace nexus
#endif //_NEXUS_RESOURCE_TEXTURE_H_