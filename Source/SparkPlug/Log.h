#ifndef __SPARKPLUG_LOG__
#define __SPARKPLUG_LOG__

namespace SparkPlug
{

enum LogLevel
{
	LogLevel_Info,
	LogLevel_Warning,
	LogLevel_Error
};

class LogHandler
{
public:
	LogHandler();
	virtual ~LogHandler();
	
	virtual void handleLogMessage( LogLevel level, const char* message ) = 0;
};

void LogRaw( LogLevel level, const char* message );

void Log( const char* format, ... );
void LogWarning( const char* format, ... );
void LogError( const char* format, ... );

}

#endif
