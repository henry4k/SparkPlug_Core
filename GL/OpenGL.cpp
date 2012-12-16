#include <SparkPlug/Common.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Texture.h>

namespace SparkPlug
{
namespace GL
{

enum Error
{
	Error_None = GL_NO_ERROR,
	Error_InvalidEnum = GL_INVALID_ENUM,
	Error_InvalidValue = GL_INVALID_VALUE,
	Error_InvalidOperation = GL_INVALID_OPERATION,
	Error_StackOverflow = GL_STACK_OVERFLOW,
	Error_StackUnderflow = GL_STACK_UNDERFLOW,
	Error_OutOfMemory = GL_OUT_OF_MEMORY
};
const char* AsString( Error error )
{
	switch(error)
	{
		case Error_None:
			return "None";
		
		case Error_InvalidEnum:
			return "Invalid enum";

		case Error_InvalidValue:
			return "Invalid value";

		case Error_InvalidOperation:
			return "Invalid operation";

		case Error_StackOverflow:
			return "Stack overflow";

		case Error_StackUnderflow:
			return "Stack underflow";

		case Error_OutOfMemory:
			return "Out of memory";

		default:
			return "Unknown error";
	}	
}

void CheckGl()
{
	Error e = (Error)glGetError();
	if(e != Error_None)
	{
		LogError("%s", (const char*)AsString(e));
		Break();
	}
}

void DebugMark( const char* msg )
{
	if(GLEW_GREMEDY_string_marker)
	{
		glStringMarkerGREMEDY(0, msg);
	}
}


}
}

#if defined(GLEW_STATIC)
#include <GL/glew.c>
#endif
