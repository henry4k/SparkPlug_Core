#ifndef __SPARKPLUG_STRING__
#define __SPARKPLUG_STRING__

#include <string>

namespace SparkPlug
{
	typedef char Char;
	
	class String : public std::basic_string<Char>
	{
		String();
		String( const Char* cstring );
		
		// ...
	};
	
	class StringBuilder // aka Rope
	{
		StringBuilder();
		StringBuilder( const String& init );
		// ...
		
		StringBuilder& append( const String& v );
		// ...
		
		String getString();
	};
	
	class StringFormatter
	{
		StringFormatter();
		StringFormatter( const String& init );
		// ...
		
		StringFormatter& setFormatString( const String& init );
		
		StringFormatter& add( const String& key, const String& value );
		// ...
		
		String getString();
	}

	// Formatieren:
	// - Genauigkeit von float begrenzen/erzwingen
	// - Exponentialschreibweise von float verwenden/verbieten
	//
	// - Int Hexadezimal/Oktal/Binär darstellen
	//
	// - Bool mit 1/0 oder true/false darstellen
	//
	// - String mit (Leer)zeichen von links/rechts auffüllen
	// - String auf bestimmte länge begrenzen und dann vorne/hinten abschneiden
}

#endif
