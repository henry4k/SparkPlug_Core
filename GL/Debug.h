#ifndef __SPARKPLUG_RENDER_CONTEXT__
#define __SPARKPLUG_RENDER_CONTEXT__

#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Enums.h>


namespace SparkPlug
{
namespace GL
{

class Context;

class Debugger
{
public:
	Debugger( Context* contex );
	virtual ~Debugger();
	
	void emitDebugMessage(
		DebugEventSource source,
		DebugEventType type,
		int id,
		DebugEventSeverity severity,
		const char* message
     	);
	
protected:
	/**
	 * The default action logs the errors.
	 */
	virtual void onDebugEvent(
		DebugEventSource source,
		DebugEventType type,
		int id,
		DebugEventSeverity severity,
		const char* message
	);
	
private:
	static void onDebugEventWrapper(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const char* message,
		void* userParam
	);
	
	Context* m_Context;
	bool     m_Enabled;
};

}
}

#endif
