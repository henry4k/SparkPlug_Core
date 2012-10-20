#include <string.h>
#include <algorithm>
#include <vector>
#include <SparkPlug/Common.h>
#include <SparkPlug/Log.h>

namespace SparkPlug
{

std::vector<LogHandler*>& LogHandlers()
{
	static std::vector<LogHandler*> r;
	return r;
}

LogHandler::LogHandler()
{
	LogHandlers().push_back(this);
}

LogHandler::~LogHandler()
{
	std::vector<LogHandler*>::iterator i = std::find(LogHandlers().begin(), LogHandlers().end(), this);
	assert(i == LogHandlers().end());
	LogHandlers().erase(i);
}


void LogRawLine( LogLevel level, const char* line )
{
	if(LogHandlers().empty())
	{
		switch(level)
		{
			case LogLevel_Error:
				std::cerr << "Error: " << line << std::endl;
				break;
			case LogLevel_Warning:
				std::cerr << "Warning: " << line << std::endl;
				break;
			default:
				std::cerr << line << std::endl;
				break;
		}
	}
	else
	{
		std::vector<LogHandler*>::iterator i = LogHandlers().begin();
		for(; i != LogHandlers().end(); ++i)
		{
			(*i)->handleLogMessage(level, line);
		}
	}
}

void LogRaw( LogLevel level, const char* message )
{
	assert(message);
	
	int last = 0;
	int i = 0;
	for(; message[i] != '\0'; ++i)
	{
		if(message[i] != '\n')
			continue;
		
		int length = i-last;
		char* line = ScratchPad(length+1);
		memcpy(line, &message[last], length);
		line[length] = '\0';
		LogRawLine(level, line);
		
		last = i+1;
	}
	
	if(i-last > 0)
		LogRawLine(level, &message[last]);

}


/*
std::ostream& Log()
{
	return NULL;
}

std::ostream& LogWarning()
{
	return NULL;
}

std::ostream& LogError()
{
	return NULL;
}
*/

void Log( const char* format, ... )
{
	va_list vl;
	va_start(vl, format);
		LogRaw(LogLevel_Info, FormatV(format, vl).c_str());
	va_end(vl);
}

void LogWarning( const char* format, ... )
{
	va_list vl;
	va_start(vl, format);
		LogRaw(LogLevel_Warning, FormatV(format, vl).c_str());
	va_end(vl);
}

void LogError( const char* format, ... )
{
	va_list vl;
	va_start(vl, format);
		LogRaw(LogLevel_Error, FormatV(format, vl).c_str());
	va_end(vl);
}



}
