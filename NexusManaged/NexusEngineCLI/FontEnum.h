#pragma once

namespace NexusEngine
{
#pragma region Enum
	/// <summary>
	/// Specifies weights for font rendering.
	/// </summary>
	/// <unmanaged href="ms901140">FW</unmanaged>
	public enum class FontWeight : System::Int32
	{
		/// <summary>
		/// The font weight doesn't matter.
		/// </summary>
		DoNotCare = FW_DONTCARE,

		/// <summary>
		/// Make the font thin.
		/// </summary>
		Thin = FW_THIN,

		/// <summary>
		/// Make the font extra light.
		/// </summary>
		ExtraLight = FW_EXTRALIGHT,

		/// <summary>
		/// Make the font ultra light.
		/// </summary>
		UltraLight = FW_ULTRALIGHT,

		/// <summary>
		/// Make the font light.
		/// </summary>
		Light = FW_LIGHT,

		/// <summary>
		/// Use a normal weight.
		/// </summary>
		Normal = FW_NORMAL,

		/// <summary>
		/// Use a regular weight.
		/// </summary>
		Regular = FW_REGULAR,

		/// <summary>
		/// Use a medium weight.
		/// </summary>
		Medium = FW_MEDIUM,

		/// <summary>
		/// Use a semi-bold weight.
		/// </summary>
		SemiBold = FW_SEMIBOLD,

		/// <summary>
		/// Use a demi-bold weight.
		/// </summary>
		DemiBold = FW_DEMIBOLD,

		/// <summary>
		/// Use a bold weight.
		/// </summary>
		Bold = FW_BOLD,

		/// <summary>
		/// Use an extra bold weight.
		/// </summary>
		ExtraBold = FW_EXTRABOLD,

		/// <summary>
		/// Use an ultra bold weight.
		/// </summary>
		UltraBold = FW_ULTRABOLD,

		/// <summary>
		/// Use a heavy weight.
		/// </summary>
		Heavy = FW_HEAVY,

		/// <summary>
		/// Use a black weight.
		/// </summary>
		Black = FW_BLACK,
	};

	/// <summary>
	/// Defines possible character sets for fonts.
	/// </summary>
	/// <unmanaged>CHARSET</unmanaged>
	public enum class CharacterSet : System::Int32
	{
		/// <summary>
		/// The ANSI character set.
		/// </summary>
		Ansi = ANSI_CHARSET,

		/// <summary>
		/// The default system character set.
		/// </summary>
		Default = DEFAULT_CHARSET,

		/// <summary>
		/// The symbol character set.
		/// </summary>
		Symbol = SYMBOL_CHARSET,

		/// <summary>
		/// The ShiftJIS character set.
		/// </summary>
		ShiftJIS = SHIFTJIS_CHARSET,

		/// <summary>
		/// The Hangul character set.
		/// </summary>
		Hangul = HANGUL_CHARSET,

		/// <summary>
		/// The GB2312 character set.
		/// </summary>
		GB2312 = GB2312_CHARSET,

		/// <summary>
		/// The Chinese character set.
		/// </summary>
		ChineseBig5 = CHINESEBIG5_CHARSET,

		/// <summary>
		/// The OEM character set.
		/// </summary>
		Oem = OEM_CHARSET,

		/// <summary>
		/// The Johab character set.
		/// </summary>
		Johab = JOHAB_CHARSET,

		/// <summary>
		/// The Hebrew character set.
		/// </summary>
		Hebrew = HEBREW_CHARSET,

		/// <summary>
		/// The Arabic character set.
		/// </summary>
		Arabic = ARABIC_CHARSET,

		/// <summary>
		/// The Greek character set.
		/// </summary>
		Greek = GREEK_CHARSET,

		/// <summary>
		/// The Turkish character set.
		/// </summary>
		Turkish = TURKISH_CHARSET,

		/// <summary>
		/// The Vietnamese character set.
		/// </summary>
		Vietnamese = VIETNAMESE_CHARSET,

		/// <summary>
		/// The Thai character set.
		/// </summary>
		Thai = THAI_CHARSET,

		/// <summary>
		/// The East Europe character set.
		/// </summary>
		EastEurope = EASTEUROPE_CHARSET,

		/// <summary>
		/// The Russian character set.
		/// </summary>
		Russian = RUSSIAN_CHARSET,

		/// <summary>
		/// The Baltic character set.
		/// </summary>
		Baltic = BALTIC_CHARSET,

		/// <summary>
		/// The Mac character set.
		/// </summary>
		Mac = MAC_CHARSET
	};

	/// <summary>
	/// Specifies quality options for font rendering.
	/// </summary>
	/// <unmanaged href="ms901140">QUALITY</unmanaged>
	public enum class FontQuality : System::Int32
	{
		/// <summary>
		/// Default quality levels.
		/// </summary>
		Default = DEFAULT_QUALITY,

		/// <summary>
		/// Draft quality.
		/// </summary>
		Draft = DRAFT_QUALITY,

		/// <summary>
		/// Proof quality.
		/// </summary>
		Proof = PROOF_QUALITY,

		/// <summary>
		/// Non-antialiased quality.
		/// </summary>
		NonAntialiased = NONANTIALIASED_QUALITY,

		/// <summary>
		/// Antialiased quality.
		/// </summary>
		Antialiased = ANTIALIASED_QUALITY,

		/// <summary>
		/// Clear type quality.
		/// </summary>
		ClearType = CLEARTYPE_QUALITY,

		/// <summary>
		/// Clear type natural quality.
		/// </summary>
		ClearTypeNatural = CLEARTYPE_NATURAL_QUALITY,
	};

	/// <summary>
	/// Defines pitch and family settings for fonts.
	/// </summary>
	/// <unmanaged href="ms901140">(various font constants)</unmanaged>
	[System::Flags]
	public enum class PitchAndFamily : System::Int32
	{
		/// <summary>
		/// Default pitch.
		/// </summary>
		Default = DEFAULT_PITCH,

		/// <summary>
		/// Fixed pitch.
		/// </summary>
		Fixed = FIXED_PITCH,

		/// <summary>
		/// Variable pitch.
		/// </summary>
		Variable = VARIABLE_PITCH,

		/// <summary>
		/// Mono pitch.
		/// </summary>
		Mono = MONO_FONT,

		/// <summary>
		/// The font family doesn't matter.
		/// </summary>
		DontCare = FF_DONTCARE,

		/// <summary>
		/// Use the Roman family.
		/// </summary>
		Roman = FF_ROMAN,

		/// <summary>
		/// Use the Swiss family.
		/// </summary>
		Swiss = FF_SWISS,

		/// <summary>
		/// Use the Modern family.
		/// </summary>
		Modern = FF_MODERN,

		/// <summary>
		/// Use the Script family.
		/// </summary>
		Script = FF_SCRIPT,

		/// <summary>
		/// Use the Decorative family.
		/// </summary>
		Decorative = FF_DECORATIVE,
	};

	/// <summary>
	/// Defines precision levels for font rendering.
	/// </summary>
	/// <unmanaged href="cc215248">OutPrecision</unmanaged>
	public enum class Precision : System::Int32
	{
		/// <summary>
		/// Default precision.
		/// </summary>
		Default = OUT_DEFAULT_PRECIS,

		/// <summary>
		/// String-level precision.
		/// </summary>
		String = OUT_STRING_PRECIS,

		/// <summary>
		/// Character-level precision.
		/// </summary>
		Character = OUT_CHARACTER_PRECIS,

		/// <summary>
		/// Stroke-level precision.
		/// </summary>
		Stroke = OUT_STROKE_PRECIS,

		/// <summary>
		/// TrueType precision.
		/// </summary>
		TrueType = OUT_TT_PRECIS,

		/// <summary>
		/// Device precision.
		/// </summary>
		Device = OUT_DEVICE_PRECIS,

		/// <summary>
		/// Raster precision.
		/// </summary>
		Raster = OUT_RASTER_PRECIS,

		/// <summary>
		/// TrueType only precision.
		/// </summary>
		TrueTypeOnly = OUT_TT_ONLY_PRECIS,

		/// <summary>
		/// Outline precision.
		/// </summary>
		Outline = OUT_OUTLINE_PRECIS,

		/// <summary>
		/// Screen outline precision.
		/// </summary>
		ScreenOutline = OUT_SCREEN_OUTLINE_PRECIS,

		/// <summary>
		/// PostScript only precision.
		/// </summary>
		PostScriptOnly = OUT_PS_ONLY_PRECIS,
	};

	/// <summary>
	/// Specifies formatting options for text rendering.
	/// </summary>
	/// <unmanaged href="bb773199">DT</unmanaged>
	[System::Flags]
	public enum class DrawTextFormat : System::Int32
	{
		/// <summary>
		/// Align the text to the top.
		/// </summary>
		Top = DT_TOP,

		/// <summary>
		/// Align the text to the left.
		/// </summary>
		Left = DT_LEFT,

		/// <summary>
		/// Align the text to the center.
		/// </summary>
		Center = DT_CENTER,

		/// <summary>
		/// Align the text to the right.
		/// </summary>
		Right = DT_RIGHT,

		/// <summary>
		/// Vertically align the text to the center.
		/// </summary>
		VerticalCenter = DT_VCENTER,

		/// <summary>
		/// Align the text to the bottom.
		/// </summary>
		Bottom = DT_BOTTOM,

		/// <summary>
		/// Allow word breaks.
		/// </summary>
		WordBreak = DT_WORDBREAK,

		/// <summary>
		/// Force all text to a single line.
		/// </summary>
		SingleLine = DT_SINGLELINE,

		/// <summary>
		/// Expand tab characters.
		/// </summary>
		ExpandTabs = DT_EXPANDTABS,

		/// <summary>
		/// Don't clip the text.
		/// </summary>
		NoClip = DT_NOCLIP,

		/// <summary>
		/// Rendering the text in right-to-left reading order.
		/// </summary>
		RtlReading = DT_RTLREADING,

		/// <summary>
		/// 水平居中+垂直居中.
		/// </summary>
		AllCenter = DT_CENTER|DT_VCENTER|DT_SINGLELINE,
	};
#pragma endregion Enum
}
