#ifndef __SPARKPLUG_PIXEL__
#define __SPARKPLUG_PIXEL__

#include <SparkPlug/Common.h>


namespace SparkPlug
{

enum PixelSemantic
{
	PixelSemantic_Luminance,
	PixelSemantic_LuminanceAlpha,
	PixelSemantic_RGB,
	PixelSemantic_RGBA,
	PixelSemantic_Depth
};
const char* AsString( PixelSemantic semantic );

enum PixelComponent
{
	PixelComponent_UInt8,
	PixelComponent_UInt16,
	PixelComponent_UInt24,
	PixelComponent_UInt32,
	PixelComponent_Float16,
	PixelComponent_Float32
};
const char* AsString( PixelComponent component );

class PixelFormat
{
public:
	PixelFormat( PixelSemantic semantic, PixelComponent component );
	PixelFormat( const PixelFormat& source );
	PixelFormat& operator = ( const PixelFormat& source );
	
	bool isValid() const;
	
	bool operator == ( const PixelFormat& format ) const;
	bool operator != ( const PixelFormat& format ) const;
	
	PixelSemantic semantic() const;
	PixelComponent componentType() const;
	
	int pixelSize() const;
	int componentSize() const;
	
	int componentCount() const;
	
	String asString() const;
	
private:
	PixelSemantic  m_Semantic;
	PixelComponent m_Component;
};

}
#endif
