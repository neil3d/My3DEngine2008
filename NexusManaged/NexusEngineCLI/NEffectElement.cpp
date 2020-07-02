#include "StdAfx.h"
#include "NEffectElement.h"
#include "NEffectQuad.h"
#include "NEffectParticleEmitter.h"
#include "NEffectCylinder.h"
#include "NEffectGrid.h"

namespace NexusEngine
{
	NEffectElement^ NEffectElement::CreateEffectElement(String^ nativeClassName, String^ elementName)
	{
		pin_ptr<const wchar_t> szNativeClassName = PtrToStringChars(nativeClassName);
		nexus::neffect_element::ptr new_elem( nconstruct<neffect_element>( szNativeClassName ) );
		if ( !new_elem )
			return nullptr;
		
		NEffectElement^ retObj = nullptr;
		if (nativeClassName == _T("neffect_quad"))
		{
			retObj = gcnew NEffectQuad( boost::dynamic_pointer_cast<neffect_quad>(new_elem) );			
		}
		else if(nativeClassName == _T("neffect_particle_emitter"))
		{
			retObj = gcnew NEffectParticleEmitter( boost::dynamic_pointer_cast<neffect_particle_emitter>(new_elem) );	
		}
		else if(nativeClassName == _T("neffect_cylinder"))
		{
			retObj = gcnew NEffectCylinder( boost::dynamic_pointer_cast<neffect_cylinder>(new_elem) );	
		}
		else if(nativeClassName == _T("neffect_grid"))
		{
			retObj = gcnew NEffectGrid( boost::dynamic_pointer_cast<neffect_grid>(new_elem) );	
		}

		if( retObj!=nullptr )
			retObj->Init(elementName);

		return retObj;
	}

	Collections::ArrayList^ NEffectElement::GetNativeClassNameList()
	{
		Collections::ArrayList^ ret = gcnew Collections::ArrayList();

		const nexus::nclass* root_class = nexus::neffect_element::reflection_get_static_class();
		for ( size_t i=0; i<root_class->get_num_children(); i++)
		{
			const nexus::nclass* child_class = root_class->get_child(i);
			ret->Add( gcnew String(child_class->get_name()) );
		}
		return ret;
	}

	NEffectElement::NEffectElement(neffect_element::ptr nativeObj):NativeObjectOwnedBySharedPtr(nativeObj)
	{
	}

	System::String^ NEffectElement::Name::get()
	{
		return gcnew System::String(NativePtr->get_name().c_str());
	}

	void NEffectElement::Name::set(String^ val)
	{
		pin_ptr<const wchar_t> szVal = PtrToStringChars(val);

		NativePtr->m_name = szVal;
	}

	neffect_element* NEffectElement::NativePtr::get()
	{
		return ndynamic_cast<neffect_element>( m_smart_ptr->get() );
	}

	void NEffectElement::Init(String^ name)
	{
		pin_ptr<const wchar_t> szName = PtrToStringChars(name);
		NativePtr->init(szName);
	}
}//namespace NexusEngine