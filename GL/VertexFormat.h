#ifndef __SPARKPLUG_GL_VERTEX_FORMAT__
#define __SPARKPLUG_GL_VERTEX_FORMAT__

#include <vector>
#include <SparkPlug/Common.h>
#include <SparkPlug/GL/OpenGL.h>


namespace SparkPlug
{
namespace GL
{


enum AttributeType
{
	AttributeType_UInt8,
	AttributeType_NormalizedUInt8,
	AttributeType_UInt16,
	AttributeType_NormalizedUInt16,
	AttributeType_UInt32,
	AttributeType_NormalizedUInt32,
	AttributeType_Float32,
	AttributeType_Float64
};
const char* AsString( AttributeType type );
GLenum ConvertToGL( AttributeType type );
AttributeType AttributeTypeFromGL( GLenum e );
int SizeOf( AttributeType type );
bool IsNormalized( AttributeType type );
char AsChar( AttributeType type );
AttributeType AttributeTypeByChar( char ch );


class VertexAttribute
{
public:
	VertexAttribute();
	VertexAttribute( const char* name, int count, AttributeType type );
	VertexAttribute( const char* def, int length );
	VertexAttribute( const char* def );
	virtual ~VertexAttribute();
	
	bool operator == ( const VertexAttribute& format ) const;
	bool operator != ( const VertexAttribute& format ) const;
	
	  const char* name() const;
	          int componentCount() const;
	AttributeType componentType() const;
	          int sizeInBytes() const;
		  
	std::string asString() const;
	
private:
	void setByDef( const char* name, int length );
	
	std::string m_Name;
	        int m_Count;
	   AttributeType m_Type;
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
