#ifndef __SPARKPLUG_SHADER__
#define __SPARKPLUG_SHADER__

#include <map>
#include <string>

#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Texture.h>

namespace SparkPlug
{
namespace GL
{

class ShaderProgramm
{
	public:
		ShaderProgramm();
		~ShaderProgramm();
		
		bool loadFromSource( const char* source );
		bool loadFromFile( const char* file );

		// ...

	protected:
		GLuint m_Name; 
};

class Shader
{
	public:
		Shader();
		~Shader();
		
		bool load( const char* vert, const char* frag );
		void bind() const;
		
		void setUniform( const char* name, int value ) const;
		void setUniform( const char* name, float value ) const;
		void setUniform( const char* name, int length, const float* value ) const;
		
		GLuint name() const { return m_Name; }
		
	protected:
		void clear();
		void updateUniformLocations();
		int getUniformLocation( const char* uniformName ) const;
		
		GLuint m_Name;
		std::map<std::string, int> m_UniformLocations;
};

}
}

#endif
