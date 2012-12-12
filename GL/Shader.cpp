#include <stdio.h>
#include <string>
#include <fstream>

#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Context.h>
#include <SparkPlug/GL/Shader.h>


namespace SparkPlug
{
namespace GL
{


/// ---- Utils ----
// TODO: Rewrite these functions

std::string StringFromFile( const char* path )
{
	std::ifstream f(path);
	std::string r;
	
	while( f.good() )
	{
		std::string line;
		std::getline(f, line);
		r.append(line + "\n");
	}
	
	return r;
}

void ShowShaderLog( GLuint handle )
{
	GLint length = 0;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);
	
	char* log = NULL;
	if(length)
	{
		log = new char[length];
		glGetShaderInfoLog(handle, length, NULL, log);
	}
	
	if(log)
	{
		Log("%s", log);
		delete[] log;
	}
}

void ShowProgramLog( GLuint handle )
{
	GLint length = 0;
	glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);

	char* log = NULL;
	if(length)
	{
		log = new char[length];
		glGetProgramInfoLog(handle, length, NULL, log);
	}
	
	if(log)
	{
		Log("%s", log);
		delete[] log;
	}
}


/// Shader Object ///
Shader::Shader( Context* context, ShaderType type ) :
	Object(context)
{
	m_Handle = glCreateShader(ConvertToGL(type));
}

Shader::~Shader()
{
	glDeleteShader(m_Handle);
}

std::string Shader::toString() const
{
	return m_File;
}

StrongRef<Shader> Shader::CreateFromFile( Context* context, ShaderType type, const char* file )
{
	Shader* obj = new Shader(context, type);
	
	obj->m_File = file;
	
	std::string source = StringFromFile(file);
	if(source.empty())
		return NULL;
	
	const char* shaderSource = source.c_str();
	int shaderLength = source.length();
	glShaderSource(obj->m_Handle, 1, &shaderSource, &shaderLength);
	
	glCompileShader(obj->m_Handle);
	
	
	GLint state;
	glGetShaderiv(obj->m_Handle, GL_COMPILE_STATUS, &state);
	ShowShaderLog(obj->m_Handle);
	if(state)
		Log("Compiled shader object '%s' successfully", file);
	else
		Log("Error compiling shader object '%s'", file);
	
	if(!state)
		return NULL;
	
	return StrongRef<Shader>(obj);
}


/// Program ///
Program::Program( Context* context ) :
	Object(context)
{
	m_Handle = glCreateProgram();
}

Program::~Program()
{
	glDeleteProgram(m_Handle);
}

std::string Program::toString() const
{
	std::string buf = "( ";
	std::set< StrongRef<Shader> >::const_iterator i = m_AttachedObjects.begin();
	for(; i != m_AttachedObjects.end(); ++i)
		buf += (*i)->toString() + " ";
	buf += ")";
	return buf;
}

StrongRef<Program> Program::Create( Context* context )
{
	return StrongRef<Shader>(new Program(context));
}

bool Program::attach( StrongRef<Shader>& object )
{
	if(m_AttachedObjects.count(object))
		return true;
	
	m_AttachedObjects.insert(object);
	glAttachShader(m_Handle, object->handle());
	return true;
}

bool Program::detach( StrongRef<Shader>& object )
{
	if(!m_AttachedObjects.count(object))
		return true;
	
	m_AttachedObjects.erase(object);
	glDetachShader(m_Handle, object->handle());
	return true;
}

bool Program::link()
{
	glLinkProgram(m_Handle);
	{
		GLint state;
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &state);
		ShowProgramLog(m_Handle);
		if(state)
			Log("Linked shader program %s successfully ", toString().c_str());
		else
			LogError("Error linking shader program %s", toString().c_str());
		
		if(!state)
			return false;
	}
	
	glValidateProgram(m_Handle);
	{
		GLint state;
		glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &state);
		ShowProgramLog(m_Handle);
		if(state)
			Log("Validated shader program successfully %s", toString().c_str());
		else
			Log("Error validating shader program %s", toString().c_str());
		
// 		if(!state)
// 			return false;
	}
	
	updateUniformLocations();
	
	return true;
}

void Program::updateUniformLocations()
{
	m_UniformLocations.clear();
	
	int uniformCount = -1;
	glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &uniformCount); 
	
	for(int i = 0; i < uniformCount; ++i)
	{
		int nameLength = -1;
		char name[100];
		
		int size = -1;
		GLenum type = GL_ZERO;
		
		glGetActiveUniform(m_Handle, GLuint(i), sizeof(name)-1, &nameLength, &size, &type, name);
		
		name[nameLength] = '\0';
		GLuint location = glGetUniformLocation(m_Handle, name);
		
		m_UniformLocations[name] = location;
	}
}

int Program::getUniformLocation( const char* uniformName ) const
{
	std::map<std::string,int>::const_iterator i = m_UniformLocations.find(uniformName);
	if(i != m_UniformLocations.end())
		return i->second;
	else
		return -1;
}

bool Program::setUniform( const char* name, int value )
{
	int location = getUniformLocation(name);
	if(location == -1)
		return false;
	
	context()->bindProgram(this);
	glUniform1i(location, value);
	return true;
}

bool Program::setUniform( const char* name, float value )
{
	int location = getUniformLocation(name);
	if(location == -1)
		return false;
	
	context()->bindProgram(this);
	glUniform1f(location, value);
	return true;
}

bool Program::setUniform( const char* name, int length, const float* values )
{
	int location = getUniformLocation(name);
	if(location == -1)
		return false;

	context()->bindProgram(this);
	
	switch(length)
	{
		case 1: glUniform1fv(location, 1, values); break;
		case 2: glUniform2fv(location, 1, values); break;
		case 3: glUniform3fv(location, 1, values); break;
		case 4: glUniform4fv(location, 1, values); break;
		default: assert(false);
	}
	return true;
}



































/*




// --------------------------------------------


/// ---- Shader ----

Shader::Shader() :
	m_Name(-1)
{
}

Shader::~Shader()
{
	clear();
}

void Shader::clear()
{
	if(m_Name != -1)
	{
		glDeleteProgram(m_Name);
		m_Name = -1;
	}
}

void Shader::updateUniformLocations()
{
	m_UniformLocations.clear();
	
	int uniformCount = -1;
	glGetProgramiv(m_Name, GL_ACTIVE_UNIFORMS, &uniformCount); 
	
	for(int i = 0; i < uniformCount; ++i)
	{
		int nameLength = -1;
		char name[100];
		
		int size = -1;
		GLenum type = GL_ZERO;
		
		glGetActiveUniform(m_Name, GLuint(i), sizeof(name)-1, &nameLength, &size, &type, name);
		
		name[nameLength] = '\0';
		GLuint location = glGetUniformLocation(m_Name, name);
		
		m_UniformLocations[name] = location;
	}
}

bool Shader::load( const char* vert, const char* frag )
{
	clear();
	
	GLuint vertObject = CreateShader(vert, GL_VERTEX_SHADER);
	if(!vertObject)
		return false;
	
	GLuint fragObject = CreateShader(frag, GL_FRAGMENT_SHADER);
	if(!fragObject)
		return false;
	
	GLuint program = glCreateProgram();
	m_Name = program;
	glAttachShader(program, vertObject);
	glAttachShader(program, fragObject);
// 	BindVertexAttributes(this);
	
	glLinkProgram(program);
	{
		GLint state;
		glGetProgramiv(program, GL_LINK_STATUS, &state);
		ShowProgramLog(program);
		if(state)
			Log("Linked shader program successfully (vert = '%s'  frag = '%s')", vert, frag);
		else
			LogError("Error linking shader program (vert = '%s'  frag = '%s')", vert, frag);
		
		if(!state)
			return false;
	}
	
	glValidateProgram(program);
	{
		GLint state;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &state);
		ShowProgramLog(program);
		if(state)
			Log("Validated shader program successfully (vert = '%s'  frag = '%s')", vert, frag);
		else
			Log("Error validating shader program (vert = '%s'  frag = '%s')", vert, frag);
		
// 		if(!state)
// 			return false;
	}
	
	updateUniformLocations();
	
	CheckGl();
	
	return true;
}

void Shader::bind() const
{
	glUseProgram(m_Name);
}

*/



}
}
