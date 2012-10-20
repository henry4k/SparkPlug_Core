#ifndef __SPARKPLUG_CRC32__
#define __SPARKPLUG_CRC32__

namespace SparkPlug
{

/// Calculates the crc32 checksum for an data chunk.
unsigned int Crc32( unsigned int crc, const char* buf, int len );

/// Calculates the crc32 checksum for some data.
unsigned int Crc32Data( const void* buf, int len );

/// Calculates the crc32 checksum for a string.
unsigned int Crc32String( const char* str );

/// Calculates the crc32 checksum for a file.
unsigned int Crc32File( const char* path );

}

#endif
