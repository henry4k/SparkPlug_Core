#ifndef __SPARKPLUG_GL_DATATYPE__
#define __SPARKPLUG_GL_DATATYPE__

#include <SparkPlug/GL/OpenGL.h>

namespace SparkPlug
{
namespace GL
{

enum PrimitiveDataType
{
	PrimitiveDataType_Bool,
	PrimitiveDataType_Byte,
	PrimitiveDataType_UByte,
	PrimitiveDataType_Short,
	PrimitiveDataType_UShort,
	PrimitiveDataType_Int,
	PrimitiveDataType_UInt,
	PrimitiveDataType_Float,
	PrimitiveDataType_Double
};
const char* AsString( PrimitiveDataType type );
GLenum ConvertToGL( PrimitiveDataType type );
PrimitiveDataType PrimitiveDataTypeFromGL( GLenum e );
int SizeOf( PrimitiveDataType type );

enum CompositeDataType
{
	CompositeDataType_None,
	CompositeDataType_Vector,
	CompositeDataType_Matrix
};
const char* AsString( CompositeDataType type );

class DataType
{
public:
	DataType();
	DataType( GLenum e );
	DataType( const char* def );
	DataType( const char* def, int length );
	DataType( PrimitiveDataType primitive, CompositeDataType composite, int compositeSize );
	
	PrimitiveDataType primitveType() const;
	CompositeDataType compositeType() const;
	
	int compositeSize() const;
	int componentCount() const;
	int sizeInBytes() const;

	std::string toString() const;
	GLenum toGLenum() const;

	bool operator == ( const DataType& other ) const;
	bool operator != ( const DataType& other ) const;
	
private:
	void set( PrimitiveDataType primitive, CompositeDataType composite, int compositeSize );
	void setByDef( const char* def, int length );
	
	PrimitiveDataType m_PrimitiveDataType;
	CompositeDataType m_CompositeDataType;
	              int m_CompositeSize;
};

}
}

#endif
