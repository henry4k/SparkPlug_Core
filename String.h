#ifndef __SPARKPLUG_STRING__
#define __SPARKPLUG_STRING__

#include <vector>
#include <SparkPlug/Unicode.h>

namespace SparkPlug
{

typedef Utf16 Char;

/**
 * String objects are immutable.
 * Therefore all member functions must be constant.
 */
class String
{
public:
	String();
	String( const String& str );
	String( const char* str, int maxUnits = -1 );
	String( const Utf8* str, int maxUnits = -1 );
	String( const Utf16* str, int maxUnits = -1 );
	String( const Utf32* str, int maxUnits = -1 );
	
	/**
	 * Returns the unit count, not character count.
	 */
	int size() const;

	// int length() const; <- Could confuse people since it suggets that it returns the amount of *characters* ...

private:
	std::vector<Char> m_Chars;
};

}

#endif
