#include <SparkPlug/GL/Sampler.h>

namespace SparkPlug
{
namespace GL
{

Sampler::Sampler( Context* context ) :
	SamplerBase(context)
{
	glGenSamplers(1, &m_Handle);
}

Sampler::~Sampler()
{
	glDeleteSamplers(1, &m_Handle);
}

void Sampler::bind( int textureUnit ) const
{
	glBindSampler(textureUnit, m_Handle);
}

void Sampler::Unbind( int textureUnit )
{
	glBindSampler(textureUnit, 0);
}

void Sampler::setFilter( TextureFilter filter )
{
// 	bind();
	glSamplerParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, ConvertToGL(filter, false));
	glSamplerParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, ConvertToGL(filter, false));
	SamplerBase::setFilter(filter);
}

void Sampler::setAddressMode( TextureAddressMode mode )
{
// 	bind();
	glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_S, ConvertToGL(mode));
	glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_T, ConvertToGL(mode));
	glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_R, ConvertToGL(mode));
	SamplerBase::setAddressMode(mode);
}

void Sampler::setMaxAnisotropic( float level )
{
// 	bind();
	glSamplerParameterf(m_Handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
	SamplerBase::setMaxAnisotropic(level);
}

}
}
