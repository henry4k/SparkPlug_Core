#ifndef __SPARKPLUG_OPENGL__
#define __SPARKPLUG_OPENGL__

#if defined(__APPLE__)
#	include <OpenGL/glext.h>
#else
#	include <GL/glew.h>
#endif

namespace SparkPlug
{
	void CheckGl();
}

#endif
