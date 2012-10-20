#ifndef __SPARKPLUG_DIRECTORY__
#define __SPARKPLUG_DIRECTORY__

#include <string>
#include <SparkPlug/Path.h>

namespace SparkPlug
{

/**
	Directory iterator
	Open and iterate through a directory
**/
class Directory
{
	public:
		Directory();
		Directory( const Path& path );
		~Directory();
		
		void open( const Path& path );
		void close();
		
		/// The path to the directory
		const Path& path() const;
		
		/// Wether or not to emit special directorys like '.' and '..'
		void omitSpecial( bool enable );
		
		/// true if another entry is available
		bool next();
		
		/// true if there are no further entries
		bool end() const;
		
		/// The basename of the current entry
		std::string entryName() const;
		
		/// The path to the current entry
		Path entryPath() const;
		
	protected:
		bool m_OmitSpecial;
		Path m_Path;
		void* m_Data;
		bool m_End;
};

}

#endif
