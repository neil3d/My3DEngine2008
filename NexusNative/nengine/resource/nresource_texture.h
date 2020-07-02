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
	class nAPI nresource_texture
		: public nresource
	{
	public:
		typedef boost::intrusive_ptr<nresource_texture> ptr;

		explicit nresource_texture(const nstring& name_str) : nresource(name_str)	{}
		virtual ~nresource_texture(void)	{}

		nrender_texture* get_render_texture()	{	return m_render_tex.get();}
	
	protected:
		render_res_ptr<nrender_texture>	m_render_tex;

		nDECLARE_VIRTUAL_CLASS(nresource_texture)
	};

	/**
	 *	2D贴图资源类
	*/
	class nAPI nresource_texture_2d :
		public nresource_texture
	{
	public:
		typedef boost::intrusive_ptr<nresource_texture_2d> ptr;

		explicit nresource_texture_2d(const nstring& name_str);
		virtual ~nresource_texture_2d(void);

		virtual void load_from_file(const resource_location& loc);		
		
		nrender_texture2D* get_render_texture2d() 
		{
			return static_cast<nrender_texture2D*>(get_render_texture());
		}	

		nDECLARE_NAMED_CLASS(nresource_texture_2d)
	};

	class nAPI nresource_cube_map : public nresource_texture
	{
	public:
		typedef boost::intrusive_ptr<nresource_cube_map> ptr;

		explicit nresource_cube_map(const nstring& name_str);
		virtual ~nresource_cube_map(void);

		virtual void load_from_file(const resource_location& loc);		

		nDECLARE_NAMED_CLASS(nresource_cube_map)
	};
}//namespace nexus
#endif //_NEXUS_RESOURCE_TEXTURE_H_