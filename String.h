#ifndef __SPARKPLUG_STRING__
#define __SPARKPLUG_STRING__

#include <vector>
#include "Char.h"

namespace SparkPlug
{

class String
{
public:


	String();

	// String( const Char* str );
	// String( const void* str, DecodeFn decoder );
	
	int length() const;

private:
	std::vector<Char> m_Data;
};

}

#endif
