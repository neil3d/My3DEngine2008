/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D9_TEXTURE_H_
#define _NEXUS_D3D9_TEXTURE_H_
#include "d3d_ptr.h"

namespace nexus
{
	class d3d9_texture2D
		: public nrender_texture2D
	{
	public:
		d3d9_texture2D(void);
		virtual ~d3d9_texture2D(void);

		virtual void load_from_file(nfile_system* fs, const nstring& pkg_name, const nstring& file_name);
		virtual void* get_handle() const
		{
			return (void*)(m_texture.get());
		}

	private:
		d3d_texture_ptr	m_texture;
	};

	class d3d9_heightmap
		: public nrender_heightmap
	{
	public:
		d3d9_heightmap(void);
		virtual ~d3d9_heightmap(void);

		virtual void create(size_t w, size_t h);
		virtual void copy_heightmap(const nrect& rc, nheight_map16* src);
		virtual void update_rect(const nrect& rc, nheight_map16* src);
		virtual nsize get_size() const;

		virtual void* get_handle() const
		{
			return (void*)(m_texture.get());
		}
	private:
		d3d_texture_ptr	m_texture;
	};

	class d3d9_alphamap
		: public nrender_alphamap
	{
	public:
		d3d9_alphamap(void);
		virtual ~d3d9_alphamap(void);

		virtual void create(size_t w, size_t h);
		virtual void copy_alpha(size_t channel, const nrect& rc, nalpha_map* src);
		nsize get_size() const;

		virtual void* get_handle() const
		{
			return (void*)(m_texture.get());
		}
	private:
		d3d_texture_ptr	m_texture;
	};
}//namespace nexus

#endif //_NEXUS_D3D9_TEXTURE_H_