#pragma once
#include "ThumbnailCreator.h"

namespace NexusEngine
{
	ref class ImageThumbnailCreator : public ThumbnailCreator
	{
	public:
		ImageThumbnailCreator(void);
		virtual ~ImageThumbnailCreator(void);

		property System::Collections::ArrayList^ AssociatedFileExtension
		{
			virtual System::Collections::ArrayList^ get() override
			{
				return m_fileExtList;
			}
		}

		virtual void Init() override;
		virtual HBITMAP CreateThumbnail(NResourceLoc loc, int w, int h) override;
		HBITMAP LoadTextureAsBitmap(NResourceLoc loc);
		virtual void Close() override;

	private:
		void LoadImage(NResourceLoc loc);
	private:
		HDC	m_hdc;
		System::Collections::ArrayList^	m_fileExtList;		
	};
}//namespace NexusEngine