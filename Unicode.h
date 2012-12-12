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
	CharConversionFlags_Strict         = (1 << 0),
	CharConversionFlags_ZeroTerminated = (1 << 1)
};


/**
 * Looks through the string and returns its units.
 * `maxUnits` may be omitted or set to `-1`
 * when scanning sources of unknown size.
 */
int StringLength( const Utf8* source, int maxUnits = -1 );
int StringLength( const Utf16* source, int maxUnits = -1 );
int StringLength( const Utf32* source, int maxUnits = -1 );

/**
 * Tests if a string consists of legal/allowed characters.
 */
bool IsLegalString( const Utf8* source, int units );
bool IsLegalString( const Utf16* source, int units );
bool IsLegalString( const Utf32* source, int units );

/**
 * Converts between different encodings.
 * May run in strict or lenient mode,
 * the latter one won't bail out on lesser errors.
 * Keep in mind that the characters are *appended* to the destination.
 */
CharConversionResult ConvertChars( int flags, int units, const Utf32* source, std::vector<Utf16>* destination );
CharConversionResult ConvertChars( int flags, int units, const Utf32* source, std::vector<Utf8>* destination );
CharConversionResult ConvertChars( int flags, int units, const Utf16* source, std::vector<Utf32>* destination );
CharConversionResult ConvertChars( int flags, int units, const Utf16* source, std::vector<Utf8>* destination );
CharConversionResult ConvertChars( int flags, int units, const Utf8* source, std::vector<Utf32>* destination );
CharConversionResult ConvertChars( int flags, int units, const Utf8* source, std::vector<Utf16>* destination );

/**
 * Dummy function that directly copies the data.
 */
CharConversionResult ConvertChars( int flags, int units, const Utf32* source, std::vector<Utf32>* destination );
CharConversionResult ConvertChars( int flags, int units, const Utf16* source, std::vector<Utf16>* destination );
CharConversionResult ConvertChars( int flags, int units, const Utf8* source, std::vector<Utf8>* destination );


}

#endif
