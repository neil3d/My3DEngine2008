#pragma once

using namespace System::ComponentModel;

namespace NexusEngine
{
	value class Rect;

	namespace Design
	{
		/// <summary>
		/// Provides a type converter to convert <see cref="Rect"/> objects to and from various 
		/// other representations.
		/// </summary>
		public ref class RectConverter : System::ComponentModel::ExpandableObjectConverter
		{
		private:
			System::ComponentModel::PropertyDescriptorCollection^ m_Properties;

		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="RectConverter"/> class.
			/// </summary>
			RectConverter();

			/// <summary>
			/// Returns whether this converter can convert the object to the specified type, using the specified context.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <param name="destinationType">A <see cref="System::Type"/> that represents the type you want to convert to.</param>
			/// <returns><c>true</c> if this converter can perform the conversion; otherwise, <c>false</c>.</returns>
			virtual bool CanConvertTo(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Type^ destinationType) override;

			/// <summary>
			/// Returns whether this converter can convert an object of the given type to the type of this converter, using the specified context.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <param name="sourceType">A System::Type that represents the type you want to convert from.</param>
			/// <returns><c>true</c> if this converter can perform the conversion; otherwise, <c>false</c>.</returns>
			virtual bool CanConvertFrom(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Type^ sourceType) override;

			/// <summary>
			/// Converts the given value object to the specified type, using the specified context and culture information.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <param name="culture">A <see cref="System::Globalization::CultureInfo"/>. If <c>null</c> is passed, the current culture is assumed.</param>
			/// <param name="value">The <see cref="System::Object"/> to convert.</param>
			/// <param name="destinationType">A <see cref="System::Type"/> that represents the type you want to convert to.</param>
			/// <returns>An <see cref="System::Object"/> that represents the converted value.</returns>
			virtual System::Object^ ConvertTo(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Globalization::CultureInfo^ culture, System::Object^ value, System::Type^ destinationType) override;

			/// <summary>
			/// Converts the given object to the type of this converter, using the specified context and culture information.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <param name="culture">A <see cref="System::Globalization::CultureInfo"/>. If <c>null</c> is passed, the current culture is assumed.</param>
			/// <param name="value">The <see cref="System::Object"/> to convert.</param>
			/// <returns>An <see cref="System::Object"/> that represents the converted value.</returns>
			virtual System::Object^ ConvertFrom(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Globalization::CultureInfo^ culture, System::Object^ value) override;

			/// <summary>
			/// Returns whether changing a value on this object requires a call to <c>System::ComponentModel::TypeConverter::CreateInstance(System::Collections::IDictionary^)</c>
			/// to create a new value, using the specified context.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <returns><c>false</c> if changing a property on this object requires a call to <c>System::ComponentModel::TypeConverter::CreateInstance(System::Collections::IDictionary^)</c></returns>
			virtual bool GetCreateInstanceSupported(System::ComponentModel::ITypeDescriptorContext^ context) override;

			/// <summary>
			/// Creates an instance of the type that this <see cref="System::ComponentModel::TypeConverter"/> is associated with, using the specified context, given a set of property values for the object.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <param name="propertyValues">An <see cref="System::Collections::IDictionary"/> of new property values.</param>
			/// <returns>An <see cref="System::Object"/> representing the given <see cref="System:Collections::IDictionary"/>, or <c>null</c> if the object cannot be created.</returns>
			virtual System::Object^ CreateInstance(System::ComponentModel::ITypeDescriptorContext^ context, 
				System::Collections::IDictionary^ propertyValues) override;

			/// <summary>
			/// Returns whether this object supports properties, using the specified context.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <returns><c>true</c> if GetProperties should be called to find the properties of this object; otherwise, <c>false</c>.</returns>
			virtual bool GetPropertiesSupported(System::ComponentModel::ITypeDescriptorContext^ context) override;

			/// <summary>
			/// Creates an instance of the type that this <see cref="System::ComponentModel::TypeConverter"/> is associated with, using the specified context, given a set of property values for the object.
			/// </summary>
			/// <param name="context">A <see cref="System::ComponentModel::ITypeDescriptorContext"/> that provides a format context.</param>
			/// <param name="value">An <see cref="System::Object"/> that specifies the type of array for which to get properties. </param>
			/// <param name="attributes">An array of type <see cref="System::Attribute"/> that is used as a filter.</param>
			/// <returns>A <see cref="System::ComponentModel::PropertyDescriptorCollection"/> with the properties that are exposed for this data type, or a null reference (<c>Nothing</c> in Visual Basic) if there are no properties.</returns>
			virtual System::ComponentModel::PropertyDescriptorCollection^ GetProperties(System::ComponentModel::ITypeDescriptorContext^ context,
				System::Object^ value, array<System::Attribute^>^ attributes ) override;
		};
	}
}