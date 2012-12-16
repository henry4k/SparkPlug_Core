#ifdef __WIN32__
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#ifndef NOGDI
		#define NOGDI
	#endif
	
	#include <windows.h>
#else
	#include <csignal>
#endif

#include <cstdio>
#include <cstdlib>
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

	//std::terminate();
	std::abort();
}

std::string Format( const char* format, ... )
{
	std::va_list vl;
	va_start(vl, format);
		std::string r = FormatV(format, vl);
	va_end(vl);
	return r;
}

std::string FormatV( const char* format, std::va_list& vl )
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

void Break()
{
	#ifdef __WIN32__
		DebugBreak();
	#else
		raise(SIGTRAP);
	#endif
}

}
