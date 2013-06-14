#include <cstdio>
#include <cstring>
#include <cassert>
#include <SparkPlug/Crc32.h>

namespace SparkPlug
{

//static const unsigned int Crc32Poly = 0x04C11DB7;
static const unsigned int Crc32PolyRev = 0xEDB88320;

unsigned int Crc32Table[256];

bool InitCrc32()
{
	unsigned int i, j, h = 1;
	Crc32Table[0] = 0;
	for(i = 128; i; i >>= 1)
	{
		h = (h >> 1) ^ ((h & 1) ? Crc32PolyRev : 0);
		// h is now Crc32Table[i]
		for(j = 0; j < 256; j += 2 * i)
			Crc32Table[i + j] = Crc32Table[j] ^ h;
	}
	
	assert(Crc32Table[255] != 0);
	
	return true;
}
bool Crc32TableInitialized = InitCrc32();


/*
 * This computes the standard preset and inverted CRC, as used
 * by most networking standards.  Start by passing in an initial
 * chaining value of 0, and then pass in the return value from the
 * previous crc32() call.  The final return value is the CRC.
 * Note that this is a little-endian CRC, which is best used with
 * data transmitted lsbit-first, and it should, itself, be appended
 * to data in little-endian byte and bit order to preserve the
 * property of detecting all burst errors of length 32 bits or less.
 */
unsigned int Crc32(unsigned int crc, const char* buf, int len)
{
	crc ^= 0xffffffff;
	while(len--)
		crc = (crc >> 8) ^ Crc32Table[(crc ^ *buf++) & 0xff];
	return crc ^ 0xffffffff;
}


/**
 * Compute the Crc32 checksum for the first len bytes of the buffer.
 *
 * @param buf the data over which we're taking the CRC
 * @param len the length of the buffer
 * @return the resulting Crc32 checksum
 */
unsigned int Crc32Data(const void* buf, int len)
{
	unsigned int crc;
	crc = Crc32(0L, 0, 0);
	crc = Crc32(crc, (const char*)buf, len);
	return crc;
}

unsigned int Crc32String( const char* str )
{
	return Crc32Data(str, std::strlen(str));
}




static const int FileBufferSize = 1024;
unsigned int Crc32File( const char* path )
{
	char buffer[FileBufferSize];
	
	unsigned int crc;
	crc = Crc32(0L, 0, 0);

	std::FILE* f = std::fopen(path, "rb");
	int gcount;
	if(f == NULL)
		return 0;

	do
	{
		gcount = std::fread(buffer, 1, FileBufferSize, f);
		crc = Crc32(crc, buffer, gcount);
	} while(std::feof(f));
	
	std::fclose(f);

	return crc;
}
// TODO: Use new file io system

}
