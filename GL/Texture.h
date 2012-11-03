#ifndef __SPARKPLUG_TEXTURE__
#define __SPARKPLUG_TEXTURE__ 

#include <SparkPlug/Vector.h>
#include <SparkPlug/Reference.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Object.h>




namespace SparkPlug
{

class Image;
class PixelFormat;

namespace GL
{

enum TextureType
{
	TextureType_1D,
	TextureType_2D,
	TextureType_3D,
	TextureType_Rect,
	TextureType_CubeMap,
	TextureType_Count
};
const char* AsString( TextureType type );
GLenum ConvertToGL( TextureType type );
GLenum ConvertToProxyGL( TextureType type );

enum TextureFilter
{
	TextureFilter_Nearest,
	TextureFilter_Bilinear,
	TextureFilter_Trilinear,
	TextureFilter_Count
};
const char* AsString( TextureFilter filter );
GLint ConvertToGL( TextureFilter filter, bool usingMipMaps );

enum TextureAddressMode
{
	TextureAddressMode_Repeat,
	TextureAddressMode_Clamp,
	TextureAddressMode_Count
};
const char* AsString( TextureAddressMode mode );
GLenum ConvertToGL( TextureAddressMode mode );

enum TextureCubeFace
{
	TextureCubeFace_None,
	TextureCubeFace_Positive_X,
	TextureCubeFace_Negative_X,
	TextureCubeFace_Positive_Y,
	TextureCubeFace_Negative_Y,
	TextureCubeFace_Positive_Z,
	TextureCubeFace_Negative_Z
};
// GLenum ConvertToGL( TextureCubeFace mode );


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
	bool is1D() const;
	bool is2D() const;
	bool is3D() const;
	bool isCube() const;
	bool isPowerOfTwo() const;
	
	bool hasMipMaps() const;
	int mipMapLevels() const;
	
// 	vec3i size( int mipMapLevel = 0 ) const;
	
	void setFilter( TextureFilter filter );
	void setAddressMode( TextureAddressMode mode );
	void setMaxAnisotropic( float level );
	
private:
	Texture( Context* context, TextureType type );
// 	Texture( Context* context, TextureType type, const Image& image, bool useMipMaps, bool sRGB );
	static bool TestTextureCreation( TextureType type, vec3i size, PixelFormat format, bool useSRGB );
	
	TextureType m_Type;
	int         m_MipMapLevels;
	vec3i       m_Size;
};

}
}

#endif
