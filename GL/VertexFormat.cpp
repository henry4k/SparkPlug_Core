#include <SparkPlug/GL/VertexFormat.h>


namespace SparkPlug
{
namespace GL
{

/// ---- DataType ----

const char* AsString( DataType type )
{
	switch(type)
	{
		case DataType_UInt8:
			return "UInt8";
		case DataType_UInt16:
			return "UInt16";
		case DataType_UInt32:
			return "UInt32";
		case DataType_Float32:
			return "Float32";
		case DataType_Float64:
			return "Float64";
		default:
			;
	}
	return "UnknownDataType";
}

GLenum ConvertToGL( DataType type )
{
	switch(type)
	{
		case DataType_UInt8:
			return GL_UNSIGNED_BYTE;
		case DataType_UInt16:
			return GL_UNSIGNED_SHORT;
		case DataType_UInt32:
			return GL_UNSIGNED_INT;
		case DataType_Float32:
			return GL_FLOAT;
		case DataType_Float64:
			return GL_DOUBLE;
		default:
			;
	}
	FatalError("Invalid data type: %u", type);
	return 0;
}

int SizeOf( DataType type )
{
	switch(type)
	{
		case DataType_UInt8:
			return sizeof(GLubyte);
		case DataType_UInt16:
			return sizeof(GLushort);
		case DataType_UInt32:
			return sizeof(GLuint);
		case DataType_Float32:
			return sizeof(GLfloat);
		case DataType_Float64:
			return sizeof(GLdouble);
		default:
			;
	}
	FatalError("Invalid index type: %u", type);
	return 0;
}


/// ---- VertexAttribute ----

VertexAttribute::VertexAttribute( int count, DataType type ) :
	m_Count(count),
	m_Type(type)
{
}

VertexAttribute::~VertexAttribute()
{
}

bool VertexAttribute::operator == ( const VertexAttribute& format ) const
{
	return
		(m_Count == format.m_Count) &&
		(m_Type == format.m_Type);
}

bool VertexAttribute::operator != ( const VertexAttribute& format ) const
{
	return !(*this == format);
}

int VertexAttribute::componentCount() const
{
	return m_Count;
}

DataType VertexAttribute::componentType() const
{
	return m_Type;
}

int VertexAttribute::sizeInBytes() const
{
	return SizeOf(m_Type)*m_Count;
}


/// ---- VertexFormat ----

VertexFormat::VertexFormat()
{
}

VertexFormat::VertexFormat( const char* def )
{
	/* Type:
		v: vertex position
		c: color
		e: edge flag
		f: fog coordinate
		n: normal
		s: secondary color
		t: texture coordinate
		//0-15g: generic
	*/
	
	/* DataType
		b: signed byte
		B: unsigned byte
		s: signed short
		S: unsigned short
		i: signed int
		I: unsigned int
		f: float
		d: double
	*/
	
	// <Type><Count><DataType>
	
	char attributeType;
	int  count;
	DataType dataType;
	
	int mode = 0;
	
	for(; *def != '\0'; ++def)
	{
		if(*def == ' ')
			continue;
		
		switch(mode)
		{
			case 0: // attributeType
			{
				attributeType = *def;
				mode = 1;
			} break;
			
			case 1: // count
			{
				count = *def - '0';
				assert(InclusiveInside(1,count,4));
				mode = 2;
			} break;
			
			case 2: // dataType
			{
				switch(*def)
				{
					case 'b':
					case 'B':
						dataType = DataType_UInt8;
						break;
					case 's':
					case 'S':
						dataType = DataType_UInt16;
						break;
					case 'i':
					case 'I':
						dataType = DataType_UInt32;
						break;
					case 'f':
						dataType = DataType_Float32;
						break;
					case 'd':
						dataType = DataType_Float64;
						break;
					default:
						FatalError("Unknown data type %c", *def);
						assert(false);
				}
				
				appendAttribute(VertexAttribute(count, dataType));

				mode = 0;
				
			} break;
			
			default:
				FatalError("Invalid mode");
		}
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
	// TODO
	FatalError("VertexFormat::asString() is yet unimplemented");
	return "Unimplemented";
}






const VertexFormat VertexFormat::V3("v3f");
const VertexFormat VertexFormat::V3N3("v3f n3f");
const VertexFormat VertexFormat::V3N3T2("v3f n3f t2f");
const VertexFormat VertexFormat::V3T2("v3f t2f");
const VertexFormat VertexFormat::V3C4("v3f c4f");
const VertexFormat VertexFormat::V3N3C4("v3f n3f c4f");
const VertexFormat VertexFormat::V3N3T2C4("v3f n3f t2f c4f");





}
}
