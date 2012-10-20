#ifndef __SPARKPLUG_COMMON__
#define __SPARKPLUG_COMMON__

#include <cstdarg>
#include <cassert>
#include <string>
#include <SparkPlug/Log.h>

namespace SparkPlug
{

typedef std::string String;
typedef unsigned int uint;

void FatalError( const char* format, ... );

String Format( const char* format, ... );
String FormatV( const char* format, std::va_list& vl );

char* ScratchPad( int size );

}

#endif
