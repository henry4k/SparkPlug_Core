#ifndef __SPARKPLUG_RENDER_CONTEXT__
#define __SPARKPLUG_RENDER_CONTEXT__

#include <vector>
#include <SparkPlug/Reference.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Texture.h>


namespace SparkPlug
{
namespace GL
{

class Context;

class Limits
{
	public:
		int maxTextureUnits;
		
		
	private:
		friend class Context;
		Limits( Context* context );
		
		Context* m_Context;
};

class Context
{
	public:
		Context();
		virtual ~Context();
		
		const Limits& limits() const;
		
		void bindTexture( const StrongRef<Texture>& texture, int unit );
		void unbindTexture( int unit );
		const StrongRef<Texture>& boundTexture( int unit ) const;
		
// 		void bindShader( const StrongRef<ShaderProgram>& shader );
// 		void unbindShader();
// 		const StrongRef<ShaderProgram>& boundShader() const;
		
	private:
		Limits m_Limits;
		
		
		int m_ActiveTextureUnit;
		void selectTextureUnit( int unit );
		
		/**
		 * Length is limits().maxTextureUnits
		 */
		StrongRef<Texture>* m_TextureUnits;
		
// 		StrongRef<ShaderProgram> m_ShaderProgram;
};

}
}

#endif
