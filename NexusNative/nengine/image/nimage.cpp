#include "StdAfx.h"
#include "nimage.h"
#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")
#include "ncore.h"

namespace nexus
{
	void nimage::init_image_sdk()
	{
		// Initialize IL
		ilInit();
		
		//--初始化一些默认状态
		ilEnable(IL_FILE_OVERWRITE);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		// Initialize ILU
		iluInit();

		ilutInit();
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
		if( !ilLoadImage(file_name.c_str()) )
		{
			const TCHAR* il_error = iluErrorString(ilGetError());
			nthrow2(file_name.c_str(), il_error);
		}
	}

	void nimage::save_to_file(const nstring& file_name)
	{
		ilBindImage(m_impl->image_name);
		if( !ilSaveImage(file_name.c_str()) )
		{
			const TCHAR* il_error = iluErrorString(ilGetError());
			nthrow2(file_name.c_str(), il_error);
		}
	}

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
			il_fmt = IL_BGR;
			break;
		case EPF_A8R8G8B8:
			il_fmt = IL_BGRA;
			break;
		default:
			nASSERT( 0 && "unknown pixel format");
		}

		if( !ilConvertImage(il_fmt, IL_UNSIGNED_BYTE) )
		{			
			const TCHAR* il_error = iluErrorString(ilGetError());
			nthrow(il_error);			
		}
	}

	void nimage::create(size_t w, size_t h, EPixelFormat fmt)
	{
		ILint il_fmt = 0;
		unsigned char Bpp = 4;
		switch(fmt)
		{
		case EPF_R8G8B8:
			il_fmt = IL_BGR;
			Bpp = 3;
			break;
		case EPF_A8R8G8B8:
			il_fmt = IL_BGRA;
			Bpp = 4;
			break;
		default:
			nASSERT( 0 && "unknown pixel format");
		}

		//--
		destroy();

		//--
		ilGenImages(1, &m_impl->image_name);
		ilBindImage(m_impl->image_name);

		if( !ilTexImage(w, h, 1, Bpp, il_fmt, IL_UNSIGNED_BYTE, NULL) )
		{
			const TCHAR* il_error = iluErrorString(ilGetError());
			nthrow(il_error);			
		}
	}
}//namespace nexus