#include "StdAfx.h"
#include "nimage.h"
#include <il/il.h>
#include <il/ilu.h>
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")

namespace nexus
{
	void nimage::init_image_sdk()
	{
		// Initialize IL
		ilInit();
		// Initialize ILU
		iluInit();
	}

	void nimage::close_image_sdk()
	{
		ilShutDown();
	}

	struct nimage::impl
	{
		ILuint image_name;

		impl(void):image_name(0)
		{}
	};

	nimage::nimage(void)
		: m_impl( new impl )
	{
	}

	nimage::~nimage(void)
	{
		destroy();
		delete m_impl;
	}

	void nimage::load_from_file(const nstring& file_name)
	{
		destroy();

		//--
		ilGenImages(1, &m_impl->image_name);
		ilBindImage(m_impl->image_name);
		ilLoadImage(file_name.c_str()); 
	}

	void nimage::save_to_file(const nstring& file_name)
	{}

	EPixelFormat _convert_il_format_32(ILint fmt)
	{
		switch(fmt)
		{
		case IL_RGBA:
			return EPF_A8R8G8B8;
		}
		return EPF_None;
	}

	EPixelFormat _convert_il_format_24(ILint fmt)
	{
		switch(fmt)
		{
		case IL_RGB:
			return EPF_R8G8B8;
		}
		return EPF_None;
	}

	EPixelFormat nimage::get_format() const
	{
		ilBindImage(m_impl->image_name);
		ILint bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
		ILint fmt = ilGetInteger(IL_IMAGE_FORMAT);

		switch( bpp )
		{
		case 32:
			return _convert_il_format_32(fmt);		
		case 24:
			return _convert_il_format_24(fmt);		
		}

		return EPF_None;
	}

	size_t nimage::get_width() const
	{
		ilBindImage(m_impl->image_name);
		return ilGetInteger(IL_IMAGE_WIDTH);
	}

	size_t nimage::get_height() const
	{
		ilBindImage(m_impl->image_name);
		return ilGetInteger(IL_IMAGE_HEIGHT);
	}

	void* nimage::get_pixel() const
	{
		ilBindImage(m_impl->image_name);
		return ilGetData();
	}

	void nimage::destroy()
	{
		if( m_impl->image_name )
		{
			ilDeleteImages(1, &m_impl->image_name); 
			m_impl->image_name = 0;
		}
	}

	void nimage::convert_format(EPixelFormat fmt)
	{
		ILint il_fmt = 0;
		switch(fmt)
		{
		case EPF_R8G8B8:
			il_fmt = IL_RGB;
			break;
		case EPF_A8R8G8B8:
			il_fmt = IL_RGBA;
			break;
		default:
			nASSERT( 0 && "unknown pixel format");
		}

		ILboolean ok = ilConvertImage(il_fmt, IL_UNSIGNED_BYTE);
	}
}//namespace nexus