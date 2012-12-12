#ifndef __SPARKPLUG_SHADER__
#define __SPARKPLUG_SHADER__

#include <map>
#include <set>
#include <string>

#include <SparkPlug/Reference.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Enums.h>
#include <SparkPlug/GL/Object.h>

namespace SparkPlug
{
namespace GL
{

class Shader : public Object
{
public:
	static StrongRef<Shader> CreateFromFile( Context* context, ShaderType type, const char* file );
	~Shader();
	
	std::string toString() const;
	
private:
	Shader( Context* context, ShaderType type );
	std::string m_File;
};


class Program : public Object
{
public:
	static StrongRef<Program> Create( Context* context );
	~Program();
	
	std::string toString() const;
	
	bool attach( StrongRef<Shader>& object );
	bool detach( StrongRef<Shader>& object );
	
	bool link();
	
	bool setUniform( const char* name, int value );
	bool setUniform( const char* name, float value );
	bool setUniform( const char* name, int length, const float* value );
	
private:
	Program( Context* context );
	
	void clear();
	void updateUniformLocations();
	int getUniformLocation( const char* uniformName ) const;
	
	std::set< StrongRef<Shader> > m_AttachedObjects;
	std::map<std::string, int>    m_UniformLocations;
};


}
}

#endif
