#ifndef __SPARKPLUG_GL_OBJECT__
#define __SPARKPLUG_GL_OBJECT__

#include <SparkPlug/GL/OpenGL.h>



namespace SparkPlug
{
namespace GL
{

class Context;

class Object
{
public:
	Object( Context* context );
	virtual ~Object();
	
	GLuint handle() const;
	Context* context() const;
	
protected:
	GLuint m_Handle;
	
private:
	Context* m_Context;
};

}
}

#endif
