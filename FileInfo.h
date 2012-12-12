#ifndef __SPARKPLUG_FILEINFO__
#define __SPARKPLUG_FILEINFO__

#include <ctime>
#include <sys/stat.h>

#include <SparkPlug/Platform.h>
#include <SparkPlug/Path.h>


namespace SparkPlug
{

class FileInfo
{
	public:
		FileInfo( const Path& path, bool followSymbolicLinks = true );
		
		bool good() const; /// Wether stat() succeeded
		
		time_t accessTime() const; /// Note that some (file)systems dont support this
		time_t creationTime() const; /// Note that some (file)systems dont support this
		time_t modificationTime() const;
		
		int size() const;
		
		bool isRegularFile() const;
		bool isDirectory() const;
		
		// Only available under Unix systems (on windows they simply return false all the time)
		bool isBlockDevice() const;
		bool isCharacterDevice() const;
		bool isFifo() const;
		bool isLink() const;
		bool isSocket() const;
	
	protected:
		void fail();
		
#if defined(__WIN32__)
		struct _stat64 m_Stat;
#else
		struct stat m_Stat;
#endif
};

}

#endif
