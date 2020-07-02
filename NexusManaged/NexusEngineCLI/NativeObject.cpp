#include "stdafx.h"
#include "NativeObject.h"

namespace NexusEngine
{
	nexus::nobject* INativeObj::ConstructObject(System::String^ nativeClassName)
	{
		pin_ptr<const wchar_t> szClassName = PtrToStringChars(nativeClassName);
		return nclass_factory::instance()->alloc_object(szClassName);
	}

	//-- class NativeObjectBorrowed -------------------------------------------------------
	NativeObjectBorrowed::NativeObjectBorrowed(nexus::nobject* native_obj):m_obj(native_obj)
	{}

	nexus::nobject* NativeObjectBorrowed::RawPtr::get()
	{
		return m_obj;
	}

	nexus::nobject* NativeObjectBorrowed::NativePtr::get()
	{
		return m_obj;
	}

	//-- class NativeObjectOwnedBySharedPtr -------------------------------------------------------
	NativeObjectOwnedBySharedPtr::NativeObjectOwnedBySharedPtr(const boost::shared_ptr<nexus::nobject>& native_obj)
	{
		m_smart_ptr = new boost::shared_ptr<nexus::nobject>( native_obj );
	}
	
	NativeObjectOwnedBySharedPtr::~NativeObjectOwnedBySharedPtr()
	{
		delete m_smart_ptr;
		m_smart_ptr = NULL;
	}

	nexus::nobject* NativeObjectOwnedBySharedPtr::RawPtr::get()
	{
		if( m_smart_ptr )				
			return m_smart_ptr->get();
		else
			return NULL;
	}

	nexus::nobject* NativeObjectOwnedBySharedPtr::NativePtr::get()
	{
		return RawPtr;
	}

	boost::shared_ptr<nexus::nobject>* NativeObjectOwnedBySharedPtr::SharedPtr::get()
	{
		return m_smart_ptr;
	}

	//-- class NativeObjectOwnedRaw -------------------------------------------------------
	NativeObjectOwnedRaw::NativeObjectOwnedRaw(nexus::nobject* obj):m_obj(obj)
	{}
	
	NativeObjectOwnedRaw::~NativeObjectOwnedRaw()
	{
		delete m_obj;
		m_obj = 0;
	}

	nexus::nobject* NativeObjectOwnedRaw::RawPtr::get()
	{
		return m_obj;
	}

	nexus::nobject* NativeObjectOwnedRaw::NativePtr::get()
	{
		return m_obj;
	}
}//namespace NexusCore