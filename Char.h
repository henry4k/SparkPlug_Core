#ifndef __SPARKPLUG_CHAR__
#define __SPARKPLUG_CHAR__

#include <vector>
#include <SparkPlug/Unicode.h>

namespace SparkPlug
{

typedef Utf8 Char;

/**
 * Decodes a maximum of `length` characters from `source` to `destination`.
 * Will decode less then `length` when a zero is encountered.
 * May return a negative value on error.
 */
typedef int CharDecoderFn( int length, const void* source, Char* destination );

/**
 * Encodes a maximum of `length` characters from `source` to `destination`.
 * Will encode less then `length` when a zero is encountered.
 * May return a negative value on error.
 */
typedef int CharEncoderFn( int length, const Char* source, void* destination );


int DecodeUtf8( int length, const void* source, Char* destination );
int EncodeUtf8( int length, const Char* source, void* destination );

int DecodeUtf16( int length, const void* source, Char* destination );
int EncodeUtf16( int length, const Char* source, void* destination );

int DecodeUtf32( int length, const void* source, Char* destination );
int EncodeUtf32( int length, const Char* source, void* destination );

}

#endif
