#include "StdAfx.h"
#include "NResourceSpecialEffect.h"
#include "NEffectQuad.h"
#include "NEffectParticleEmitter.h"
#include "NEffectCylinder.h"
#include "NEffectGrid.h"

namespace NexusEngine
{
	NResourceSpecialEffect::NResourceSpecialEffect(nresource_special_effect::ptr nativeRes)
		: NResource(nativeRes)
	{
		m_elements = gcnew Generic::List<NEffectElement^>();
	}

	nresource_special_effect* NResourceSpecialEffect::NativePtr::get()
	{
		return static_cast<nresource_special_effect*>( m_nativeResPtr->get() );
	}

	void NResourceSpecialEffect::AddElement(NEffectElement^ newElement)
	{
		m_elements->Add(newElement);
		NativePtr->add_element( newElement->GetNativeSmartPtr() );
		NativePtr->post_edit_change(true);
	}

	void NResourceSpecialEffect::RemoveElement(String^ elementName)
	{
		pin_ptr<const wchar_t> szElementName = PtrToStringChars(elementName);
		NativePtr->remove_element(szElementName);

		for each(NEffectElement^ elem in m_elements)
		{
			if (elem->Name == elementName)
			{
				m_elements->Remove(elem);
				break;
			}
		}
	}

	void NResourceSpecialEffect::Clear()
	{
		m_elements->Clear();
		NativePtr->clear();
	}

	void NResourceSpecialEffect::LoadFromFile( NResourceLoc loc )
	{
		NResource::LoadFromFile(loc);
		//更新数据,注意类型转换
		for (size_t i=0;i<NativePtr->get_num_element();++i)
		{
			neffect_element::ptr elem=NativePtr->get_element(i);
			String^ name=gcnew String(elem->reflection_get_class()->get_name());
			NEffectElement^ retObj = nullptr;
			if (name == _T("neffect_quad"))
			{
				retObj = gcnew NEffectQuad( boost::dynamic_pointer_cast<neffect_quad>(elem) );			
			}
			else if(name == _T("neffect_particle_emitter"))
			{
				retObj = gcnew NEffectParticleEmitter( boost::dynamic_pointer_cast<neffect_particle_emitter>(elem) );	
			}
			else if(name == _T("neffect_cylinder"))
			{
				retObj = gcnew NEffectCylinder( boost::dynamic_pointer_cast<neffect_cylinder>(elem) );	
			}
			else if(name == _T("neffect_grid"))
			{
				retObj = gcnew NEffectGrid( boost::dynamic_pointer_cast<neffect_grid>(elem) );	
			}

			if( retObj!=nullptr )
			{
				retObj->Init(gcnew String(elem->get_name().c_str()));
				m_elements->Add(retObj);
			}
		}
	}

}//namespace NexusEngine