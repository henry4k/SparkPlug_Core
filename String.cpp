#include <SparkPlug/Common.h>
#include <SparkPlug/String.h>

namespace SparkPlug
{

String::String()
{
}

String::String( const String& str ) :
	m_Chars(str.m_Chars)
{
}

String::String( const char* str, int maxUnits )
{
	CharConversionResult r = ConvertChars(
			CharConversionFlags_ZeroTerminated,
			maxUnits,
			reinterpret_cast<const Utf8*>(str),
			&m_Chars);
	if(r != CharConversionResult_Success)
		LogError("String: %s @ %d", AsString(r), m_Chars.size());
}

String::String( const Utf8* str, int maxUnits )
{
	CharConversionResult r = ConvertChars( CharConversionFlags_ZeroTerminated, maxUnits, str, &m_Chars);
	if(r != CharConversionResult_Success)
		LogError("String: %s @ %d", AsString(r), m_Chars.size());
}

String::String( const Utf16* str, int maxUnits )
{
	CharConversionResult r = ConvertChars( CharConversionFlags_ZeroTerminated, maxUnits, str, &m_Chars);
	if(r != CharConversionResult_Success)
		LogError("String: %s @ %d", AsString(r), m_Chars.size());
}

String::String( const Utf32* str, int maxUnits )
{
	CharConversionResult r = ConvertChars( CharConversionFlags_ZeroTerminated, maxUnits, str, &m_Chars);
	if(r != CharConversionResult_Success)
		LogError("String: %s @ %d", AsString(r), m_Chars.size());
}

int String::size() const
{
	return m_Chars.size();
}

}

