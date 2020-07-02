#include "StdAfx.h"

#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")

#include "ImageThumbnailCreator.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	ImageThumbnailCreator::ImageThumbnailCreator(void)
	{
		m_fileExtList = gcnew System::Collections::ArrayList();
		m_fileExtList->Add("bmp");
		m_fileExtList->Add("jpg");
		m_fileExtList->Add("tga");
		m_fileExtList->Add("png");
		m_fileExtList->Add("dds");		

		m_hdc = NULL;
	}

	ImageThumbnailCreator::~ImageThumbnailCreator(void)
	{
	}

	HBITMAP ImageThumbnailCreator::LoadTextureAsBitmap(NResourceLoc loc)
	{
		LoadImage(loc);		
		return ilutConvertToHBitmap(m_hdc);
	}

	HBITMAP ImageThumbnailCreator::CreateThumbnail(NResourceLoc loc, int w, int h)
	{
		LoadImage(loc);

		iluScale(w, h, 1);
		return ilutConvertToHBitmap(m_hdc);
	}

	void ImageThumbnailCreator::LoadImage(NResourceLoc loc)
	{
		//-- Load File to memory lump
		nexus::resource_location nloc;
		loc.ToNative(nloc);

		nexus::nfile_system* fs = nexus::nengine::instance()->get_file_sys();
		size_t fileSize = fs->get_file_size(nloc.pkg_name, nloc.file_name);
		if(fileSize == 0
			|| fileSize == -1)
		{
			throw gcnew System::IO::FileLoadException("Image File not found or empty");
		}

		nexus::nfile::ptr fp = fs->open_file(nloc.pkg_name, nloc.file_name, EFileRead);
		boost::scoped_ptr<unsigned char> lump( 
			(unsigned char*)malloc(fileSize)
			);
		fp->read_buffer(lump.get(), fileSize);
		
		//-- load image
		ILuint imageName;

		ilGenImages(1, &imageName);
		ilBindImage(imageName);

		if( !ilLoadL(IL_TYPE_UNKNOWN , lump.get(), fileSize) )		
		{
			throw gcnew NexusNativeException("Image file load failed");
		}
	}

	void ImageThumbnailCreator::Init()
	{
		HWND hwnd = static_cast<HWND>( nexus::nengine::instance()->get_engine_config().window_handle );
		HDC wndDC = ::GetDC(hwnd);

		m_hdc = wndDC;
		//m_hdc = ::CreateCompatibleDC(wndDC);

		//::ReleaseDC(hwnd, wndDC);
	}

	void ImageThumbnailCreator::Close()
	{
		HWND hwnd = static_cast<HWND>( nexus::nengine::instance()->get_engine_config().window_handle );
		::ReleaseDC(hwnd, m_hdc);
	}
}//namespace NexusEngine