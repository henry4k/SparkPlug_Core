#ifndef __SPARKPLUG_GL_PIXEL__
#define __SPARKPLUG_GL_PIXEL__

#include <SparkPlug/Pixel.h>
#include <SparkPlug/GL/OpenGL.h>

namespace SparkPlug
{
namespace GL
{
	GLenum ConvertToGL( PixelComponent component );
	GLenum ConvertToGL( PixelSemantic semantic );
	GLenum ConvertToGL( const PixelFormat& format, bool sRGB );

}
}

#endif
