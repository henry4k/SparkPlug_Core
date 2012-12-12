#include <SparkPlug/GL/Buffer.h>

namespace SparkPlug
{
namespace GL
{

/// ----- PrimitiveType ------

const char* AsString( PrimitiveType type )
{
	switch(type)
	{
		case PrimitiveType_PointList:
			return "PointList";
		case PrimitiveType_LineList:
			return "LineList";
		case PrimitiveType_LineStrip:
			return "LineStrip";
		case PrimitiveType_LineLoop:
			return "LineLoop";
		case PrimitiveType_TriangleList:
			return "TriangleList";
		case PrimitiveType_TriangleStrip:
			return "TriangleStrip";
		case PrimitiveType_TriangleFan:
			return "TriangleFan";
	}
	return "UnknownPrimitveType";
}

GLenum ConvertToGL( PrimitiveType type )
{
	switch(type)
	{
		case PrimitiveType_PointList:
			return GL_POINTS;
		case PrimitiveType_LineList:
			return GL_LINES;
		case PrimitiveType_LineStrip:
			return GL_LINE_STRIP;
		case PrimitiveType_LineLoop:
			return GL_LINE_LOOP;
		case PrimitiveType_TriangleList:
			return GL_TRIANGLES;
		case PrimitiveType_TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case PrimitiveType_TriangleFan:
			return GL_TRIANGLE_FAN;
	}
	FatalError("Invalid primitive type: %u", type);
	return 0;
}


/// ----- BufferLockMode ------

const char* AsString( BufferLockMode type )
{
	switch(type)
	{
		case BufferLockMode_ReadOnly:
			return "ReadOnly";
		case BufferLockMode_WriteOnly:
			return "WriteOnly";
		case BufferLockMode_ReadWrite:
			return "ReadWrite";
	}
	return "UnknownBufferLockMode";
}

GLenum ConvertToGL( BufferLockMode type )
{
	switch(type)
	{
		case BufferLockMode_ReadOnly:
			return GL_READ_ONLY_ARB;
		case BufferLockMode_WriteOnly:
			return GL_WRITE_ONLY_ARB;
		case BufferLockMode_ReadWrite:
			return GL_READ_WRITE_ARB;
	}
	FatalError("Invalid buffer lock mode: %u", type);
	return 0;
}


/// ----- BufferUsage ------

const char* AsString( BufferUsage type )
{
	switch(type)
	{
		case BufferUsage_Static:
			return "Static";
		case BufferUsage_Stream:
			return "Stream";
		case BufferUsage_Dynamic:
			return "Dynamic";
	}
	return "UnknownBufferUsage";
}

GLenum ConvertToGL( BufferUsage type )
{
	switch(type)
	{
		case BufferUsage_Static:
			return GL_STATIC_DRAW_ARB;
		case BufferUsage_Stream:
			return GL_STREAM_DRAW_ARB;
		case BufferUsage_Dynamic:
			return GL_DYNAMIC_DRAW_ARB;
	}
	FatalError("Invalid buffer usage: %u", type);
	return 0;
}



/// ----- BufferUsage ------

const char* AsString( BufferTarget type )
{
	switch(type)
	{
		case BufferTarget_Vertex:
			return "Vertex";
		case BufferTarget_Index:
			return "Index";
		case BufferTarget_PixelPacker:
			return "PixelPacker";
		case BufferTarget_PixelUnpacker:
			return "PixelUnpacker";
	}
	return "UnknownBufferTarget";
}

GLenum ConvertToGL( BufferTarget type )
{
	switch(type)
	{
		case BufferTarget_Vertex:
			return GL_ARRAY_BUFFER_ARB;
		case BufferTarget_Index:
			return GL_ELEMENT_ARRAY_BUFFER_ARB;
		case BufferTarget_PixelPacker:
			return GL_PIXEL_PACK_BUFFER_ARB;
		case BufferTarget_PixelUnpacker:
			return GL_PIXEL_UNPACK_BUFFER_ARB;
	}
	FatalError("Invalid buffer target: %u", type);
	return 0;
}



/// ----- IndexType ------

const char* AsString( IndexType type )
{
	switch(type)
	{
		case IndexType_UInt8:
			return "UInt8";
		case IndexType_UInt16:
			return "UInt16";
		case IndexType_UInt32:
			return "UInt32";
	}
	return "UnknownIndexType";
}

GLenum ConvertToGL( IndexType type )
{
	switch(type)
	{
		case IndexType_UInt8:
			return GL_UNSIGNED_BYTE;
		case IndexType_UInt16:
			return GL_UNSIGNED_SHORT;
		case IndexType_UInt32:
			return GL_UNSIGNED_INT;
	}
	FatalError("Invalid index type: %u", type);
	return 0;
}

int SizeOf( IndexType type )
{
	switch(type)
	{
		case IndexType_UInt8:
			return sizeof(GLubyte);
		case IndexType_UInt16:
			return sizeof(GLushort);
		case IndexType_UInt32:
			return sizeof(GLuint);
	}
	FatalError("Invalid index type: %u", type);
	return 0;
}


/// ---- Buffer ----

Buffer::Buffer( Context* context, BufferTarget target, BufferUsage usage, int count, int elementSize ) :
	Object(context),
	m_Target(target),
	m_Usage(usage),
	m_Count(count),
	m_ElementSize(elementSize),
	m_Locked(false)
{
	glGenBuffersARB(1, &m_Handle);
	
	// bind()
	glBufferDataARB(target, size(), NULL, ConvertToGL(m_Usage));
}

Buffer::~Buffer()
{
	glDeleteBuffersARB(1, &m_Handle);
}

/*
void Buffer::bind()
{
	glBindBuffersARB(target(), m_Handle);
}
*/

bool Buffer::isReady() const
{
	return m_Handle != 0;
}

BufferTarget Buffer::target() const
{
	return m_Target;
}

void* Buffer::lock( BufferLockMode type )
{
	assert(m_Locked == true);
	// bind()
	void* p = glMapBuffer(target(), ConvertToGL(type));
	assert(p);

	m_Locked = true;
	return p;
}

void Buffer::unlock()
{
	assert(m_Locked == false);
	
	// bind()
	if(!glUnmapBuffer(target()))
		LogWarning("glUnmapBuffer failed ...");
	
	m_Locked = false;
}

void Buffer::copyFrom( const void* source, int count, int start )
{
	assert(m_Locked);
	assert(start+count <= m_Count);
	// bind()
	glBufferSubDataARB(target(), start*elementSize(), count*elementSize(), source);
}

void Buffer::copyTo( void* destination, int count, int start )
{
	assert(m_Locked);
	assert(start+count <= m_Count);
	// bind()
	glGetBufferSubDataARB(target(), start*elementSize(), count*elementSize(), destination);
}

int Buffer::elementSize() const
{
	return m_ElementSize;
}

int Buffer::elementCount() const
{
	return m_Count;
}

int Buffer::size() const
{
	return elementCount()*elementSize();
}


/// ---- VertexBuffer ----

StrongRef<VertexBuffer> VertexBuffer::Create( Context* context, const VertexFormat& format, int count, BufferUsage usage )
{
	return new VertexBuffer(context, format, count, usage);
}

VertexBuffer::VertexBuffer( Context* context, const VertexFormat& format, int count, BufferUsage usage ) :
	Buffer(context, BufferTarget_Vertex, usage, count, format.sizeInBytes()),
	m_Format(format)
{
}

const VertexFormat& VertexBuffer::format() const
{
	return m_Format;
}


/// ---- IndexBuffer ----

StrongRef<IndexBuffer> IndexBuffer::Create( Context* context, int count, BufferUsage usage )
{
	return new IndexBuffer(context, count, usage, IndexType_UInt16);
}

IndexType IndexBuffer::indexType() const
{
	return m_IndexType;
}

IndexBuffer::IndexBuffer( Context* context, int count, BufferUsage usage, IndexType indexType ) :
	Buffer(context, BufferTarget_Index, usage, count, SizeOf(indexType)),
	m_IndexType(indexType)
{
}


/// ---- PixelBuffer ----

/*
StrongRef<PixelBuffer> PixelBuffer::Create( Context* context, const PixelFormat& format, vec3i size )
{
	return new PixelBuffer(context, format, size);
}

StrongRef<PixelFormat> PixelBuffer::CreateFromImage( Context* context, const Image& image )
{
	StrongRef<PixelBuffer> buffer = new PixelBuffer(context, image.format(), image.size());
	buffer->copyFrom(image.pixels(), image.size().volume());
	return buffer;
}
*/

// TODO: Hier BufferUsage auf PixelPack/Unpack beschränken!
// Entweder durch Laufzeit-Prüfung mit assert() o.Ä. oder mit einem eigenem Enum (ein bool ist blöd)
PixelBuffer::PixelBuffer( Context* context, BufferTarget target, BufferUsage usage, const PixelFormat& format, vec3i size ) :
	Buffer(context, target, usage, size.volume(), format.pixelSize()),
	m_Format(format),
	m_Size(size)
{
	switch(target)
	{
		case BufferTarget_PixelPacker:
		case BufferTarget_PixelUnpacker:
			break;
		default:
			FatalError("PixelBuffer doesn't accept this buffer target: %s (%u)", AsString(target), target);
	}
}

const PixelFormat& PixelBuffer::format() const
{
	return m_Format;
}

vec3i PixelBuffer::size() const
{
	return m_Size;
}


}
}
