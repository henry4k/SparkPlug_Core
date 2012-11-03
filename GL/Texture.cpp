#include <SparkPlug/Common.h>
#include <SparkPlug/Math.h>
#include <SparkPlug/Pixel.h>
#include <SparkPlug/GL/Pixel.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Texture.h>

namespace SparkPlug
{
namespace GL
{

const char* AsString( TextureType type )
{
	switch(type)
	{
		case TextureType_1D:
			return "1D";
		
		case TextureType_2D:
			return "2D";
		
		case TextureType_3D:
			return "3D";
		
		case TextureType_Rect:
			return "Rect";
		
		case TextureType_CubeMap:
			return "CubeMap";
		
		default:
			return "UnknownTextureType";
	}
}

GLenum ConvertToGL( TextureType type )
{
	switch(type)
	{
		case TextureType_1D:
			return GL_TEXTURE_1D;
		
		case TextureType_2D:
			return GL_TEXTURE_2D;
		
		case TextureType_3D:
			return GL_TEXTURE_3D;
		
		case TextureType_Rect:
			return GL_TEXTURE_RECTANGLE;
		
		case TextureType_CubeMap:
			return GL_TEXTURE_CUBE_MAP;
		
		default:
			;
	}
	
	FatalError("Invalid texture type: %u", type);
	return 0;
}

GLenum ConvertToProxyGL( TextureType type )
{
	switch(type)
	{
		case TextureType_1D:
			return GL_PROXY_TEXTURE_1D;
		
		case TextureType_2D:
			return GL_PROXY_TEXTURE_2D;
		
		case TextureType_3D:
			return GL_PROXY_TEXTURE_3D;
		
		case TextureType_Rect:
			return GL_PROXY_TEXTURE_RECTANGLE;
		
		case TextureType_CubeMap:
			return GL_PROXY_TEXTURE_CUBE_MAP;
		
		default:
			;
	}
	
	FatalError("Invalid texture type: %u", type);
	return 0;
}

const char* AsString( TextureFilter filter )
{
	switch(filter)
	{
		case TextureFilter_Nearest:
			return "Nearest";
		
		case TextureFilter_Bilinear:
			return "Bilinear";
		
		case TextureFilter_Trilinear:
			return "Trilinear";
		
		default:
			return "UnknownTextureFilter";
	}
}

GLint ConvertToGL( TextureFilter filter, bool usingMipMaps )
{
	switch(filter)
	{
		case TextureFilter_Nearest:
			if(usingMipMaps)
				return GL_NEAREST_MIPMAP_NEAREST;
			else
				return GL_NEAREST;
		
		case TextureFilter_Bilinear:
			if(usingMipMaps)
				return GL_LINEAR_MIPMAP_NEAREST;
			else
				return GL_LINEAR;
		
		case TextureFilter_Trilinear:
			if(usingMipMaps)
				return GL_LINEAR_MIPMAP_LINEAR;
			else
				return GL_LINEAR;
		
		default:
			;
	}
	
	FatalError("Invalid texture filter: %u", filter);
	return 0;
}

const char* AsString( TextureAddressMode mode )
{
	switch(mode)
	{
		case TextureAddressMode_Repeat:
			return "Repeat";
		
		case TextureAddressMode_Clamp:
			return "Clamp";
		
		default:
			return "UnknownTextureAddressMode";
	}
}

GLenum ConvertToGL( TextureAddressMode mode )
{
	switch(mode)
	{
		case TextureAddressMode_Repeat:
			return GL_REPEAT;
		
		case TextureAddressMode_Clamp:
			return GL_CLAMP_TO_EDGE;
		
		default:
			;
	}
	
	FatalError("Invalid texture address mode %u", mode);
	return 0;
}

GLenum ConvertToGL( TextureCubeFace face )
{
	switch(face)
	{
		case TextureCubeFace_Positive_X:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		
		case TextureCubeFace_Negative_X:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		
		case TextureCubeFace_Positive_Y:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		
		case TextureCubeFace_Negative_Y:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		
		case TextureCubeFace_Positive_Z:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		
		case TextureCubeFace_Negative_Z:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		
		case TextureCubeFace_None:
		default:
			;
	}
	
	FatalError("Invalid texture cube face: %u", face);
	return 0;
}




TextureFilter SamplerBase::filter() const
{
	return m_Filter;
}

void SamplerBase::setFilter( TextureFilter filter )
{
	m_Filter = filter;
}

TextureAddressMode SamplerBase::addressMode() const
{
	return m_AddressMode;
}

void SamplerBase::setAddressMode( TextureAddressMode mode )
{
	m_AddressMode = mode;
}

float SamplerBase::maxAnisotropic() const
{
	return m_MaxAnisotropic;
}

void SamplerBase::setMaxAnisotropic( float level )
{
	m_MaxAnisotropic = level;
}

// vec3i ConvertImageToTextureSize( TextureType type, vec3i imageSize )
// {
// 	switch(type)
// 	{
// 		case TextureType_1D:
// 		{
// 			assert((imageSize.x >= 1) && (imageSize.y == 1) && (imageSize.z == 1));
// 			
// 		}
// 	}
// }

bool Texture::TestTextureCreation( TextureType type, vec3i size, PixelFormat format, bool useSRGB )
{
	switch(type)
	{
		case TextureType_1D:
			glTexImage1D(
				ConvertToProxyGL(type),
				0,
				ConvertToGL(format, useSRGB),
				size.x,
				0,
				ConvertToGL(format.semantic()),
				ConvertToGL(format.componentType()),
				NULL);
			break;
		
		case TextureType_3D:
			glTexImage3D(
				ConvertToProxyGL(type),
				0,
				ConvertToGL(format, useSRGB),
				size.x,
				size.y,
				size.z,
				0,
				ConvertToGL(format.semantic()),
				ConvertToGL(format.componentType()),
				NULL);
			break;
		
		case TextureType_2D:
		case TextureType_Rect:
		case TextureType_CubeMap:
			glTexImage2D(
				ConvertToProxyGL(type),
				0,
				ConvertToGL(format, useSRGB),
				size.x,
				size.y,
				0,
				ConvertToGL(format.semantic()),
				ConvertToGL(format.componentType()),
				NULL);
			break;
		
		default:
			FatalError("Invalid texture type: %u", type);
	}
	
	GLint width;
	glGetTexLevelParameteriv(
		ConvertToProxyGL(type),
		0,
		GL_TEXTURE_WIDTH,
		&width);
	
	if(width == 0)
	{
		LogError("Cannot create %s texture:", AsString(type));
		LogError("Size: %d, %d, %d", size.x, size.y, size.z);
		LogError("Format: %s (sRGB=%d)", format.asString().c_str(), (int)useSRGB);
		return false;
	}
	return true;
}

StrongRef<Texture> Texture::Create( Context* context, TextureType type )
{
	Texture* texture = new Texture(context, type);
	return StrongRef<Texture>(texture);
}

StrongRef<Texture> Texture::CreateFromImage( Context* context, TextureType type, const Image& image )
{
	Texture* texture = new Texture(context, type);
	
// 	bind(0);
// 	switch(type)
// 	{
// 		case TextureType_1D:
// 			glTexImage2D(GL_TEXTURE_2D, 0, format, img->getSize().x, img->getSize().y, 0, format, img->getType(), img->getData());
// 			break;
// 		
// 		case TextureType_2D:
// 			glTexImage2D(GL_TEXTURE_2D, 0, format, img->getSize().x, img->getSize().y, 0, format, img->getType(), img->getData());
// 			break;
// 	}
	
	return StrongRef<Texture>(texture);
}

Texture::Texture( Context* context, TextureType type ) :
	SamplerBase(context),
	m_Type(type)
{
	glGenTextures(1, &m_Handle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Handle);
}

TextureType Texture::type() const
{
	return m_Type;
}

bool Texture::is1D() const
{
	return m_Type == TextureType_1D;
}

bool Texture::is2D() const
{
	return
		(m_Type == TextureType_2D) ||
		(m_Type == TextureType_Rect);
}

bool Texture::is3D() const
{
	return m_Type == TextureType_3D;
}

bool Texture::isCube() const
{
	return m_Type == TextureType_CubeMap;
}

bool Texture::isPowerOfTwo() const
{
	return
		IsPowerOfTwo(m_Size.x) &&
		IsPowerOfTwo(m_Size.y) &&
		IsPowerOfTwo(m_Size.z);
}

bool Texture::hasMipMaps() const
{
	return m_MipMapLevels > 1;
}

int Texture::mipMapLevels() const
{
	return m_MipMapLevels;
}

void Texture::setFilter( TextureFilter filter )
{
// 	bind();
	// ...
	SamplerBase::setFilter(filter);
}

void Texture::setAddressMode( TextureAddressMode mode )
{
// 	bind();
	// ...
	SamplerBase::setAddressMode(mode);
}

void Texture::setMaxAnisotropic( float level )
{
// 	bind();
	// ...
	SamplerBase::setMaxAnisotropic(level);
}

}
}
