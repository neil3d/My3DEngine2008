#pragma once
#include "ThumbnailCreator.h"

namespace NexusEngine
{
	ref class MeshThumbnailCreator : public ThumbnailCreator
	{
	public:
		MeshThumbnailCreator(void);

		property System::Collections::ArrayList^ AssociatedFileExtension
		{
			virtual System::Collections::ArrayList^ get() override
			{
				return m_fileExtList;
			}
		}

		virtual void Init() override;
		virtual HBITMAP CreateThumbnail(NResourceLoc loc, int w, int h) override;
		virtual void Close() override;

	private:	
		HDC	m_hdc;
		System::Collections::ArrayList^	m_fileExtList;		
	};
}//namespace NexusEngine