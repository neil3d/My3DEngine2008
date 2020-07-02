/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_D3D_PTR_H_
#define _NEXUS_D3D_PTR_H_

#include "ncore.h"

namespace nexus
{
	template<typename T>
	struct d3d_ptr
	{
	private:
		T*	m_com_obj;	// 调用Com的IUnknow接口,管理对象生存期

	public:
		d3d_ptr() : m_com_obj(NULL)	{}
		d3d_ptr(T* ptr) : m_com_obj(ptr)	{}
		d3d_ptr(const d3d_ptr& other)
		{	
			m_com_obj = other.m_com_obj;
			if(m_com_obj)
				m_com_obj->AddRef();
		}
		~d3d_ptr()
		{	
			if(m_com_obj)
			{
				m_com_obj->Release();				
			}
			m_com_obj = NULL;
		}

		const d3d_ptr& operator=(const d3d_ptr& other)
		{
			if(m_com_obj)
				m_com_obj->Release();

			m_com_obj = other.m_com_obj;
			if(m_com_obj)
				m_com_obj->AddRef();

			return *this;
		}

		bool operator ! () const
		{
			return m_com_obj == NULL;
		}

		operator bool() const
		{	return m_com_obj!=NULL;}

		bool operator == (const d3d_ptr& other)
		{
			return m_com_obj == other.m_com_obj;
		}

		int reset()	
		{	 
			int count =0;
			if( m_com_obj )
			{
				count = m_com_obj->Release();
			}
			m_com_obj = NULL;
			return count;
		}

		void reset( T* obj )
		{
			reset();
			m_com_obj = obj;
		}

		T* operator ->() const
		{	return m_com_obj; }

		T& operator*() const
		{	return *m_com_obj;}

		T* get() const
		{	return m_com_obj; }
	};

	typedef d3d_ptr<IDirect3DDevice9>			d3d_device_ptr;
	typedef d3d_ptr<IDirect3DVertexBuffer9>		d3d_vb_ptr;
	typedef d3d_ptr<IDirect3DIndexBuffer9>		d3d_index_ptr;
	typedef d3d_ptr<ID3DXEffect>				d3d_effect_ptr;
	typedef d3d_ptr<IDirect3DTexture9>			d3d_texture_ptr;
	typedef d3d_ptr<IDirect3DCubeTexture9>		d3d_cube_texture_ptr;
    typedef d3d_ptr<IDirect3DVolumeTexture9>	d3d_volume_texture_ptr;
	typedef d3d_ptr<IDirect3DSurface9>			d3d_surface_ptr;
	typedef d3d_ptr<ID3DXSprite>				d3d_sprite_ptr;
	typedef d3d_ptr<ID3DXFont>					d3d_font_ptr;
}//namespace nexus

#endif //_NEXUS_D3D_PTR_H_