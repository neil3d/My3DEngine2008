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
	using namespace NexusEngineExtension;

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
	[System::SerializableAttribute]
	[TypeConverter(Design::NResourceLocConverter::typeid)]
	[Editor(Design::NResourceLocEditor::typeid, UITypeEditor::typeid)]
	[XmlClassSerializable("NResourceLoc")]
	public value struct NResourceLoc : System::IEquatable<NResourceLoc>
	{
		[XmlFieldSerializable("pkgName")]
		System::String^ pkgName;
		[XmlFieldSerializable("fileName")]
		System::String^	fileName;

		NResourceLoc(System::String^ pkg, System::String^ file)
		{
			pkgName = pkg;
			fileName = file;
		}

		NResourceLoc(const nexus::resource_location& nativeLoc);

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

		/// <summary>
		/// Tests for equality between two objects.
		/// </summary>
		/// <param name="left">The first value to compare.</param>
		/// <param name="right">The second value to compare.</param>
		/// <returns><c>true</c> if <paramref name="left"/> has the same value as <paramref name="right"/>; otherwise, <c>false</c>.</returns>
		static bool operator == ( NResourceLoc left, NResourceLoc right );

		/// <summary>
		/// Tests for inequality between two objects.
		/// </summary>
		/// <param name="left">The first value to compare.</param>
		/// <param name="right">The second value to compare.</param>
		/// <returns><c>true</c> if <paramref name="left"/> has a different value than <paramref name="right"/>; otherwise, <c>false</c>.</returns>
		static bool operator != ( NResourceLoc left, NResourceLoc right );

		/// <summary>
		/// Returns the hash code for this instance.
		/// </summary>
		/// <returns>A 32-bit signed integer hash code.</returns>
		virtual int GetHashCode() override;

		/// <summary>
		/// Returns a value that indicates whether the current instance is equal to a specified object. 
		/// </summary>
		/// <param name="obj">Object to make the comparison with.</param>
		/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
		virtual bool Equals( System::Object^ obj ) override;

		/// <summary>
		/// Returns a value that indicates whether the current instance is equal to the specified object. 
		/// </summary>
		/// <param name="other">Object to make the comparison with.</param>
		/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
		virtual bool Equals( NResourceLoc other );

		/// <summary>
		/// Determines whether the specified object instances are considered equal. 
		/// </summary>
		/// <param name="value1">The first value to compare.</param>
		/// <param name="value2">The second value to compare.</param>
		/// <returns><c>true</c> if <paramref name="value1"/> is the same instance as <paramref name="value2"/> or 
		/// if both are <c>null</c> references or if <c>value1.Equals(value2)</c> returns <c>true</c>; otherwise, <c>false</c>.</returns>
		static bool Equals( NResourceLoc% value1, NResourceLoc% value2 );
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
		void PostEditChange(bool ready);

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