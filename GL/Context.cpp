#include <SparkPlug/Common.h>
#include <SparkPlug/GL/Context.h>


namespace SparkPlug
{
namespace GL
{

/// --- Limits ---
Limits::Limits( Context* context ) :
	m_Context(context)
{
	maxTextureUnits = 4;
}


/// --- Context ---
Context::Context() :
	m_Limits(this)
{
	m_ActiveTextureUnit = 0;
	m_TextureUnits = new StrongRef<Texture>[limits().maxTextureUnits];
}

Context::~Context()
{
	delete[] m_TextureUnits;
}

const Limits& Context::limits() const
{
	return m_Limits;
}

void Context::selectTextureUnit( int unit )
{
	if(m_ActiveTextureUnit == unit)
		return;

	glActiveTextureARB(GL_TEXTURE0_ARB+unit);
	m_ActiveTextureUnit = unit;
}

void Context::bindTexture( const StrongRef<Texture>& texture, int unit )
{
	assert(InsideArray(unit, limits().maxTextureUnits));
	
	if(m_TextureUnits[unit] == texture)
		return;
	
	selectTextureUnit(unit);
	
	if(!m_TextureUnits[unit] || (m_TextureUnits[unit]->type() != texture->type()))
	{
		if(m_TextureUnits[unit])
			glDisable(ConvertToGL(m_TextureUnits[unit]->type()));
		glEnable(ConvertToGL(texture->type()));
	}
	
	glBindTexture(ConvertToGL(texture->type()), texture->handle());
	m_TextureUnits[unit] = texture;
}

void Context::unbindTexture( int unit )
{
	assert(InsideArray(unit, limits().maxTextureUnits));
	
	glBindTexture(ConvertToGL(m_TextureUnits[unit]->type()), 0);
	m_TextureUnits[unit] = StrongRef<Texture>();
}

const StrongRef<Texture>& Context::boundTexture( int unit ) const
{
	assert(InsideArray(unit, limits().maxTextureUnits));
	return m_TextureUnits[unit];
}

}
}
