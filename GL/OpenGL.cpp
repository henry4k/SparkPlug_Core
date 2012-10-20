#include <SparkPlug/Common.h>
#include <SparkPlug/gl/OpenGL.h>
#include <SparkPlug/GL/Texture.h>

namespace SparkPlug
{

void CheckGl()
{
	GLenum e = glGetError();
	if(e != GL_NO_ERROR)
	{
		FatalError("%s", (const char*)gluErrorString(e));
	}
}

}

#if defined(GLEW_STATIC)
#include <GL/glew.c>
#endif
