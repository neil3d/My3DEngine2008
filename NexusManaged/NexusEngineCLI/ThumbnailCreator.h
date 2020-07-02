#pragma once
#include "NResource.h"

namespace NexusEngine
{
	ref class ImageThumbnailCreator;

	ref class ThumbnailCreator abstract
	{
	public:
		ThumbnailCreator(void)	{}
		virtual ~ThumbnailCreator(void)	{}

		// string list
		property System::Collections::ArrayList^ AssociatedFileExtension
		{
			virtual System::Collections::ArrayList^ get() =0;
		}

		virtual void Init() = 0;
		virtual HBITMAP CreateThumbnail(NResourceLoc loc, int w, int h) = 0;
		virtual void Close() = 0;
	};

	ref class ThumbnailCreatorManager
	{
	public:
		ThumbnailCreatorManager(void)	
		{
			m_fallback = NULL;
		}
		virtual ~ThumbnailCreatorManager(void)	{}

		void Init();
		void Close();
		
		HBITMAP CreateThumbnail(NResourceLoc loc, int w, int h);		
		HBITMAP LoadTextureAsBitmap(NResourceLoc loc);
	private:
		void AddThumbnailCreator(ThumbnailCreator^ thumb);
	private:
		System::Collections::Generic::List<ThumbnailCreator^>^	m_creatorList;
		System::Collections::Generic::Dictionary<String^, ThumbnailCreator^>^ m_fileExtDict;

		ImageThumbnailCreator^	m_imgCreator;
		
		HBITMAP	m_fallback;
		HBITMAP	m_error;
		HBITMAP m_fileTypeXML;		
		HBITMAP m_fileTypeAnimSet;
	};
}//namespace NexusEngine