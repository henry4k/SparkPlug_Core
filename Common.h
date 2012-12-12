#ifndef __SPARKPLUG_COMMON__
#define __SPARKPLUG_COMMON__

#include <cstdarg>
#include <cassert>
#include <string>
#include <SparkPlug/Log.h>
#include <SparkPlug/Logic.h>

namespace SparkPlug
{

typedef unsigned int uint;

void FatalError( const char* format, ... );

std::string Format( const char* format, ... );
std::string FormatV( const char* format, std::va_list& vl );

char* ScratchPad( int size );

}

#endif
