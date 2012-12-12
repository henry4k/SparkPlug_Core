#include <SparkPlug/Common.h>
#include <SparkPlug/Logic.h>
#include <SparkPlug/Image.h>

namespace SparkPlug
{

Image::Image() :
	m_Format(PixelFormat::Invalid),
	m_Size(0, 0, 0),
	m_Data(NULL)
{
}

Image::Image( PixelFormat format, vec3i size ) :
	m_Format(format),
	m_Size(size)
{
	m_Data = new char[ m_Format.pixelSize()*m_Size.volume() ];
}

Image::~Image()
{
	if(m_Data)
		delete[] m_Data;
}

void Image::initFromRaw( PixelFormat format, vec3i size, void* pixels )
{
	if(m_Data)
		delete[] m_Data;
	
	m_Format = format;
	m_Size = size;
	m_Data = (char*)pixels;
	
	// .. deal with it.
}

const PixelFormat& Image::format() const
{
	return m_Format;
}

vec3i Image::size() const
{
	return m_Size;
}

void* Image::pixels()
{
	return m_Data;
}

const void* Image::pixels() const
{
	return m_Data;
}

void* Image::pixelAt( vec3i pos )
{
	return m_Data + m_Format.pixelSize()*ArrayIndex(pos, m_Size);
}

const void* Image::pixelAt( vec3i pos ) const
{
	return m_Data + m_Format.pixelSize()*ArrayIndex(pos, m_Size);
}

Image::Image( const Image& source ) :
	m_Format(source.format())
{
	assert(false);
}

Image& Image::operator = ( const Image& source )
{
	assert(false);
	return *this;
}

}
