#include <cctype>
#include <cstring>
#include <SparkPlug/GL/VertexFormat.h>


namespace SparkPlug
{
namespace GL
{




struct ValueInfo
{
	DataType type;
	bool normalized;
	int components;
};



/// ---- AttributeType ----

const char* AsString( AttributeType type )
{
	switch(type)
	{
		case AttributeType_UInt8:
			return "UInt8";
		case AttributeType_NormalizedUInt8:
			return "normalized UInt8";
		case AttributeType_UInt16:
			return "UInt16";
		case AttributeType_NormalizedUInt16:
			return "normalized UInt16";
		case AttributeType_UInt32:
			return "UInt32";
		case AttributeType_NormalizedUInt32:
			return "normalized UInt32";
		case AttributeType_Float32:
			return "Float32";
		case AttributeType_Float64:
			return "Float64";
		default:
			;
	}
	return "UnknownAttributeType";
}

GLenum ConvertToGL( AttributeType type )
{
	switch(type)
	{
		case AttributeType_UInt8:
		case AttributeType_NormalizedUInt8:
			return GL_UNSIGNED_BYTE;
		case AttributeType_UInt16:
		case AttributeType_NormalizedUInt16:
			return GL_UNSIGNED_SHORT;
		case AttributeType_UInt32:
		case AttributeType_NormalizedUInt32:
			return GL_UNSIGNED_INT;
		case AttributeType_Float32:
			return GL_FLOAT;
		case AttributeType_Float64:
			return GL_DOUBLE;
		default:
			;
	}
	FatalError("Invalid attribute type: %u", type);
	return 0;
}

AttributeType AttributeTypeFromGL( GLenum e, bool normalized, int* countOut )
{
	int dummy = 0;
	if(countOut == NULL)
		countOut = &dummy;
	
	switch(e)
	{
		case GL_UNSIGNED_INT: *countOut = 1; return AttributeType_Float32;
		case GL_DOUBLE:       *countOut = 1; return AttributeType_Float64;
		
		case GL_FLOAT:           *countOut = 1; return AttributeType_Float32;
		case GL_FLOAT_VEC2_ARB:  *countOut = 2; return AttributeType_Float32;
		case GL_FLOAT_VEC3_ARB:  *countOut = 3; return AttributeType_Float32;
		case GL_FLOAT_VEC4_ARB:  *countOut = 4; return AttributeType_Float32;
		
		case GL_FLOAT_VEC2_ARB:  *countOut = 2; return AttributeType_Float32;
		case GL_DOUBLE_VEC2_ARB: *countOut = 2; return AttributeType_Float64;
		case GL_DOUBLE: return AttributeType_Float64;
		default: ;
	}
	FatalError("Invalid gl attribute type: %u", e);
	return AttributeType_Float32;
}

AttributeType AttributeTypeFromGL( GLenum e, bool normalized )
{
	switch(e)
	{
		case GL_UNSIGNED_BYTE:
			return normalized ? AttributeType_NormalizedUInt8 : AttributeType_UInt8;
		case GL_UNSIGNED_SHORT:
			return normalized ? AttributeType_NormalizedUInt16 : AttributeType_UInt16;
		case GL_UNSIGNED_INT:
			return normalized ? AttributeType_NormalizedUInt32 : AttributeType_UInt32;
		case GL_FLOAT:
			return AttributeType_Float32;
		case GL_DOUBLE:
			return AttributeType_Float64;
		default:
			;
	}
	FatalError("Invalid gl attribute type: %u", e);
	return AttributeType_Float32;
}

int SizeOf( AttributeType type )
{
	switch(type)
	{
		case AttributeType_UInt8:
		case AttributeType_NormalizedUInt8:
			return sizeof(GLubyte);
		case AttributeType_UInt16:
		case AttributeType_NormalizedUInt16:
			return sizeof(GLushort);
		case AttributeType_UInt32:
		case AttributeType_NormalizedUInt32:
			return sizeof(GLuint);
		case AttributeType_Float32:
			return sizeof(GLfloat);
		case AttributeType_Float64:
			return sizeof(GLdouble);
		default:
			;
	}
	FatalError("Invalid attribute type: %u", type);
	return 0;
}

bool IsNormalized( AttributeType type )
{
	switch(type)
	{
		case AttributeType_NormalizedUInt8:
		case AttributeType_NormalizedUInt16:
		case AttributeType_NormalizedUInt32:
			return true;
		default: ;
	}
	return false;
}

char AsChar( AttributeType type )
{
	switch(type)
	{
		case AttributeType_UInt8:            return 'B';
		case AttributeType_NormalizedUInt8:  return 'b';
		case AttributeType_UInt16:           return 'S';
		case AttributeType_NormalizedUInt16: return 's';
		case AttributeType_UInt32:           return 'I';
		case AttributeType_NormalizedUInt32: return 'i';
		case AttributeType_Float32:          return 'f';
		case AttributeType_Float64:          return 'd';
		default: ;
	}
	FatalError("Invalid attribute type: %u", type);
	return '?';
}

AttributeType AttributeTypeByChar( char ch )
{
	switch(ch)
	{
		case 'B': return AttributeType_UInt8;
		case 'b': return AttributeType_NormalizedUInt8;
		case 'S': return AttributeType_UInt16;
		case 's': return AttributeType_NormalizedUInt16;
		case 'I': return AttributeType_UInt32;
		case 'i': return AttributeType_NormalizedUInt32;
		case 'f': return AttributeType_Float32;
		case 'd': return AttributeType_Float64;
		default: ;
	}
	FatalError("Invalid attribute type char: %c", ch);
	return AttributeType_UInt8;
}


/// ---- VertexAttribute ----

VertexAttribute::VertexAttribute()
{
}

VertexAttribute::VertexAttribute( const char* name, int count, AttributeType type ) :
	m_Name(name),
	m_Count(count),
	m_Type(type)
{
}

VertexAttribute::VertexAttribute( const char* def, int length )
{
	setByDef(def, length);
}

VertexAttribute::VertexAttribute( const char* def )
{
	setByDef(def, std::strlen(def));
}

VertexAttribute::~VertexAttribute()
{
}

void VertexAttribute::setByDef( const char* def, int length )
{
	std::vector<char> nameBuffer;
	int count;
	AttributeType type;
	
	int mode = 0;
	
	for(int i = 0; i < length; ++i)
	{
		char ch = def[i];
		
		switch(mode)
		{
			case 0: // name
			{
				if(ch == ':')
				{
					nameBuffer.push_back('\0');
					m_Name = nameBuffer.data();
					mode = 1;
				}
				else
					nameBuffer.push_back(ch);
			} break;
			
			case 1: // count
			{
				m_Count = ch - '0';
				assert(InclusiveInside(1,m_Count,4));
				mode = 2;
			} break;
			
			case 2: // type
			{
				m_Type = AttributeTypeByChar(ch);
				return;
			} break;
			
			default:
				FatalError("Invalid mode");
		}
	}
}

bool VertexAttribute::operator == ( const VertexAttribute& format ) const
{
	return
		(m_Name == format.m_Name) &&
		(m_Count == format.m_Count) &&
		(m_Type == format.m_Type);
}

bool VertexAttribute::operator != ( const VertexAttribute& format ) const
{
	return !(*this == format);
}

const char* VertexAttribute::name() const
{
	return m_Name.c_str();
}

int VertexAttribute::componentCount() const
{
	return m_Count;
}

AttributeType VertexAttribute::componentType() const
{
	return m_Type;
}

int VertexAttribute::sizeInBytes() const
{
	return SizeOf(m_Type)*m_Count;
}

std::string VertexAttribute::asString() const
{
	const char buf[] =
	{
		':',
		(char)('0'+m_Count),
		AsChar(m_Type),
		'\0'
	};
	
	return m_Name+buf;
}


/// ---- VertexFormat ----

VertexFormat::VertexFormat()
{
}

VertexFormat::VertexFormat( const char* def )
{
	int begin = 0;
	int i = 0;
	bool wasSpace = true;
	
	for(; def[i] != '\0'; ++i)
	{
		char ch = def[i];
		bool isSpace = std::isspace(ch);
		
		// Wechsel von WS
		if(wasSpace && !isSpace)
		{
			begin = i;
		}
		// Wechsel zu WS
		else if(!wasSpace && isSpace)
		{
			assert((i-begin) >= 0);
			appendAttribute(VertexAttribute(&def[begin], i-begin));
			begin = -1;
		}
		
		wasSpace = isSpace;
	}
	if(begin >= 0)
	{
		assert((i-begin) >= 0);
		appendAttribute(VertexAttribute(&def[begin], i-begin));
	}
}

VertexFormat::VertexFormat( const VertexFormat& source ) :
	m_Attributes(source.m_Attributes)
{
}

VertexFormat& VertexFormat::operator=( const VertexFormat& source )
{
	m_Attributes = source.m_Attributes;
	return *this;
}

bool VertexFormat::isValid() const
{
	return sizeInBytes() != 0;
}

bool VertexFormat::operator==( const VertexFormat& format ) const
{
	return m_Attributes == format.m_Attributes;
}

bool VertexFormat::operator!=( const VertexFormat& format ) const
{
	return !(*this == format);
}

int VertexFormat::attributeCount() const
{
	return m_Attributes.size();
}

const VertexAttribute& VertexFormat::attribute( int i ) const
{
	return m_Attributes[i];
}

void VertexFormat::appendAttribute( const VertexAttribute& attribute )
{
	m_Attributes.push_back(attribute);
}

int VertexFormat::sizeInBytes() const
{
	int bytes = 0;
	std::vector<VertexAttribute>::const_iterator i = m_Attributes.begin();
	for(; i != m_Attributes.end(); ++i)
		bytes += i->sizeInBytes();
	return bytes;
}

std::string VertexFormat::asString() const
{
	std::string buf("(");
	
	std::vector<VertexAttribute>::const_iterator i = m_Attributes.begin();
	for(; i != m_Attributes.end(); ++i)
	{
		if(i != m_Attributes.begin())
			buf += " ";
		buf += i->asString();
	}
	buf += ")";
	return buf;
}


const VertexFormat VertexFormat::V3      ("Position:3f");
const VertexFormat VertexFormat::V3N3    ("Position:3f Normal:3f");
const VertexFormat VertexFormat::V3N3T2  ("Position:3f Normal:3f TexCoord:2f");
const VertexFormat VertexFormat::V3T2    ("Position:3f TexCoord:2f");
const VertexFormat VertexFormat::V3C4    ("Position:3f Color:4f");
const VertexFormat VertexFormat::V3N3C4  ("Position:3f Normal:3f Color:4f");
const VertexFormat VertexFormat::V3N3T2C4("Position:3f Normal:3f TexCoord:2f Color:4f");



}
}
