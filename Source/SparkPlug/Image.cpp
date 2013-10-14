#include <SparkPlug/Common.h>
#include <SparkPlug/Logic.h>
#include <SparkPlug/Image.h>

namespace SparkPlug
{

Image::Image() :
	m_Format(PixelFormat::Invalid),
	m_Width(0),
	m_Height(0),
	m_Depth(0),
	m_Data(NULL)
{
}

Image::Image( PixelFormat format, int width, int height, int depth ) :
	m_Format(format),
	m_Width(width),
	m_Height(height),
	m_Depth(depth)
{
	m_Data = new char[ m_Format.pixelSize()*width*height*depth ];
}

Image::~Image()
{
	if(m_Data)
		delete[] m_Data;
}

void Image::initFromRaw( PixelFormat format, int width, int height, int depth, void* pixels )
{
	if(m_Data)
		delete[] m_Data;
	
	m_Format = format;
	m_Width = width;
	m_Height = height;
	m_Depth = depth;
	m_Data = (char*)pixels;
	
	// .. deal with it.
}

const PixelFormat& Image::format() const
{
	return m_Format;
}

int Image::width() const
{
	return m_Width;
}

int Image::height() const
{
	return m_Height;
}

int Image::depth() const
{
	return m_Depth;
}

void* Image::pixels()
{
	return m_Data;
}

const void* Image::pixels() const
{
	return m_Data;
}

void* Image::pixelAt( int x, int y, int z )
{
	return m_Data + m_Format.pixelSize()*ArrayIndex(x,y,z, m_Width,m_Height,m_Depth);
}

const void* Image::pixelAt( int x, int y, int z ) const
{
	return m_Data + m_Format.pixelSize()*ArrayIndex(x,y,z, m_Width,m_Height,m_Depth);
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
