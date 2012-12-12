#ifndef __SPARKPLUG_GL_VERTEX_FORMAT__
#define __SPARKPLUG_GL_VERTEX_FORMAT__

#include <vector>
#include <SparkPlug/Common.h>


namespace SparkPlug
{
namespace GL
{


enum DataType
{
	DataType_UInt8,
	DataType_UInt16,
	DataType_UInt32,
	DataType_Float32,
	DataType_Float64
};


class VertexAttribute
{
public:
	VertexAttribute( int count, DataType type );
	virtual ~VertexAttribute();
	
	int componentCount() const;
	int componentType() const;
	int sizeInBytes() const;
	
private:
	int m_Count;
	DataType m_Type;
};


class VertexFormat
{
public:
	VertexFormat();
	VertexFormat( const VertexFormat& source );
	VertexFormat& operator = ( const VertexFormat& source );
	
	bool isValid() const;
	
	bool operator == ( const VertexFormat& format ) const;
	bool operator != ( const VertexFormat& format ) const;
	
	int attributeCount() const;
	const VertexAttribute& attribute( int i ) const;
	void appendAttribute( const VertexAttribute& attribute );

	int sizeInBytes() const;
	
	std::string asString() const;
	
	
	static const VertexFormat V3;
	static const VertexFormat V3N3;
	static const VertexFormat V3N3T2;
	static const VertexFormat V3T2;
	static const VertexFormat V3C4;
	static const VertexFormat V3N3C4;
	static const VertexFormat V3N3T2C4;
	
private:
	std::vector<VertexAttribute> m_Attributes;
};

}
}
#endif
