#include <cstring>
#include <SparkPlug/Common.h>
#include <SparkPlug/GL/Debug.h>


namespace SparkPlug
{
namespace GL
{


/// Debugger ///

Debugger::Debugger( Context* context ) :
	m_Context(context),
	m_Enabled(false)
{
}

Debugger::~Debugger()
{
}

/*
void Debugger::enable( bool e )
{
	if(e == m_Enabled)
		return;
	m_Enabled = e;
	
	if(e)
	{
		glDebugMessageCallbackARB(Debugger::onDebugEventWrapper, this);
	}
	else
	{
	}

}
*/

void Debugger::emitDebugMessage(
	DebugEventSource source,
	DebugEventType type,
	int id,
	DebugEventSeverity severity,
	const char* message
)
{
	glDebugMessageInsertARB(
		ConvertToGL(source),
		ConvertToGL(type),
		id,
		ConvertToGL(severity),
		std::strlen(message),
		message
	);
}

void Debugger::onDebugEvent(
	DebugEventSource source,
	DebugEventType type,
	int id,
	DebugEventSeverity severity,
	const char* message
)
{
	// ...
}

void Debugger::onDebugEventWrapper(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const char* message,
	void* userParam
)
{
	Debugger* debugger = reinterpret_cast<Debugger*>(userParam);
	debugger->onDebugEvent(
		DebugEventSourceFromGL(source),
		DebugEventTypeFromGL(type),
		id,
		DebugEventSeverityFromGL(severity),
		message
	);
}

}
}
