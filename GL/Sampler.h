#ifndef __SPARKPLUG_SAMPLER__
#define __SPARKPLUG_SAMPLER__

#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Texture.h>

namespace SparkPlug
{
namespace GL
{

class Context;

class Sampler : public SamplerBase
{
public:
	Sampler( Context* context );
	~Sampler();
	
	void bind( int textureUnit ) const;
	static void Unbind( int textureUnit );
	
	void setFilter( TextureFilter filter );
	void setAddressMode( TextureAddressMode mode );
	void setMaxAnisotropic( float level );
};

}
}

#endif
