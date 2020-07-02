#include "StdAfx.h"
#include "NResourceLocEditor.h"
#include "NResource.h"
#include "NEditorEngine.h"

using namespace System::Windows::Forms::Design;

namespace NexusEngine
{
	namespace Design
	{
		NResourceLocEditor::NResourceLocEditor(void)
		{
		}

		NResourceLocEditor::~NResourceLocEditor(void)
		{
		}

		System::Object^ NResourceLocEditor::EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value)
		{
			// Return the value if the value is not of type NResourceLoc
			if( value->GetType() != NResourceLoc::typeid )
				return value;

			//-- 这里省去了显示Dropdown user control,直接使用引擎的当前资源路径
			NEditorEngine^ eng = (NEditorEngine^)NEditorEngine::Instance();
			NResourceLoc curRes = eng->GetCurrentResourceLoc();
			
			if( curRes.IsValid() )
				return gcnew NResourceLoc(curRes.pkgName, curRes.fileName);
			else
				return value;
		}
	}
}