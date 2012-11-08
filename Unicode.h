#ifndef __SPARKPLUG_UNICODE__
#define __SPARKPLUG_UNICODE__

#include <vector>

namespace SparkPlug
{

typedef unsigned char  Utf8;
typedef unsigned short Utf16;
typedef unsigned long  Utf32;

enum CharConversionResult 
{
	CharConversionResult_Success,
	CharConversionResult_UnfinishedChar,
	CharConversionResult_IllegalChar
};
const char* AsString( CharConversionResult v );

enum CharConversionFlags
{
	CharConversionFlags_Strict,
	CharConversionFlags_Lenient
};
const char* AsString( CharConversionFlags v );


/**
 * Looks through the string and returns its length.
 * Returns `-1` when the string exceeds `maxLength`!
 * `maxLength` may be omitted or set to `-1`
 * to accept an unlimited amount of characters.
 */
int StringLength( const Utf8* source, int maxLength = -1 );
int StringLength( const Utf16* source, int maxLength = -1 );
int StringLength( const Utf32* source, int maxLength = -1 );

/**
 * Tests if a string consists of legal/allowed characters.
 */
bool IsLegalString( const Utf8* source, int length );
bool IsLegalString( const Utf16* source, int length );
bool IsLegalString( const Utf32* source, int length );

/**
 * Converts between different encodings.
 * May run in strict or lenient mode,
 * the latter one won't bail out on lesser errors.
 * Keep in mind that the characters are *appended* to the destination.
 */
CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf32* source, std::vector<Utf16>* destination );
CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf32* source, std::vector<Utf8>* destination );
CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf16* source, std::vector<Utf32>* destination );
CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf16* source, std::vector<Utf8>* destination );
CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf8* source, std::vector<Utf32>* destination );
CharConversionResult ConvertChars( CharConversionFlags flags, int length, const Utf8* source, std::vector<Utf16>* destination );


}

#endif
