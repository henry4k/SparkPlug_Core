#ifndef __SPARKPLUG_FILE__
#define __SPARKPLUG_FILE__

#include <istream>
#include <ostream>

namespace SparkPlug
{

namespace FileMode
{	
enum Enum
{
	Binary = (1 << 0), // Open file in binary mode (default)
	Text   = (1 << 1), // Open file in text mode

	Overwrite = (1 << 2), // Overwrite existing data (default)
	Append    = (1 << 3), // Append to end of file
	Truncuate = (1 << 4)  // Clear file on opening
};
}

std::istream* ReadFile( const char* path, int flags );
std::ostream* WriteFile( const char* path, int flags );

}

#endif
