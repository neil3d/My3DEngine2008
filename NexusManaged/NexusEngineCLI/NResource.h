/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NResourceLocEditor.h"

namespace NexusEngine
{
	namespace Design
	{
		//!	提供String到NResourceLoc的互相转换
		public ref class NResourceLocConverter : System::ComponentModel::ExpandableObjectConverter
		{	
		public:
			virtual bool CanConvertTo(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Type^ destinationType) override;
			virtual bool CanConvertFrom(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Type^ sourceType) override;
			virtual System::Object^ ConvertTo(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Globalization::CultureInfo^ culture, System::Object^ value, System::Type^ destinationType) override;
			virtual System::Object^ ConvertFrom(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Globalization::CultureInfo^ culture, System::Object^ value) override;
		};
	}

	//! Resource Location
	[TypeConverter(Design::NResourceLocConverter::typeid)]
	[Editor(Design::NResourceLocEditor::typeid, UITypeEditor::typeid)]
	public value struct NResourceLoc
	{
		System::String^ pkgName;
		System::String^	fileName;

		NResourceLoc(System::String^ pkg, System::String^ file)
		{
			pkgName = pkg;
			fileName = file;
		}

		NResourceLoc(System::String^ loc);
		virtual System::String^ ToString() override;

		void FromNative(const nexus::resource_location& nativeLoc);
		void ToNative(nexus::resource_location& nativeLoc);

		property String^ PackageName
		{
			String^ get()	{	return pkgName;}
			void set(String^ val)	{	pkgName = val;}
		}

		property String^ FileName
		{
			String^ get()	{	return fileName;}
			void set(String^ val)	{	fileName = val;}
		}

		[System::ComponentModel::BrowsableAttribute(false)]
		property String^ FileExtension
		{
			String^ get();
		}

		bool IsValid()
		{
			if( pkgName==nullptr
				|| fileName==nullptr)
				return false;
			if( pkgName->Length <= 0
				|| fileName->Length <= 0)
				return false;
			return true;
		}
	};

	/**
	 *	资源对象基类, 包装nexus::nresource类对象
	*/
	public ref class NResource
	{
	public:
		//!	资源在文件系统中的位置
		property NResourceLoc Location
		{	NResourceLoc get(); }

		//! 资源名称
		property System::String^ Name
		{	System::String^ get(); }

		void LoadFromFile(NResourceLoc loc);
		void SaveToFile(NResourceLoc loc, bool xml);

		//-- Native Wrapper		
	public:
		explicit NResource(nresource::ptr nativeResPtr);
		~NResource(void);
	protected:
		boost::intrusive_ptr<nresource>* m_nativeResPtr;
		property nresource* NativePtr
		{
			nresource* get();
		}
	};
}//namespace NexusEngine