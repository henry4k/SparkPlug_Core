#ifndef __SPARKPLUG_GL_VERTEX_FORMAT__
#define __SPARKPLUG_GL_VERTEX_FORMAT__

#include <vector>
#include <SparkPlug/Common.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/DataType.h>


namespace SparkPlug
{
namespace GL
{


class VertexAttribute
{
public:
	VertexAttribute();
	VertexAttribute( const char* name, const DataType& type, bool normalize );
	VertexAttribute( const char* def, int length );
	VertexAttribute( const char* def );
	virtual ~VertexAttribute();
	
	bool operator == ( const VertexAttribute& format ) const;
	bool operator != ( const VertexAttribute& format ) const;
	
	const char* name() const;
	const DataType& componentType() const;
	
	std::string asString() const;
	
private:
	void setByDef( const char* name, int length );
	
	std::string m_Name;
	   DataType m_Type;
	       bool m_Normalize;
};


class VertexFormat
{
public:
	VertexFormat();
	VertexFormat( const char* def );
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
