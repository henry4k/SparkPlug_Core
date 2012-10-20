#include <SparkPlug/Logic.h>
#include <SparkPlug/Pixel.h>


namespace SparkPlug
{

const char* AsString( PixelSemantic semantic )
{
	switch(semantic)
	{
		case PixelSemantic_Luminance:
			return "Luminance";
		
		case PixelSemantic_LuminanceAlpha:
			return "LuminanceAlpha";
		
		case PixelSemantic_RGB:
			return "RGB";
		
		case PixelSemantic_RGBA:
			return "RGBA";
		
		case PixelSemantic_Depth:
			return "Depth";
		
		default:
			return "UnknownPixelSemantic";
	}
}

const char* AsString( PixelComponent component )
{
	switch(component)
	{
		case PixelComponent_UInt8:
			return "UInt8";
		
		case PixelComponent_UInt16:
			return "UInt16";
		
		case PixelComponent_UInt24:
			return "UInt24";
		
		case PixelComponent_UInt32:
			return "UInt32";
		
		case PixelComponent_Float16:
			return "Float16";
		
		case PixelComponent_Float32:
			return "Float32";
		
		default:
			return "UnknownPixelComponent";
	}
}

int ComponentCount( PixelSemantic semantic )
{
	switch(semantic)
	{
		case PixelSemantic_Depth:
		case PixelSemantic_Luminance:
			return 1;
		
		case PixelSemantic_LuminanceAlpha:
			return 2;
		
		case PixelSemantic_RGB:
			return 3;
		
		case PixelSemantic_RGBA:
			return 4;
		
		default:
			FatalError("Invalid pixel semantic: %u", semantic);
			return 0;
	}
}

int Size( PixelComponent component )
{
	switch(component)
	{
		case PixelComponent_UInt8:
			return 1;
		
		case PixelComponent_UInt16:
			return 2;
		
		case PixelComponent_UInt24:
			return 3;
		
		case PixelComponent_UInt32:
			return 4;
		
		case PixelComponent_Float16:
			return 2;
		
		case PixelComponent_Float32:
			return 4;
		
		default:
			FatalError("Invalid pixel component: %u", component);
			return 0;
	}
}

PixelFormat::PixelFormat( PixelSemantic semantic, PixelComponent component ) :
	m_Semantic(semantic),
	m_Component(component)
{
}

PixelFormat::PixelFormat( const PixelFormat& source ) :
	m_Semantic(source.m_Semantic),
	m_Component(source.m_Component)
{
}

PixelFormat& PixelFormat::operator = ( const PixelFormat& source )
{
	m_Semantic = source.m_Semantic;
	m_Component = source.m_Component;
	return *this;
}

bool PixelFormat::isValid() const
{
	return
		Inside<int>(PixelSemantic_Luminance, m_Semantic, PixelSemantic_RGBA) &&
		Inside<int>(PixelComponent_UInt8, m_Component, PixelComponent_Float32);
}

bool PixelFormat::operator == ( const PixelFormat& format ) const
{
	return
		(m_Semantic == format.m_Semantic) &&
		(m_Component == format.m_Component);
}

bool PixelFormat::operator != ( const PixelFormat& format ) const
{
	return !(*this == format);
}

PixelSemantic PixelFormat::semantic() const
{
	return m_Semantic;
}

PixelComponent PixelFormat::componentType() const
{
	return m_Component;
}

int PixelFormat::pixelSize() const
{
	return componentSize()*componentCount();
}

int PixelFormat::componentSize() const
{
	return Size(m_Component);
}

int PixelFormat::componentCount() const
{
	return ComponentCount(m_Semantic);
}

String PixelFormat::asString() const
{
	return Format("%s %s", AsString(m_Semantic), AsString(m_Component));
}

}
