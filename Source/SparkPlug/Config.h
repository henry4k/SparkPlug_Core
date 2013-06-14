#ifndef __SPARKPLUG_CONFIG__
#define __SPARKPLUG_CONFIG__

#include <map>
#include <SparkPlug/Common.h>

namespace SparkPlug
{

class Config
{
public:
	Config();
	
	void set( const char* key, const char* value );
	const char* get( const char* key ) const;
	
	// utillity functions
	void set( const char* key, int value );
	void set( const char* key, double value );
	
	// "foo/bar" <- Tables werden durch eine Pfadangabe gekennzeichnet
	// "foo/42/bar/0" <- Arrays sind Tables welche ledeglich Zahlen enthalten
	
protected:
	std::map<std::string, std::string> m_Entries;
};

}

#endif