#include <fstream>
#include <SparkPlug/File.h>

namespace SparkPlug
{

std::ios_base::openmode CreateOpenMode( int flags )
{
	using namespace std;
	ios_base::openmode r = static_cast<ios_base::openmode>(0);

	if(flags & FileMode::Text)
		; // text
	else
		r |= ios_base::binary; // default
	
	if(flags & FileMode::Truncuate)
		r |= ios_base::trunc;
	else if(flags & FileMode::Append)
		r |= ios_base::app;
	//else
	//	; // TODO: overwrite

	return r;
}

std::istream* ReadFile( const char* path, int flags )
{
	std::ifstream* f = new std::ifstream();
	f->open(path, CreateOpenMode(flags));
	return f;
}

std::ostream* WriteFile( const char* path, int flags )
{
	std::ofstream* f = new std::ofstream();
	f->open(path, CreateOpenMode(flags));
	return f;
}

}
