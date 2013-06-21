#ifndef __SPARKPLUG_IMAGE__
#define __SPARKPLUG_IMAGE__

#include <SparkPlug/Vector.h>
#include <SparkPlug/Pixel.h>

namespace SparkPlug
{

class Image
{
public:
	Image();
	Image( PixelFormat format, vec3i size );
	~Image();
	
	/**
	 * (Re-)Initializes an image with new content.
	 * IMPORTANT: The pixel data is not memcopied,
	 * its used directly as internal data pointer.
	 */
	void initFromRaw( PixelFormat format, vec3i size, void* pixels );
	
	const PixelFormat& format() const;
	
	vec3i size() const;
	
	void* pixels();
	const void* pixels() const;
	
	void* pixelAt( vec3i pos );
	const void* pixelAt( vec3i pos ) const;
	
private:
	Image( const Image& source );
	Image& operator = ( const Image& source );
	
	PixelFormat m_Format;
	vec3i m_Size;
	char* m_Data;
};

}

#endif