#ifndef __SPARKPLUG_GL_ENUMS__
#define __SPARKPLUG_GL_ENUMS__

#include <SparkPlug/GL/OpenGL.h>

namespace SparkPlug
{
namespace GL
{
/// Texture ///
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


/// Shader ///

enum ShaderType
{
	ShaderType_Vertex,
	ShaderType_Fragment,
	ShaderType_Geometry,
	ShaderType_TesselationControl,
	ShaderType_TesselationEvaluation,
	ShaderType_Compute
};
const char* AsString( ShaderType type );
GLenum      ConvertToGL( ShaderType type );


/// Debug ///
enum DebugEventSource
{
	DebugEventSource_API,
	DebugEventSource_WindowSystem,
	DebugEventSource_ShaderCompiler,
	DebugEventSource_ThirdParty,
	DebugEventSource_Application,
	DebugEventSource_Other
};
const char* AsString( DebugEventSource source );
GLenum ConvertToGL( DebugEventSource source );
DebugEventSource DebugEventSourceFromGL( GLenum type );


enum DebugEventType
{
	DebugEventType_Error,
	DebugEventType_DeprecatedBehavior,
	DebugEventType_UndefinedBahavior,
	DebugEventType_Portability,
	DebugEventType_Performance,
	DebugEventType_Other
};
const char* AsString( DebugEventType type );
GLenum ConvertToGL( DebugEventType type );
DebugEventType DebugEventTypeFromGL( GLenum type );


enum DebugEventSeverity
{
	DebugEventSeverity_High,
	DebugEventSeverity_Medium,
	DebugEventSeverity_Low
};
const char* AsString( DebugEventSeverity severity );
GLenum ConvertToGL( DebugEventSeverity severity );
DebugEventSeverity DebugEventSeverityFromGL( GLenum severity );

}
}

#endif
