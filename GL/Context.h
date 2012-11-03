#ifndef __SPARKPLUG_RENDER_CONTEXT__
#define __SPARKPLUG_RENDER_CONTEXT__

#include <SparkPlug/Reference.h>
#include <SparkPlug/gl/OpenGL.h>


namespace SparkPlug
{

class Context;


class Limits
{
	public:
		int maxTextureUnits;
		// ...
		
	protected:
		Limits( const Context& context );
		friend class Context;
};


class State
{
	public:
		StrongRef<ShaderProgram> shaderProgram;
		
	protected:
		State( const Context& context );
		friend class Context;
};


class Context
{
	public:
		Context();
		virtual ~Context();
		
		const Limits& limits() const;
		const State&  state() const;
		
	private:
		Limits m_Limits;
		State  m_State;
};

}

#endif