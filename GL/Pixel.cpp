#include <SparkPlug/Common.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Pixel.h>

namespace SparkPlug
{
namespace GL
{

GLenum ConvertToGL( PixelComponent component )
{
	switch(component)
	{
		case PixelComponent_UInt8:
			return GL_UNSIGNED_BYTE;
		
		case PixelComponent_UInt16:
			return GL_UNSIGNED_SHORT;
		
		case PixelComponent_UInt32:
			return GL_UNSIGNED_INT;
		
		case PixelComponent_Float16:
			return GL_HALF_FLOAT;
		
		case PixelComponent_Float32:
			return GL_FLOAT;
		
		default:
			;
	}
	
	FatalError("Invalid pixel component: %u", component);
	return 0;
}

GLenum ConvertToGL( PixelSemantic semantic )
{
	switch(semantic)
	{
		case PixelSemantic_Luminance:
			return GL_LUMINANCE;
		
		case PixelSemantic_LuminanceAlpha:
			return GL_LUMINANCE_ALPHA;
		
		case PixelSemantic_RGB:
			return GL_RGB;
		
		case PixelSemantic_RGBA:
			return GL_RGBA;
		
		case PixelSemantic_Depth:
			return GL_DEPTH_COMPONENT;
		
		default:
			;
	}
	
	FatalError("Invalid pixel semantic: %u", semantic);
	return 0;
}

GLenum ConvertToGL( const PixelFormat& format, bool sRGB )
{
	switch(format.componentType())
	{
		case PixelComponent_UInt8:
		{
			switch(format.semantic())
			{
				case PixelSemantic_Luminance:
				{
					if(sRGB)
						return GL_SLUMINANCE8;
					else
						return GL_LUMINANCE8;
				}
				
				case PixelSemantic_LuminanceAlpha:
				{
					if(sRGB)
						return GL_SLUMINANCE8_ALPHA8;
					else
						return GL_LUMINANCE8_ALPHA8;
				}
				
				case PixelSemantic_RGB:
				{
					if(sRGB)
						return GL_SRGB8;
					else
						return GL_RGB8;
				}
				
				case PixelSemantic_RGBA:
				{
					if(sRGB)
						return GL_SRGB8_ALPHA8;
					else
						return GL_RGBA8;
				}
				
				default:
					;
			}
			
			break;
		}
		
		case PixelComponent_UInt16:
		{
			if(format.semantic() == PixelSemantic_Depth)
				return GL_DEPTH_COMPONENT16;
			break;
		}
		
		case PixelComponent_UInt24:
		{
			if(format.semantic() == PixelSemantic_Depth)
				return GL_DEPTH_COMPONENT24;
			break;
		}
		
		case PixelComponent_UInt32:
		{
			if(format.semantic() == PixelSemantic_Depth)
				return GL_DEPTH_COMPONENT32;
			break;
		}
		
		case PixelComponent_Float16:
		{
			switch(format.semantic())
			{
				case PixelSemantic_Luminance:
				{
					if(!GLEW_ARB_texture_float)
					{
						FatalError("GLEW_ARB_texture_float not supported");
						break;
					}
					
					return GL_LUMINANCE16F_ARB;
				}
				
				case PixelSemantic_LuminanceAlpha:
				{
					if(!GLEW_ARB_texture_float)
					{
						FatalError("GLEW_ARB_texture_float not supported");
						break;
					}
					
					return GL_LUMINANCE_ALPHA16F_ARB;
				}
				
				case PixelSemantic_RGB:
					return GL_RGB16F;
				
				case PixelSemantic_RGBA:
					return GL_RGBA16F;
				
				default:
					;
			}
			
			break;
		}
		
		case PixelComponent_Float32:
		{
			switch(format.semantic())
			{
				case PixelSemantic_Luminance:
				{
					if(!GLEW_ARB_texture_float)
					{
						FatalError("GLEW_ARB_texture_float not supported");
						break;
					}
					
					return GL_LUMINANCE32F_ARB;
				}
				
				case PixelSemantic_LuminanceAlpha:
				{
					if(!GLEW_ARB_texture_float)
					{
						FatalError("GLEW_ARB_texture_float not supported");
						break;
					}
					
					return GL_LUMINANCE_ALPHA32F_ARB;
				}
				
				case PixelSemantic_RGB:
					return GL_RGB32F;
				
				case PixelSemantic_RGBA:
					return GL_RGBA32F;
				
				default:
					;
			}
			
			break;
		}
		
		default:
			;
	}
	
	FatalError("Pixel format '%s' not supported!", format.asString().c_str());
	return 0;
}

}
}
