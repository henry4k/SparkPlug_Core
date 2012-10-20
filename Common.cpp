#include <cstdio>
#include <exception>
#include <SparkPlug/Common.h>


namespace SparkPlug
{

void FatalError( const char* format, ... )
{
	std::va_list vl;
	va_start(vl, format);
		LogError("%s", FormatV(format, vl).c_str());
	va_end(vl);
	std::terminate();
}

String Format( const char* format, ... )
{
	std::va_list vl;
	va_start(vl, format);
		String r = FormatV(format, vl);
	va_end(vl);
	return r;
}

String FormatV( const char* format, std::va_list& vl )
{
	char* p = ScratchPad(1024);
	int count = std::vsprintf(p, format, vl);
	if(count < 0)
		return "";
	return p;
}

char* ScratchPad( int size )
{
	static char staticBuffer[2048];
	assert(size <= sizeof(staticBuffer));
	return staticBuffer;
}



}
