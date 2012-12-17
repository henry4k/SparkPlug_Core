#ifndef __SPARKPLUG_GL_DATATYPE__
#define __SPARKPLUG_GL_DATATYPE__

#include <SparkPlug/GL/OpenGL.h>

namespace SparkPlug
{
namespace GL
{

enum PrimitveType
{
	PrimitveType_Bool,
	PrimitveType_Byte,
	PrimitveType_UByte,
	PrimitveType_Short,
	PrimitveType_UShort,
	PrimitveType_Int,
	PrimitveType_UInt,
	PrimitveType_Float,
	PrimitveType_Double
};
const char* AsString( PrimitveType type );
GLenum ConvertToGL( PrimitveType type );
PrimitveType PrimitveTypeFromGL( GLenum e );

enum CompositeType
{
	CompositeType_None,
	CompositeType_Vector,
	CompositeType_Matrix
};
const char* AsString( CompositeType type );

class TypeInfo
{
public:
	TypeInfo();
	TypeInfo( GLenum e );
	TypeInfo( PrimitveType primitive, CompositeType composite, int compositeSize );
	
	 PrimitveType primitveType() const;
	CompositeType compositeType() const;
	
	int compositeSize() const;
	int componentCount() const;
	int sizeInBytes() const;
	
private:
	 PrimitveType m_PrimitiveType;
	CompositeType m_CompositeType;
	          int m_CompositeSize;
};

}
}

#endif