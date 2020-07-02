#pragma once

using namespace System::Drawing::Design;
using namespace System::ComponentModel;

namespace NexusEngine
{
	namespace Design
	{
		ref class NResourceLocEditor : public UITypeEditor
		{
		public:
			NResourceLocEditor(void);
			virtual ~NResourceLocEditor(void);

			virtual UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext^ context) override
			{
				return UITypeEditorEditStyle::DropDown;
			}

			virtual System::Object^ EditValue(ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) override;		
		};
	}
}