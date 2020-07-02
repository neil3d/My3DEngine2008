/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RENDER_TEXTURE_H_
#define _NEXUS_RENDER_TEXTURE_H_
#include "nrender_resource.h"
#include "../ncore/height_map/nheight_map.h"
#include "math/rect.h"

namespace nexus
{
	/**
	 *	render hardware texture基类
	*/
	class nAPI nrender_texture :
		public nrender_resource
	{
	public:
		nrender_texture(void)	{}
		virtual ~nrender_texture(void)	{}

		virtual void* get_handle() const = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_texture)
	};

	/**
	 *	render hardware 2D贴图
	*/
	class nAPI nrender_texture2D :
		public nrender_texture
	{
	public:
		nrender_texture2D(void)		{		}
		virtual ~nrender_texture2D(void)		{	}

		virtual void load_from_file(nfile_system* fs, const nstring& pkg_name, const nstring& file_name) = 0;
		virtual const fsize& get_original_size() const = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_texture2D)
	};

	/**
	 *	render hardware cube map
	*/
	class nAPI nrender_cube_map :
		public nrender_texture
	{
	public:
		nrender_cube_map(void)		{		}
		virtual ~nrender_cube_map(void)		{	}

		virtual void load_from_file(nfile_system* fs, const nstring& pkg_name, const nstring& file_name) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_cube_map)
	};

	/**
	 *	用于渲染地形的height map
	*/
	class nAPI nrender_heightmap :
		public nrender_texture
	{
	public:
		nrender_heightmap(void)		{		}
		virtual ~nrender_heightmap(void)		{	}

		virtual void create(size_t w, size_t h) = 0;
		virtual void copy_heightmap(const nrect& rc, nheight_map16* src) = 0;
		virtual void update_rect(const nrect& rc, nheight_map16* src) = 0;
		virtual nsize get_size() const = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_heightmap)
	};

	class nAPI nrender_alphamap :
		public nrender_texture
	{
	public:
		nrender_alphamap(void)	{}
		virtual ~nrender_alphamap(void)	{}

		virtual void create(size_t w, size_t h) = 0;
		virtual void copy_alpha(size_t channel, int src_x, int src_y, nalpha_map* src) = 0;
		virtual void set_alpha(size_t channel, unsigned char val) = 0;

		nDECLARE_VIRTUAL_CLASS(nrender_alphamap)
	};
}//namespace nexus
#endif //_NEXUS_RENDER_TEXTURE_H_