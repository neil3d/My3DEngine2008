#pragma once
#include "NResource.h"
#include "NEffectElement.h"

namespace NexusEngine
{
	public ref class NResourceSpecialEffect : public NResource
	{
	public:
		void AddElement(NEffectElement^ newElement);
		void RemoveElement(String^ elementName);
		void Clear();
		int  ElementCount(){return m_elements->Count;}
		NEffectElement^ GetElement(int index){return m_elements[index];}
		void LoadFromFile(NResourceLoc loc);
	protected:
		Generic::List<NEffectElement^>^	m_elements;

		//--
	public:
		explicit NResourceSpecialEffect(nresource_special_effect::ptr nativeRes);
		nresource_special_effect::ptr GetNativeSmartPtr()
		{
			return boost::dynamic_pointer_cast<nresource_special_effect>(*m_nativeResPtr);
		}
	protected:
		property nresource_special_effect* NativePtr
		{
			nresource_special_effect* get();
		}
	};
}//namespace NexusEngine