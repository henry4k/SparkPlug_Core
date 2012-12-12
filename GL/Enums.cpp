#include <SparkPlug/Common.h>
#include <SparkPlug/GL/Enums.h>


namespace SparkPlug
{
namespace GL
{

/// Texture ///

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
	}
	return "UnknownTextureType";
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
	}
	return "UnknownTextureFilter";
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
	}
	FatalError("Invalid texture filter: %u", filter);
	return 0;
}

const char* AsString( TextureAddressMode mode )
{
	switch(mode)
	{
		case TextureAddressMode_Repeat: return "Repeat";
		case TextureAddressMode_Clamp:  return "Clamp";
	}
	return "UnknownTextureAddressMode";
}

GLenum ConvertToGL( TextureAddressMode mode )
{
	switch(mode)
	{
		case TextureAddressMode_Repeat: return GL_REPEAT;
		case TextureAddressMode_Clamp:  return GL_CLAMP_TO_EDGE;
	}
	FatalError("Invalid texture address mode %u", mode);
	return 0;
}

GLenum ConvertToGL( TextureCubeFace face )
{
	switch(face)
	{
		case TextureCubeFace_Positive_X: return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case TextureCubeFace_Negative_X: return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case TextureCubeFace_Positive_Y: return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case TextureCubeFace_Negative_Y: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case TextureCubeFace_Positive_Z: return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case TextureCubeFace_Negative_Z: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		case TextureCubeFace_None: ;
	}
	FatalError("Invalid texture cube face: %u", face);
	return 0;
}



/// Shader ///

const char* AsString( ShaderType type )
{
	switch(type)
	{
		case ShaderType_Vertex:
			return "vertex";
		case ShaderType_Fragment:
			return "fragment";
		case ShaderType_Geometry:
			return "geometry";
		case ShaderType_TesselationControl:
			return "tesselation control";
		case ShaderType_TesselationEvaluation:
			return "tesselation evaluation";
		case ShaderType_Compute:
			return "compute";
	}
	return "invalid";
}

GLenum ConvertToGL( ShaderType type )
{
	switch(type)
	{
		case ShaderType_Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType_Fragment:
			return GL_FRAGMENT_SHADER;
		case ShaderType_Geometry:
			return GL_GEOMETRY_SHADER;
#ifdef GL_TESS_CONTROL_SHADER
		case ShaderType_TesselationControl:
			return GL_TESS_CONTROL_SHADER;
		case ShaderType_TesselationEvaluation:
			return GL_TESS_EVALUATION_SHADER;
#endif
#ifdef GL_COMPUTE_SHADER
		case ShaderType_Compute:
			return GL_COMPUTE_SHADER;
#endif
	}
	FatalError("%s shader type (%u) not available", AsString(type), type);
	return 0;
}


/// Debug ///

const char* AsString( DebugEventSource source )
{
	switch(source)
	{
		case DebugEventSource_API: return "API";
		case DebugEventSource_WindowSystem: return "window system";
		case DebugEventSource_ShaderCompiler: return "shader compiler";
		case DebugEventSource_ThirdParty: return "third party";
		case DebugEventSource_Application: return "application";
		case DebugEventSource_Other: return "other";
	}
	return "unknown";
}

GLenum ConvertToGL( DebugEventSource source )
{
	switch(source)
	{
#ifdef GL_ARB_debug_output
		case DebugEventSource_API: return GL_DEBUG_SOURCE_API_ARB;
		case DebugEventSource_WindowSystem: return GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB;
		case DebugEventSource_ShaderCompiler: return GL_DEBUG_SOURCE_SHADER_COMPILER_ARB;
		case DebugEventSource_ThirdParty: return GL_DEBUG_SOURCE_THIRD_PARTY_ARB;
		case DebugEventSource_Application: return GL_DEBUG_SOURCE_APPLICATION_ARB;
		case DebugEventSource_Other: return GL_DEBUG_SOURCE_OTHER_ARB;
#endif
	}
	FatalError("Invalid debug event source: %u", source);
	return 0;
}

DebugEventSource DebugEventSourceFromGL( GLenum type )
{
	switch(type)
	{
#ifdef GL_ARB_debug_output
		case GL_DEBUG_SOURCE_API_ARB: return DebugEventSource_API;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: return DebugEventSource_WindowSystem;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: return DebugEventSource_ShaderCompiler;
		case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: return DebugEventSource_ThirdParty;
		case GL_DEBUG_SOURCE_APPLICATION_ARB: return DebugEventSource_Application;
		case GL_DEBUG_SOURCE_OTHER_ARB: return DebugEventSource_Other;
#endif
	}
	FatalError("Invalid debug event gl type: %u", type);
	return DebugEventSource_Other;
}



const char* AsString( DebugEventType type )
{
	switch(type)
	{
		case DebugEventType_Error: return "error";
		case DebugEventType_DeprecatedBehavior: return "deprecated behavior";
		case DebugEventType_UndefinedBahavior: return "undefined behavior";
		case DebugEventType_Portability: return "portability";
		case DebugEventType_Performance: return "performance";
		case DebugEventType_Other: return "other";
	}
	return "unknown";
}

GLenum ConvertToGL( DebugEventType type )
{
	switch(type)
	{
#ifdef GL_ARB_debug_output
		case DebugEventType_Error: return GL_DEBUG_TYPE_ERROR_ARB;
		case DebugEventType_DeprecatedBehavior: return GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB;
		case DebugEventType_UndefinedBahavior: return GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB;
		case DebugEventType_Portability: return GL_DEBUG_TYPE_PORTABILITY_ARB;
		case DebugEventType_Performance: return GL_DEBUG_TYPE_PERFORMANCE_ARB;
		case DebugEventType_Other: return GL_DEBUG_TYPE_OTHER_ARB;
#endif
	}
	FatalError("Invalid debug event type: %u", type);
	return 0;
}

DebugEventType DebugEventTypeFromGL( GLenum type )
{
	switch(type)
	{
#ifdef GL_ARB_debug_output
		case GL_DEBUG_TYPE_ERROR_ARB: return DebugEventType_Error;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: return DebugEventType_DeprecatedBehavior;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: return DebugEventType_UndefinedBahavior;
		case GL_DEBUG_TYPE_PORTABILITY_ARB: return DebugEventType_Portability;
		case GL_DEBUG_TYPE_PERFORMANCE_ARB: return DebugEventType_Performance;
		case GL_DEBUG_TYPE_OTHER_ARB: return DebugEventType_Other;
#endif
	}
	FatalError("Invalid debug event gl type: %u", type);
	return DebugEventType(0);
}





const char* AsString( DebugEventSeverity severity )
{
	switch(severity)
	{
		case DebugEventSeverity_High: return "high";
		case DebugEventSeverity_Medium: return "medium";
		case DebugEventSeverity_Low: return "low";
	}
	return "unknown";
}

GLenum ConvertToGL( DebugEventSeverity severity )
{
	switch(severity)
	{
#ifdef GL_ARB_debug_output
		case DebugEventSeverity_High: return GL_DEBUG_SEVERITY_HIGH_ARB;
		case DebugEventSeverity_Medium: return GL_DEBUG_SEVERITY_MEDIUM_ARB;
		case DebugEventSeverity_Low: return GL_DEBUG_SEVERITY_LOW_ARB;
#endif
	}
	FatalError("Invalid debug event severity: %u", severity);
	return 0;
}

DebugEventSeverity DebugEventSeverityFromGL( GLenum severity )
{
	switch(severity)
	{
#ifdef GL_ARB_debug_output
		case GL_DEBUG_SEVERITY_HIGH_ARB: return DebugEventSeverity_High;
		case GL_DEBUG_SEVERITY_MEDIUM_ARB: return DebugEventSeverity_Medium;
		case GL_DEBUG_SEVERITY_LOW_ARB: return DebugEventSeverity_Low;
#endif
	}
	FatalError("Invalid debug event gl severity: %u", severity);
	return DebugEventSeverity(0);
}

}
}
