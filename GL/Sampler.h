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
	
	void setFilter( TextureFilter f );
	void setAddressMode( TextureAddressMode m );
	void setMaxAnisotropic( float level );
};

}
}

#endif
