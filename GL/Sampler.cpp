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

void Sampler::setFilter( TextureFilter f )
{
	if(f == filter())
		return;
	glSamplerParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, ConvertToGL(f, true));
	glSamplerParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, ConvertToGL(f, false));
	SamplerBase::setFilter(f);
}

void Sampler::setAddressMode( TextureAddressMode m )
{
	if(m == addressMode())
		return;
	glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_S, ConvertToGL(m));
	glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_T, ConvertToGL(m));
	glSamplerParameteri(m_Handle, GL_TEXTURE_WRAP_R, ConvertToGL(m));
	SamplerBase::setAddressMode(m);
}

void Sampler::setMaxAnisotropic( float level )
{
	if(level == maxAnisotropic())
		return;
	glSamplerParameterf(m_Handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
	SamplerBase::setMaxAnisotropic(level);
}

}
}
