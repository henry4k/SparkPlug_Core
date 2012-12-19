#include <cstring>
#include <SparkPlug/Common.h>
#include <SparkPlug/GL/DataType.h>

namespace SparkPlug
{
namespace GL
{

/// ---- PrimitiveType ----

const char* AsString( PrimitveType type )
{
	switch(type)
	{
		case PrimitveType_Bool:   return "bool";
		case PrimitveType_Byte:   return "byte";
		case PrimitveType_UByte:  return "ubyte";
		case PrimitveType_Short:  return "short";
		case PrimitveType_UShort: return "ushort";
		case PrimitveType_Int:    return "int";
		case PrimitveType_UInt:   return "uint";
		case PrimitveType_Float:  return "float";
		case PrimitveType_Double: return "double";
		default: ;
	}
	return "UnknownPrimitiveType";
}

char ToDefinitionChar( PrimitiveType type )
{
	switch(type)
	{
		case PrimitveType_Bool:   return '?';
		case PrimitveType_Byte:   return 'b';
		case PrimitveType_UByte:  return 'B';
		case PrimitveType_Short:  return 's';
		case PrimitveType_UShort: return 'S';
		case PrimitveType_Int:    return 'i';
		case PrimitveType_UInt:   return 'I';
		case PrimitveType_Float:  return 'f';
		case PrimitveType_Double: return 'd';
		default: ;
	}
	return "UnknownPrimitiveType";
}

GLenum ConvertToGL( PrimitveType type )
{
	switch(type)
	{
		case PrimitveType_Bool:   return GL_BOOL;
		case PrimitveType_Byte:   return GL_BYTE;
		case PrimitveType_UByte:  return GL_UNSIGNED_BYTE;
		case PrimitveType_Short:  return GL_SHORT;
		case PrimitveType_UShort: return GL_UNSIGNED_SHORT;
		case PrimitveType_Int:    return GL_INT;
		case PrimitveType_UInt:   return GL_UNSIGNED_INT;
		case PrimitveType_Float:  return GL_FLOAT;
		case PrimitveType_Double: return GL_DOUBLE;
		default: ;
	}
	FatalError("Invalid primitve type %u", type);
	return 0;
}

PrimitveType PrimitveTypeFromGL( GLenum e )
{
	switch(type)
	{
		case GL_BOOL:           return PrimitveType_Bool;
		case GL_BYTE:           return PrimitveType_Byte;
		case GL_UNSIGNED_BYTE:  return PrimitveType_UByte;
		case GL_SHORT:          return PrimitveType_Short;
		case GL_UNSIGNED_SHORT: return PrimitveType_UShort;
		case GL_INT:            return PrimitveType_Int;
		case GL_UNSIGNED_INT:   return PrimitveType_UInt;
		case GL_FLOAT:          return PrimitveType_Float;
		case GL_DOUBLE:         return PrimitveType_Double;
		default: ;
	}
	FatalError("Invalid gl primitve type %u", type);
	return PrimitveType_Bool;
}

int SizeOf( PrimitveType type )
{
	switch(type)
	{
		case PrimitveType_Bool:   return sizeof(GLboolean);
		case PrimitveType_Byte:   return sizeof(GLbyte);
		case PrimitveType_UByte:  return sizeof(GLubyte);
		case PrimitveType_Short:  return sizeof(GLshort);
		case PrimitveType_UShort: return sizeof(GLushort);
		case PrimitveType_Int:    return sizeof(GLint);
		case PrimitveType_UInt:   return sizeof(GLuint);
		case PrimitveType_Float:  return sizeof(GLfloat);
		case PrimitveType_Double: return sizeof(GLdouble);
		default: ;
	}
	FatalError("Invalid primitve type: %u", type);
	return 1;
}

PrimitveType PrimitveTypeByChar( char ch )
{
	switch(ch)
	{
		case '?': return PrimitveType_Bool;
		case 'b': return PrimitveType_Byte;
		case 'B': return PrimitveType_UByte;
		case 's': return PrimitveType_Short;
		case 'S': return PrimitveType_UShort;
		case 'i': return PrimitveType_Int;
		case 'I': return PrimitveType_UInt;
		case 'f': return PrimitveType_Float;
		case 'd': return PrimitveType_Double;
		default: ;
	}
	FatalError("%c does not map to a primitive type!", ch);
	return PrimitveType_Bool;
}


/// ---- CompositeType ----

const char* AsString( CompositeType type )
{
	switch(type)
	{
		case CompositeType_None:   return "none";
		case CompositeType_Vector: return "vector";
		case CompositeType_Matrix: return "matrix";
		default: ;
	}
	return "UnknownCompositeType";
}

CompositeType CompositeTypeByDefString( const std::string& s )
{
	if(s.empty())
		return CompositeType_None;
	if(s == "vec")
		return CompositeType_Vector;
	else if(s == "mat")
		return CompositeType_Matrix;
	else
	{
		FatalError("'%s' does not map to a composite type!", s.c_str());
		return CompositeType_None;
	}
}

const char* ToDefinitionString( CompositeType type )
{
	switch(type)
	{
		case CompositeType_None:   return "";
		case CompositeType_Vector: return "vec";
		case CompositeType_Matrix: return "mat";
	}
	return "UnknownCompositeType";
}


/// ---- DataType ----

DataType::DataType()
{
	// Attention: Uninitialized!
}

DataType::DataType( GLenum e )
{
	switch(e)
	{
		case GL_BOOL:           set(PrimitveType_Bool,   CompositeType_None, 1); break;
		case GL_BYTE:           set(PrimitveType_Byte,   CompositeType_None, 1); break;
		case GL_UNSIGNED_BYTE:  set(PrimitveType_UByte,  CompositeType_None, 1); break;
		case GL_SHORT:          set(PrimitveType_Short,  CompositeType_None, 1); break;
		case GL_UNSIGNED_SHORT: set(PrimitveType_UShort, CompositeType_None, 1); break;
		case GL_INT:            set(PrimitveType_Int,    CompositeType_None, 1); break;
		case GL_UNSIGNED_INT:   set(PrimitveType_Int,    CompositeType_None, 1); break;
		case GL_FLOAT:          set(PrimitveType_Float,  CompositeType_None, 1); break;
		case GL_DOUBLE:         set(PrimitveType_Double, CompositeType_None, 1); break;
		
		case GL_BOOL_VEC2: set(PrimitveType_Bool, CompositeType_Vector, 2); break;
		case GL_BOOL_VEC3: set(PrimitveType_Bool, CompositeType_Vector, 2); break;
		case GL_BOOL_VEC4: set(PrimitveType_Bool, CompositeType_Vector, 2); break;
		
		case GL_INT_VEC2: set(PrimitveType_Int, CompositeType_Vector, 2); break;
		case GL_INT_VEC3: set(PrimitveType_Int, CompositeType_Vector, 3); break;
		case GL_INT_VEC4: set(PrimitveType_Int, CompositeType_Vector, 4); break;
		
		case GL_UNSIGNED_INT_VEC2: set(PrimitveType_UInt, CompositeType_Vector, 2); break;
		case GL_UNSIGNED_INT_VEC3: set(PrimitveType_UInt, CompositeType_Vector, 3); break;
		case GL_UNSIGNED_INT_VEC4: set(PrimitveType_UInt, CompositeType_Vector, 4); break;
		
		case GL_FLOAT_VEC2: set(PrimitveType_Float, CompositeType_Vector, 2); break;
		case GL_FLOAT_VEC3: set(PrimitveType_Float, CompositeType_Vector, 3); break;
		case GL_FLOAT_VEC4: set(PrimitveType_Float, CompositeType_Vector, 4); break;
		case GL_FLOAT_MAT2: set(PrimitveType_Float, CompositeType_Matrix, 2); break;
		case GL_FLOAT_MAT3: set(PrimitveType_Float, CompositeType_Matrix, 3); break;
		case GL_FLOAT_MAT4: set(PrimitveType_Float, CompositeType_Matrix, 4); break;
		
		case GL_DOUBLE_VEC2: set(PrimitveType_Double, CompositeType_Vector, 2); break;
		case GL_DOUBLE_VEC3: set(PrimitveType_Double, CompositeType_Vector, 3); break;
		case GL_DOUBLE_VEC4: set(PrimitveType_Double, CompositeType_Vector, 4); break;
		case GL_DOUBLE_MAT2: set(PrimitveType_Double, CompositeType_Matrix, 2); break;
		case GL_DOUBLE_MAT3: set(PrimitveType_Double, CompositeType_Matrix, 3); break;
		case GL_DOUBLE_MAT4: set(PrimitveType_Double, CompositeType_Matrix, 4); break;

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

DataType::DataType( PrimitveType primitive, CompositeType composite, int compositeSize ) :
	m_PrimitiveType(primitive),
	m_CompositeType(composite),
	m_CompositeSize(compositeSize)
{
}

void DataType::set( PrimitveType primitive, CompositeType composite, int compositeSize )
{
	m_PrimitiveType = primitive;
	m_CompositeType = composite;
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
					m_CompositeType = CompositeTypeByDefString(buf);
					buf.clear();
					mode = 1;
				}
				else
					buf.push_back(ch);
			} break;
			
			case 1: // count
			{
				m_CompositeSize = ch - '0';
				assert(InclusiveInside(1,m_Count,4));
				mode = 2;
			} break;
			
			case 2: // token 3
			{
				m_PrimitiveType = PrimitiveTypeByChar(ch);
				return;
			} break;
			
			default:
				FatalError("Invalid mode");
		}
	}

	if(mode == 1 && buf.size() == 1)
	{
		m_CompositeType = CompositeType_None;
		m_CompositeSize = 1;
		m_PrimitiveType = PrimitiveTypeByChar(buf[0]);
	}
}

PrimitveType DataType::primitveType() const
{
	return m_PrimitiveType;
}

CompositeType DataType::compositeType() const
{
	return m_CompositeType;
}

int DataType::compositeSize() const
{
	return m_CompositeSize;
}

int DataType::componentCount() const
{
	switch(m_CompositeType)
	{
		case CompositeType_None:
		case CompositeType_Vector:
			return m_CompositeSize;

		case CompositeType_Matrix:
			return m_CompositeSize * m_CompositeSize;

		default:
			;
	}
	assert(false);
	return 0;
}

int DataType::sizeInBytes() const
{
	return componentCount() * SizeOf(m_PrimitiveType);
}

std::string DataType::toString() const
{
	if(m_CompositeType == CompositeType_None)
		return Format("%s", AsString(m_PrimitiveType));
	else
		return Format("%s%d%c", ToDefinitionString(m_CompositeType), m_CompositeSize, ToDefinitionChar(m_PrimitiveType));
}

GLenum DataType::toGLenum() const
{
	switch(m_CompositeType)
	{
		case CompositeType_None:
		{
			switch(m_PrimitiveType)
			{
				case PrimitveType_Bool:   return GL_BOOL;
				case PrimitveType_Byte:   return GL_BYTE;
				case PrimitveType_UByte:  return GL_UNSIGNED_BYTE;
				case PrimitveType_Short:  return GL_SHORT;
				case PrimitveType_UShort: return GL_UNSIGNED_SHORT;
				case PrimitveType_Int:    return GL_INT;
				case PrimitveType_UInt:   return GL_UNSIGNED_INT;
				case PrimitveType_Float:  return GL_FLOAT;
				case PrimitveType_Double: return GL_DOUBLE;
			}
		} break;

		case CompositeType_Vector:
		{
			switch(m_CompositeSize)
			{
				case 2:
				{
					switch(m_PrimitiveType)
					{
						case PrimitveType_Bool:   return GL_BOOL_VEC2;
						case PrimitveType_Int:    return GL_INT_VEC2;
						case PrimitveType_UInt:   return GL_UNSIGNED_INT_VEC2;
						case PrimitveType_Float:  return GL_FLOAT_VEC2;
						case PrimitveType_Double: return GL_DOUBLE_VEC2;
					}
				} break;

				case 3:
				{
					switch(m_PrimitiveType)
					{
						case PrimitveType_Bool:   return GL_BOOL_VEC3;
						case PrimitveType_Int:    return GL_INT_VEC3;
						case PrimitveType_UInt:   return GL_UNSIGNED_INT_VEC3;
						case PrimitveType_Float:  return GL_FLOAT_VEC3;
						case PrimitveType_Double: return GL_DOUBLE_VEC3;
					}
				} break;


				case 4:
				{
					switch(m_PrimitiveType)
					{
						case PrimitveType_Bool:   return GL_BOOL_VEC4;
						case PrimitveType_Int:    return GL_INT_VEC4;
						case PrimitveType_UInt:   return GL_UNSIGNED_INT_VEC4;
						case PrimitveType_Float:  return GL_FLOAT_VEC4;
						case PrimitveType_Double: return GL_DOUBLE_VEC4;
					}
				} break;
			}
		} break;

		case CompositeType_Matrix:
		{
			switch(m_CompositeSize)
			{
				case 2:
				{
					switch(m_PrimitiveType)
					{
						case PrimitveType_Float:  return GL_FLOAT_MAT2;
						case PrimitveType_Double: return GL_DOUBLE_MAT2;
					}
				} break;

				case 3:
				{
					switch(m_PrimitiveType)
					{
						case PrimitveType_Float:  return GL_FLOAT_MAT3;
						case PrimitveType_Double: return GL_DOUBLE_MAT3;
					}
				} break;

				case 4:
				{
					switch(m_PrimitiveType)
					{
						case PrimitveType_Float:  return GL_FLOAT_MAT4;
						case PrimitveType_Double: return GL_DOUBLE_MAT4;
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
		(m_PrimitiveType == other.m_PrimitiveType) &&
		(m_CompositeSize == other.m_CompositeSize) &&
		(m_CompositeSize == m_CompositeSize);
}

bool DataType::operator != ( const DataType& other ) const
{
	return !(*this == other);
}


}
}
