#ifndef __SPARKPLUG_SAMPLER__
#define __SPARKPLUG_SAMPLER__

#include <SparkPlug/gl/OpenGL.h>
#include <SparkPlug/gl/Texture.h>

namespace SparkPlug
{

class Sampler : public SamplerBase
{
public:
	Sampler();
	~Sampler();
	
	void bind( int textureUnit ) const;
	static void Unbind( int textureUnit );
	
	void setFilter( TextureFilter filter );
	void setAddressMode( TextureAddressMode mode );
	void setMaxAnisotropic( float level );
	
private:
	GLuint m_Handle;
};

}

#endif
