#include <SparkPlug/GL/Object.h>

namespace SparkPlug
{
namespace GL
{

Object::Object( Context* context ) :
	m_Context(context)
{
}

Object::~Object()
{
}

GLuint Object::handle() const
{
	return m_Handle;
}

Context* Object::context() const
{
	return m_Context;
}

}
}
