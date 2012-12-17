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


/// ---- TypeInfo ----

TypeInfo::TypeInfo()
{
	// Attention: Uninitialized!
}

TypeInfo::TypeInfo( GLenum e )
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
		
		case GL_BOOL_VEC2_ARB:   set(PrimitveType_Bool,   CompositeType_Vector, 2); break;
		case GL_BOOL_VEC3_ARB:   set(PrimitveType_Bool,   CompositeType_Vector, 2); break;
		case GL_BOOL_VEC4_ARB:   set(PrimitveType_Bool,   CompositeType_Vector, 2); break;
		
		case GL_INT_VEC2:    set(PrimitveType_Int,    CompositeType_Vector, 2); break;
		case GL_INT_VEC3:    set(PrimitveType_Int,    CompositeType_Vector, 3); break;
		case GL_INT_VEC4:    set(PrimitveType_Int,    CompositeType_Vector, 4); break;
		
		case GL_UNSIGNED_INT_VEC2: set(PrimitveType_UInt, CompositeType_Vector, 2); break;
		case GL_UNSIGNED_INT_VEC3: set(PrimitveType_UInt, CompositeType_Vector, 3); break;
		case GL_UNSIGNED_INT_VEC4: set(PrimitveType_UInt, CompositeType_Vector, 4); break;
		
		case GL_FLOAT_VEC2:  set(PrimitveType_Float,  CompositeType_Vector, 2); break;
		case GL_FLOAT_VEC3:  set(PrimitveType_Float,  CompositeType_Vector, 3); break;
		case GL_FLOAT_VEC4:  set(PrimitveType_Float,  CompositeType_Vector, 4); break;
		
		case GL_DOUBLE_VEC2: set(PrimitveType_Double, CompositeType_Vector, 2); break;
		case GL_DOUBLE_VEC3: set(PrimitveType_Double, CompositeType_Vector, 3); break;
		case GL_DOUBLE_VEC4: set(PrimitveType_Double, CompositeType_Vector, 4); break;
		
		
	}
}

TypeInfo::TypeInfo( PrimitveType primitive, CompositeType composite, int compositeSize ) :
	m_PrimitiveType(primitive),
	m_CompositeType(composite),
	m_CompositeSize(compositeSize)
{
}

void TypeInfo::set( PrimitveType primitive, CompositeType composite, int compositeSize )
{
	m_PrimitiveType = primitive;
	m_CompositeType = composite;
	m_CompositeSize = compositeSize;
}

PrimitveType TypeInfo::primitveType() const;
CompositeType TypeInfo::compositeType() const;

int TypeInfo::compositeSize() const;
int TypeInfo::componentCount() const;
int TypeInfo::sizeInBytes() const;


}
}