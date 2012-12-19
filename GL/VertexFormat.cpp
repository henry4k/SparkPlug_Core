#include <cctype>
#include <cstring>
#include <SparkPlug/GL/VertexFormat.h>


namespace SparkPlug
{
namespace GL
{


/// ---- VertexAttribute ----

VertexAttribute::VertexAttribute()
{
}

VertexAttribute::VertexAttribute( const char* name, const DataType& type, bool normalize ) :
	m_Name(name),
	m_Type(type),
	m_Normalized(normalize)
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

// "Position:vec3f TexCoord:nvec2I"
void VertexAttribute::setByDef( const char* def, int length )
{
	std::vector<char> buf;
	DataType type;
	
	m_Normalized = false;
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
					buf.push_back('\0');
					m_Name = buf.data();
					buf.clear();
					mode = 1;
				}
				else
					buf.push_back(ch);
			} break;
			
			case 1: // data type def
			{
				if(ch == 'n' && buf.size() == 0)
				{
					m_Normalized = true;
				}
				else
				{
					buf.push_back(ch);
					if(i == length-1)
					{
						m_Type = DataType(buf.data(), buf.size());
						break;
					}
				}
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
		(m_Type == format.m_Type) &&
		(m_Normalized == format.m_Normalized);
}

bool VertexAttribute::operator != ( const VertexAttribute& format ) const
{
	return !(*this == format);
}

const char* VertexAttribute::name() const
{
	return m_Name.c_str();
}

const DataType& VertexAttribute::dataType() const
{
	return m_Type;
}

bool VertexAttribute::isNormalized() const
{
	return m_Normalized;
}

std::string VertexAttribute::asString() const
{
	std::string buf = m_Name+":";
	if(m_Normalized)
		buf += "n";
	return buf+m_Type.toString();
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
		bytes += i->dataType().sizeInBytes();
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


const VertexFormat VertexFormat::V3      ("Position:vec3f");
const VertexFormat VertexFormat::V3N3    ("Position:vec3f Normal:vec3f");
const VertexFormat VertexFormat::V3N3T2  ("Position:vec3f Normal:vec3f TexCoord:vec2f");
const VertexFormat VertexFormat::V3T2    ("Position:vec3f TexCoord:vec2f");
const VertexFormat VertexFormat::V3C4    ("Position:vec3f Color:vec4f");
const VertexFormat VertexFormat::V3N3C4  ("Position:vec3f Normal:vec3f Color:vec4f");
const VertexFormat VertexFormat::V3N3T2C4("Position:vec3f Normal:vec3f TexCoord:vec2f Color:vec4f");



}
}
