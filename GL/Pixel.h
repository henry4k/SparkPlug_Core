#ifndef __SPARKPLUG_GL_PIXEL__
#define __SPARKPLUG_GL_PIXEL__

#include <SparkPlug/Pixel.h>
#include <SparkPlug/gl/OpenGL.h>

namespace SparkPlug
{
	GLenum ConvertToGL( PixelComponent component );
	GLenum ConvertToGL( PixelSemantic semantic );
	GLenum ConvertToGL( const PixelFormat& format, bool sRGB );
}

#endif
