#ifndef __SPARKPLUG_RENDER_CONTEXT__
#define __SPARKPLUG_RENDER_CONTEXT__

#include <vector>
#include <stack>
#include <SparkPlug/Reference.h>
#include <SparkPlug/GL/OpenGL.h>
#include <SparkPlug/GL/Texture.h>
#include <SparkPlug/GL/Sampler.h>
#include <SparkPlug/GL/Shader.h>


namespace SparkPlug
{
namespace GL
{

class Context;

class Limits
{
	public:
		void print() const;
		
		int maxColorAttachments;
		int maxDrawBuffers;
		
		int maxVertexTextureUnits;
		int maxFragmentTextureUnits;
		int maxCombinedTextureUnits;
		
		int maxTextureSize[TextureType_Count];
		
		int maxTextureCoords;
		int maxVertexAttributes;
		
		float maxTextureAnisotropy;
		
	private:
		friend class Context;
		Limits( Context* context );
		
		Context* m_Context;
};

class TextureBinding
{
	public:
		TextureBinding( Context* context, int unit, const StrongRef<Texture>& texture );
		virtual ~TextureBinding();
	
	private:
		Context*           m_Context;
		int                m_Unit;
		StrongRef<Texture> m_Previous;
};

class SamplerBinding
{
	public:
		SamplerBinding( Context* context, int unit, const StrongRef<Sampler>& texture );
		virtual ~SamplerBinding();
	
	private:
		Context*           m_Context;
		int                m_Unit;
		StrongRef<Sampler> m_Previous;
};

class ProgramBinding
{
	public:
		ProgramBinding( Context* context, const StrongRef<Program>& program );
		virtual ~ProgramBinding();

	private:
		Context*           m_Context;
		StrongRef<Program> m_Previous;
};

class Context
{
	public:
		virtual ~Context();
		
		const Limits& limits() const;
		
		int activeTextureUnit() const;
		
		void bindTexture( int unit, const StrongRef<Texture>& texture );
		const StrongRef<Texture>& boundTexture( int unit ) const;
		
		void bindSampler( int unit, const StrongRef<Sampler>& sampler );
		const StrongRef<Sampler>& boundSampler( int unit ) const;
		
 		void bindProgram( const StrongRef<Program>& program );
 		const StrongRef<Program>& boundProgram() const;
	
	protected:
		Context();
		void postInit();
		
	private:
		Limits* m_Limits;

		int m_ActiveTextureUnit;
		void selectTextureUnit( int unit );
		StrongRef<Texture>* m_Textures; // Length is limits().maxTextureUnits
		StrongRef<Sampler>* m_Samplers; // Length is limits().maxTextureUnits

 		StrongRef<Program> m_Program;
};

}
}

#endif
