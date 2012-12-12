#ifndef __SPARKPLUG_TEXTURE__
#define __SPARKPLUG_TEXTURE__ 

#include <SparkPlug/Vector.h>
#include <SparkPlug/Reference.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Enums.h>
#include <SparkPlug/GL/Object.h>




namespace SparkPlug
{

class Image;
class PixelFormat;

namespace GL
{

class SamplerBase : public Object
{
public:
	SamplerBase( Context* context ) : Object(context) {}
	virtual ~SamplerBase() {}

	TextureFilter filter() const;
	virtual void setFilter( TextureFilter filter );
	
	TextureAddressMode addressMode() const;
	virtual void setAddressMode( TextureAddressMode mode );
	
	float maxAnisotropic() const;
	virtual void setMaxAnisotropic( float level );

private:
	TextureFilter      m_Filter;
	TextureAddressMode m_AddressMode;
	float              m_MaxAnisotropic;
};

class Texture : public SamplerBase
{
public:
	static StrongRef<Texture> Create( Context* context, TextureType type );
	static StrongRef<Texture> CreateFromImage( Context* context, TextureType type, const Image& image );
	virtual ~Texture();
	
	TextureType type() const;
	bool isPowerOfTwo() const;
	
	bool hasMipMaps() const;
	int mipMapLevels() const;
	
// 	vec3i size( int mipMapLevel = 0 ) const;
	
	void setFilter( TextureFilter f );
	void setAddressMode( TextureAddressMode m );
	void setMaxAnisotropic( float level );
	
private:
	Texture( Context* context, TextureType type );
	static bool UploadTextureRaw( TextureType type, bool proxy, int level, const PixelFormat& format, bool sRGB, vec3i size, bool border, const void* data );
	static bool TestTextureCreation( TextureType type, vec3i size, PixelFormat format, bool sRGB );
	
	TextureType m_Type;
	int         m_MipMapLevels;
	vec3i       m_Size;
};

}
}

#endif
