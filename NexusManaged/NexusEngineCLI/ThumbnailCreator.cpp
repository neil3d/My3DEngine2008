#include "StdAfx.h"
#include "ThumbnailCreator.h"
#include "ImageThumbnailCreator.h"
#include "MeshThumbnailCreator.h"
#include "NWrapperException.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace NexusEngine
{
	void ThumbnailCreatorManager::Init()
	{
		//-- create containers
		m_creatorList = gcnew List<ThumbnailCreator^>();
		m_fileExtDict = gcnew Dictionary<String^, ThumbnailCreator^>();

		//--
		ImageThumbnailCreator^ imgThumb = gcnew ImageThumbnailCreator();
		MeshThumbnailCreator^ meshThumb = gcnew MeshThumbnailCreator();
		AddThumbnailCreator(imgThumb);
		AddThumbnailCreator(meshThumb);

		m_fallback = imgThumb->CreateThumbnail(
			NResourceLoc(_T("engine_data"),_T("editor_res/nlogo.bmp")),
			256, 256
			);
		m_fileTypeXML = imgThumb->CreateThumbnail(
			NResourceLoc(_T("engine_data"),_T("editor_res/FileType_XML.png")),
			256, 256
			);
	}

	void ThumbnailCreatorManager::AddThumbnailCreator(ThumbnailCreator^ thumb)
	{
		thumb->Init();
		m_creatorList->Add(thumb);
		
		ArrayList^ fileExtList = thumb->AssociatedFileExtension;
		for (int i=0; i<fileExtList->Count; i++)
		{
			String^ fileExt = (String^)fileExtList[i];
			m_fileExtDict->Add(fileExt, thumb);
		}
	}

	void ThumbnailCreatorManager::Close()
	{
		if( m_fallback )
		{
			::DeleteObject(m_fallback);
			m_fallback = NULL;
		}

		for (int i=0; i<m_creatorList->Count; i++)
		{
			ThumbnailCreator^ thumb = m_creatorList[i];
			thumb->Close();
		}
		m_creatorList->Clear();
		m_fileExtDict->Clear();
	}

	HBITMAP ThumbnailCreatorManager::CreateThumbnail(NResourceLoc loc, int w, int h)
	{
		HBITMAP ret = NULL;

		String^ ext = loc.FileExtension;

		//-- 固定的一些文件类型，直接返回已加载的图片
		if( ext == _T("xml") )
		{
			ret = m_fileTypeXML;
		}
		else
		{
			//-- 搜索对应的类创建缩略图
			if( m_fileExtDict->ContainsKey(loc.FileExtension) )
			{
				ThumbnailCreator^ thumb = m_fileExtDict[loc.FileExtension];
				ret = thumb->CreateThumbnail(loc, w, h);
			}
		}
		
		if( ret == NULL )
		{			
			// return fallback image
			return m_fallback;
		}
		else
		{			
			return ret;
		}
	}
}//namespace NexusEngine