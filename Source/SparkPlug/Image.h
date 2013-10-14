#ifndef __SPARKPLUG_IMAGE__
#define __SPARKPLUG_IMAGE__

#include <SparkPlug/Pixel.h>

namespace SparkPlug
{

class Image
{
public:
	Image();
	Image( PixelFormat format, int width, int height, int depth );
	~Image();
	
	/**
	 * (Re-)Initializes an image with new content.
	 * IMPORTANT: The pixel data is not memcopied,
	 * its used directly as internal data pointer.
	 */
	void initFromRaw( PixelFormat format, int width, int height, int depth, void* pixels );
	
	const PixelFormat& format() const;
	
	int width() const;
	int height() const;
	int depth() const;
	
	void* pixels();
	const void* pixels() const;
	
	void* pixelAt( int x, int y, int z );
	const void* pixelAt( int x, int y, int z ) const;
	
private:
	Image( const Image& source );
	Image& operator = ( const Image& source );
	
	PixelFormat m_Format;
	int m_Width, m_Height, m_Depth;
	char* m_Data;
};

}

#endif
