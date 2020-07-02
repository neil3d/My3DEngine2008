#pragma once
#include "NRenderFont.h"
#include "FontDescription.h"

namespace NexusEngine
{

	public ref class NFontManager sealed
	{
	//-- 定义单件访问接口
	private:
		static NFontManager^ s_instance;
		NFontManager(void);

	public:
		static property NFontManager^ Instance
		{
			NFontManager^ get()
			{
				return s_instance;
			}
		}

		static NFontManager(void)
		{
			s_instance = gcnew NFontManager;
		}

	public:
		virtual ~NFontManager(void);

		/// <summary> 创建指定的字体，如果已经创建则返回创建的字体 </summary>
		NRenderFont^ ConstructFont(FontDescription description);

		/// <summary> 查找字体有没有创建 </summary>
		bool ContainsFont(FontDescription description);

		/// <summary> 移除制定的字体 </summary>
		bool RemoveFont(FontDescription description);

		///
		void ClearFont();
		
	private:
		//-- 保存已经创建的Font
		System::Collections::Generic::Dictionary<FontDescription,NRenderFont^>		m_fontCache;
	};

} // end of namespace NexusEngine

