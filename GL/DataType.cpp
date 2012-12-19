#include <cstring>
#include <vector>
#include <SparkPlug/Common.h>
#include <SparkPlug/GL/DataType.h>

namespace SparkPlug
{
namespace GL
{

/// ---- PrimitiveDataType ----

const char* AsString( PrimitiveDataType type )
{
	switch(type)
	{
		case PrimitiveDataType_Bool:   return "bool";
		case PrimitiveDataType_Byte:   return "byte";
		case PrimitiveDataType_UByte:  return "ubyte";
		case PrimitiveDataType_Short:  return "short";
		case PrimitiveDataType_UShort: return "ushort";
		case PrimitiveDataType_Int:    return "int";
		case PrimitiveDataType_UInt:   return "uint";
		case PrimitiveDataType_Float:  return "float";
		case PrimitiveDataType_Double: return "double";
		default: ;
	}
	return "UnknownPrimitiveDataType";
}

char ToDefinitionChar( PrimitiveDataType type )
{
	switch(type)
	{
		case PrimitiveDataType_Bool:   return '?';
		case PrimitiveDataType_Byte:   return 'b';
		case PrimitiveDataType_UByte:  return 'B';
		case PrimitiveDataType_Short:  return 's';
		case PrimitiveDataType_UShort: return 'S';
		case PrimitiveDataType_Int:    return 'i';
		case PrimitiveDataType_UInt:   return 'I';
		case PrimitiveDataType_Float:  return 'f';
		case PrimitiveDataType_Double: return 'd';
		default: ;
	}
	FatalError("Invalid primitve type %u", type);
	return 'E';
}

GLenum ConvertToGL( PrimitiveDataType type )
{
	switch(type)
	{
		case PrimitiveDataType_Bool:   return GL_BOOL;
		case PrimitiveDataType_Byte:   return GL_BYTE;
		case PrimitiveDataType_UByte:  return GL_UNSIGNED_BYTE;
		case PrimitiveDataType_Short:  return GL_SHORT;
		case PrimitiveDataType_UShort: return GL_UNSIGNED_SHORT;
		case PrimitiveDataType_Int:    return GL_INT;
		case PrimitiveDataType_UInt:   return GL_UNSIGNED_INT;
		case PrimitiveDataType_Float:  return GL_FLOAT;
		case PrimitiveDataType_Double: return GL_DOUBLE;
		default: ;
	}
	FatalError("Invalid primitve type %u", type);
	return 0;
}

PrimitiveDataType PrimitiveDataTypeFromGL( GLenum e )
{
	switch(e)
	{
		case GL_BOOL:           return PrimitiveDataType_Bool;
		case GL_BYTE:           return PrimitiveDataType_Byte;
		case GL_UNSIGNED_BYTE:  return PrimitiveDataType_UByte;
		case GL_SHORT:          return PrimitiveDataType_Short;
		case GL_UNSIGNED_SHORT: return PrimitiveDataType_UShort;
		case GL_INT:            return PrimitiveDataType_Int;
		case GL_UNSIGNED_INT:   return PrimitiveDataType_UInt;
		case GL_FLOAT:          return PrimitiveDataType_Float;
		case GL_DOUBLE:         return PrimitiveDataType_Double;
		default: ;
	}
	FatalError("Invalid gl primitve type %u", e);
	return PrimitiveDataType_Bool;
}

int SizeOf( PrimitiveDataType type )
{
	switch(type)
	{
		case PrimitiveDataType_Bool:   return sizeof(GLboolean);
		case PrimitiveDataType_Byte:   return sizeof(GLbyte);
		case PrimitiveDataType_UByte:  return sizeof(GLubyte);
		case PrimitiveDataType_Short:  return sizeof(GLshort);
		case PrimitiveDataType_UShort: return sizeof(GLushort);
		case PrimitiveDataType_Int:    return sizeof(GLint);
		case PrimitiveDataType_UInt:   return sizeof(GLuint);
		case PrimitiveDataType_Float:  return sizeof(GLfloat);
		case PrimitiveDataType_Double: return sizeof(GLdouble);
		default: ;
	}
	FatalError("Invalid primitve type: %u", type);
	return 1;
}

PrimitiveDataType PrimitiveDataTypeByChar( char ch )
{
	switch(ch)
	{
		case '?': return PrimitiveDataType_Bool;
		case 'b': return PrimitiveDataType_Byte;
		case 'B': return PrimitiveDataType_UByte;
		case 's': return PrimitiveDataType_Short;
		case 'S': return PrimitiveDataType_UShort;
		case 'i': return PrimitiveDataType_Int;
		case 'I': return PrimitiveDataType_UInt;
		case 'f': return PrimitiveDataType_Float;
		case 'd': return PrimitiveDataType_Double;
		default: ;
	}
	FatalError("%c does not map to a primitive type!", ch);
	return PrimitiveDataType_Bool;
}


/// ---- CompositeDataType ----

const char* AsString( CompositeDataType type )
{
	switch(type)
	{
		case CompositeDataType_None:   return "none";
		case CompositeDataType_Vector: return "vector";
		case CompositeDataType_Matrix: return "matrix";
		default: ;
	}
	return "UnknownCompositeDataType";
}

CompositeDataType CompositeDataTypeByDefString( const std::string& s )
{
	if(s.empty())
		return CompositeDataType_None;
	if(s == "vec")
		return CompositeDataType_Vector;
	else if(s == "mat")
		return CompositeDataType_Matrix;
	else
	{
		FatalError("'%s' does not map to a composite type!", s.c_str());
		return CompositeDataType_None;
	}
}

const char* ToDefinitionString( CompositeDataType type )
{
	switch(type)
	{
		case CompositeDataType_None:   return "";
		case CompositeDataType_Vector: return "vec";
		case CompositeDataType_Matrix: return "mat";
	}
	return "UnknownCompositeDataType";
}


/// ---- DataType ----

DataType::DataType() :
	m_PrimitiveDataType(PrimitiveDataType_Bool),
	m_CompositeDataType(CompositeDataType_None),
	m_CompositeSize(0)
{
}

DataType::DataType( GLenum e )
{
	switch(e)
	{
		case GL_BOOL:           set(PrimitiveDataType_Bool,   CompositeDataType_None, 1); break;
		case GL_BYTE:           set(PrimitiveDataType_Byte,   CompositeDataType_None, 1); break;
		case GL_UNSIGNED_BYTE:  set(PrimitiveDataType_UByte,  CompositeDataType_None, 1); break;
		case GL_SHORT:          set(PrimitiveDataType_Short,  CompositeDataType_None, 1); break;
		case GL_UNSIGNED_SHORT: set(PrimitiveDataType_UShort, CompositeDataType_None, 1); break;
		case GL_INT:            set(PrimitiveDataType_Int,    CompositeDataType_None, 1); break;
		case GL_UNSIGNED_INT:   set(PrimitiveDataType_Int,    CompositeDataType_None, 1); break;
		case GL_FLOAT:          set(PrimitiveDataType_Float,  CompositeDataType_None, 1); break;
		case GL_DOUBLE:         set(PrimitiveDataType_Double, CompositeDataType_None, 1); break;
		
		case GL_BOOL_VEC2: set(PrimitiveDataType_Bool, CompositeDataType_Vector, 2); break;
		case GL_BOOL_VEC3: set(PrimitiveDataType_Bool, CompositeDataType_Vector, 2); break;
		case GL_BOOL_VEC4: set(PrimitiveDataType_Bool, CompositeDataType_Vector, 2); break;
		
		case GL_INT_VEC2: set(PrimitiveDataType_Int, CompositeDataType_Vector, 2); break;
		case GL_INT_VEC3: set(PrimitiveDataType_Int, CompositeDataType_Vector, 3); break;
		case GL_INT_VEC4: set(PrimitiveDataType_Int, CompositeDataType_Vector, 4); break;
		
		case GL_UNSIGNED_INT_VEC2: set(PrimitiveDataType_UInt, CompositeDataType_Vector, 2); break;
		case GL_UNSIGNED_INT_VEC3: set(PrimitiveDataType_UInt, CompositeDataType_Vector, 3); break;
		case GL_UNSIGNED_INT_VEC4: set(PrimitiveDataType_UInt, CompositeDataType_Vector, 4); break;
		
		case GL_FLOAT_VEC2: set(PrimitiveDataType_Float, CompositeDataType_Vector, 2); break;
		case GL_FLOAT_VEC3: set(PrimitiveDataType_Float, CompositeDataType_Vector, 3); break;
		case GL_FLOAT_VEC4: set(PrimitiveDataType_Float, CompositeDataType_Vector, 4); break;
		case GL_FLOAT_MAT2: set(PrimitiveDataType_Float, CompositeDataType_Matrix, 2); break;
		case GL_FLOAT_MAT3: set(PrimitiveDataType_Float, CompositeDataType_Matrix, 3); break;
		case GL_FLOAT_MAT4: set(PrimitiveDataType_Float, CompositeDataType_Matrix, 4); break;
		
		case GL_DOUBLE_VEC2: set(PrimitiveDataType_Double, CompositeDataType_Vector, 2); break;
		case GL_DOUBLE_VEC3: set(PrimitiveDataType_Double, CompositeDataType_Vector, 3); break;
		case GL_DOUBLE_VEC4: set(PrimitiveDataType_Double, CompositeDataType_Vector, 4); break;
		case GL_DOUBLE_MAT2: set(PrimitiveDataType_Double, CompositeDataType_Matrix, 2); break;
		case GL_DOUBLE_MAT3: set(PrimitiveDataType_Double, CompositeDataType_Matrix, 3); break;
		case GL_DOUBLE_MAT4: set(PrimitiveDataType_Double, CompositeDataType_Matrix, 4); break;

		default: FatalError("Invalid/unsupported gl data type: %u", e);
	}
}

DataType::DataType( const char* def )
{
	setByDef(def, std::strlen(def));
}

DataType::DataType( const char* def, int length )
{
	setByDef(def, length);
}

DataType::DataType( PrimitiveDataType primitive, CompositeDataType composite, int compositeSize ) :
	m_PrimitiveDataType(primitive),
	m_CompositeDataType(composite),
	m_CompositeSize(compositeSize)
{
}

void DataType::set( PrimitiveDataType primitive, CompositeDataType composite, int compositeSize )
{
	m_PrimitiveDataType = primitive;
	m_CompositeDataType = composite;
	m_CompositeSize = compositeSize;
}

void DataType::setByDef( const char* def, int length )
{
	std::vector<char> buf;
	int mode = 0;
	
	for(int i = 0; i < length; ++i)
	{
		char ch = def[i];
		
		switch(mode)
		{
			case 0: // token 1
			{
				if(std::isdigit(ch))
				{
					buf.push_back('\0');
					m_CompositeDataType = CompositeDataTypeByDefString(buf.data());
					buf.clear();
					
					m_CompositeSize = ch - '0';
					assert(InclusiveInside(1,m_CompositeSize,4));
					
					mode = 1;
				}
				else
					buf.push_back(ch);
			} break;
			
			case 1:
			{
				m_PrimitiveDataType = PrimitiveDataTypeByChar(ch);
				return;
			} break;
			
			default:
				FatalError("Invalid mode");
		}
	}

	if(mode == 1 && buf.size() == 1)
	{
		m_CompositeDataType = CompositeDataType_None;
		m_CompositeSize = 1;
		m_PrimitiveDataType = PrimitiveDataTypeByChar(buf[0]);
	}
}

PrimitiveDataType DataType::primitveType() const
{
	return m_PrimitiveDataType;
}

CompositeDataType DataType::compositeType() const
{
	return m_CompositeDataType;
}

int DataType::compositeSize() const
{
	return m_CompositeSize;
}

int DataType::componentCount() const
{
	switch(m_CompositeDataType)
	{
		case CompositeDataType_None:
		case CompositeDataType_Vector:
			return m_CompositeSize;

		case CompositeDataType_Matrix:
			return m_CompositeSize * m_CompositeSize;

		default:
			;
	}
	assert(false);
	return 0;
}

int DataType::sizeInBytes() const
{
	return componentCount() * SizeOf(m_PrimitiveDataType);
}

std::string DataType::toString() const
{
	if(m_CompositeDataType == CompositeDataType_None)
		return Format("%s", AsString(m_PrimitiveDataType));
	else
		return Format("%s%d%c", ToDefinitionString(m_CompositeDataType), m_CompositeSize, ToDefinitionChar(m_PrimitiveDataType));
}

GLenum DataType::toGLenum() const
{
	switch(m_CompositeDataType)
	{
		case CompositeDataType_None:
		{
			switch(m_PrimitiveDataType)
			{
				case PrimitiveDataType_Bool:   return GL_BOOL;
				case PrimitiveDataType_Byte:   return GL_BYTE;
				case PrimitiveDataType_UByte:  return GL_UNSIGNED_BYTE;
				case PrimitiveDataType_Short:  return GL_SHORT;
				case PrimitiveDataType_UShort: return GL_UNSIGNED_SHORT;
				case PrimitiveDataType_Int:    return GL_INT;
				case PrimitiveDataType_UInt:   return GL_UNSIGNED_INT;
				case PrimitiveDataType_Float:  return GL_FLOAT;
				case PrimitiveDataType_Double: return GL_DOUBLE;
			}
		} break;

		case CompositeDataType_Vector:
		{
			switch(m_CompositeSize)
			{
				case 2:
				{
					switch(m_PrimitiveDataType)
					{
						case PrimitiveDataType_Bool:   return GL_BOOL_VEC2;
						case PrimitiveDataType_Int:    return GL_INT_VEC2;
						case PrimitiveDataType_UInt:   return GL_UNSIGNED_INT_VEC2;
						case PrimitiveDataType_Float:  return GL_FLOAT_VEC2;
						case PrimitiveDataType_Double: return GL_DOUBLE_VEC2;
					}
				} break;

				case 3:
				{
					switch(m_PrimitiveDataType)
					{
						case PrimitiveDataType_Bool:   return GL_BOOL_VEC3;
						case PrimitiveDataType_Int:    return GL_INT_VEC3;
						case PrimitiveDataType_UInt:   return GL_UNSIGNED_INT_VEC3;
						case PrimitiveDataType_Float:  return GL_FLOAT_VEC3;
						case PrimitiveDataType_Double: return GL_DOUBLE_VEC3;
					}
				} break;


				case 4:
				{
					switch(m_PrimitiveDataType)
					{
						case PrimitiveDataType_Bool:   return GL_BOOL_VEC4;
						case PrimitiveDataType_Int:    return GL_INT_VEC4;
						case PrimitiveDataType_UInt:   return GL_UNSIGNED_INT_VEC4;
						case PrimitiveDataType_Float:  return GL_FLOAT_VEC4;
						case PrimitiveDataType_Double: return GL_DOUBLE_VEC4;
					}
				} break;
			}
		} break;

		case CompositeDataType_Matrix:
		{
			switch(m_CompositeSize)
			{
				case 2:
				{
					switch(m_PrimitiveDataType)
					{
						case PrimitiveDataType_Float:  return GL_FLOAT_MAT2;
						case PrimitiveDataType_Double: return GL_DOUBLE_MAT2;
					}
				} break;

				case 3:
				{
					switch(m_PrimitiveDataType)
					{
						case PrimitiveDataType_Float:  return GL_FLOAT_MAT3;
						case PrimitiveDataType_Double: return GL_DOUBLE_MAT3;
					}
				} break;

				case 4:
				{
					switch(m_PrimitiveDataType)
					{
						case PrimitiveDataType_Float:  return GL_FLOAT_MAT4;
						case PrimitiveDataType_Double: return GL_DOUBLE_MAT4;
					}
				} break;
			}
		} break;
	}

	FatalError("No GL type for %s found.", toString().c_str());
	return 0;
}

bool DataType::operator == ( const DataType& other ) const
{
	return
		(m_PrimitiveDataType == other.m_PrimitiveDataType) &&
		(m_CompositeSize == other.m_CompositeSize) &&
		(m_CompositeSize == m_CompositeSize);
}

bool DataType::operator != ( const DataType& other ) const
{
	return !(*this == other);
}


}
}
