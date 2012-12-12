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
	
	std::string asString() const;
	
	static const PixelFormat Invalid;
	static const PixelFormat L8;
	static const PixelFormat LA8;
	static const PixelFormat RGB8;
	static const PixelFormat RGBA8;
	
// 	static const PixelFormat L8;
// 	static const PixelFormat L16;
// 	static const PixelFormat L16F;
// 	static const PixelFormat L32F;
// 	static const PixelFormat LA8;
// 	static const PixelFormat LA16;
// 	static const PixelFormat LA16F;
// 	static const PixelFormat LA32F;
// 	static const PixelFormat RGB8;
// 	static const PixelFormat RGB16;
// 	static const PixelFormat RGB16F;
// 	static const PixelFormat RGB32F;
// 	static const PixelFormat RGBA8;
// 	static const PixelFormat RGBA16;
// 	static const PixelFormat RGBA16F;
// 	static const PixelFormat RGBA32F;
// 	static const PixelFormat DEPTH16;
// 	static const PixelFormat DEPTH24;
// 	static const PixelFormat DEPTH32;
// 	static const PixelFormat DEPTH16F;
// 	static const PixelFormat DEPTH32F;
	
	
private:
	PixelSemantic  m_Semantic;
	PixelComponent m_Component;
};

}
#endif
