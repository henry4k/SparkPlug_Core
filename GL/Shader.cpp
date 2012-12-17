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
	Object(context),
	m_Dirty(true)
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
	m_Dirty = true;
	return true;
}

bool Program::detach( StrongRef<Shader>& object )
{
	if(!m_AttachedObjects.count(object))
		return true;
	
	m_AttachedObjects.erase(object);
	glDetachShader(m_Handle, object->handle());
	m_Dirty = true;
	return true;
}

bool Program::link()
{
	bool r = linkSilent();
	ShowProgramLog(m_Handle);
	if(r)
		Log("Linked shader program %s successfully ", toString().c_str());
	else
		LogError("Error linking shader program %s", toString().c_str());
	return r;
}

bool Program::linkSilent()
{
	if(!m_Dirty)
		return true;
	
	glLinkProgram(m_Handle);
	GLint state;
	glGetProgramiv(m_Handle, GL_LINK_STATUS, &state);
	readUniformLocations();
	readAttributeSizes();
	
	m_Dirty = (state == 0);
	return state != 0;
}

bool Program::validate()
{
	bool r = validateSilent();
	ShowProgramLog(m_Handle);
	if(r)
		Log("Validated shader program successfully %s", toString().c_str());
	else
		Log("Error validating shader program %s", toString().c_str());
	return r;
}

bool Program::validateSilent()
{
	glValidateProgram(m_Handle);
	GLint state;
	glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &state);
	return state != 0;
}

void Program::readUniformLocations()
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
		
		glGetActiveUniform(m_Handle, i, sizeof(name)-1, &nameLength, &size, &type, name);
		assert(nameLength > 0);
		
		GLuint location = glGetUniformLocation(m_Handle, name);
		
		m_UniformLocations[name] = location;
	}
}

void Program::readAttributeSizes()
{
	m_AttributeSizes.clear();
	
	int attributeCount = -1;
	glGetProgramiv(m_Handle, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &attributeCount);
	
	for(int i = 0; i < attributeCount; ++i)
	{
		int nameLength = -1;
		char name[100];
		int size = -1;
		GLenum type = GL_ZERO;
		int normalized = -1;
		
		glGetActiveAttrib(
			m_Handle,
			i,
			sizeof(name)-1,
			&nameLength,
			&size,
			&type,
			name
		);
		assert(nameLength > 0);
		
		Log("Attribute %s: %d x %s",
			name,
			size,
			AsString(AttributeTypeFromGL(type, false))
		);
		
		if(nameLength > 3 &&
			name[0] == 'g' &&
			name[1] == 'l' &&
			name[2] == '_')
			continue; // We are not interested in internal attributes.
		
		m_AttributeSizes[name] = size;
	}
}

void Program::setAttributes( const VertexFormat& reference )
{
	int attributeCount = reference.attributeCount();
	std::set<std::string> referenceAttributes;
	
	// Attribute der Referenz in eine Map schreiben (schneller zugriff über Name)
	for(int i = 0; i < attributeCount; ++i)
	{
		referenceAttributes.insert(reference.attribute(i).name());
	}
	
	// Schauen welche Atribute es im Shader, aber nicht in der Referenz gibt
	for(std::map<std::string, int>::const_iterator i = m_AttributeSizes.begin(); i != m_AttributeSizes.end(); ++i)
	{
		if(referenceAttributes.count(i->first) == 0)
		{
			// i ist nicht in reference enthalten!
			// Das IST schlimm!
			LogError("Shader %s needs the vertex attribute %s, which the format does not provide.",
				toString().c_str(),
				i->first.c_str()
			);
			return;
		}
	}
	
	for(int i = 0; i < attributeCount; ++i)
	{
		const VertexAttribute& attribute = reference.attribute(i);
		std::map<std::string, int>::const_iterator targetSizeIter = m_AttributeSizes.find(attribute.name());
		
		if(targetSizeIter == m_AttributeSizes.end())
		{
			Log("Vertex format %s has overhead to %s, because %s is not present in the shader.",
				reference.asString().c_str(),
				toString().c_str(),
				attribute.name()
			);
			continue;
		}
		
		// Sind sie vielleicht unterschiedlich groß?
		if(attribute.componentCount() != targetSizeIter->second)
		{
			LogError("Vertex format %s is incomplatible to %s, because the shader needs %s to consist of %d instead of %d components.",
				reference.asString().c_str(),
				toString().c_str(),
				attribute.name(),
				targetSizeIter->second,
				attribute.componentCount()
			);
			return;
		}
		
		glBindAttribLocation(m_Handle, i, attribute.name());
		m_Dirty = true;
	}
	
	linkSilent();
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
	
	ProgramBinding binding(context(), this);
	glUniform1i(location, value);
	return true;
}

bool Program::setUniform( const char* name, float value )
{
	int location = getUniformLocation(name);
	if(location == -1)
		return false;
	
	ProgramBinding binding(context(), this);
	glUniform1f(location, value);
	return true;
}

bool Program::setUniform( const char* name, int length, const float* values )
{
	int location = getUniformLocation(name);
	if(location == -1)
		return false;

	ProgramBinding binding(context(), this);
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
