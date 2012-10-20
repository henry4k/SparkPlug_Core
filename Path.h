#ifndef __SPARKPLUG_PATH__
#define __SPARKPLUG_PATH__

#include <string>


namespace SparkPlug
{

/// Ein Schiff.
/*!
	Relative:
	foo/bar
	A path that is relative to the current working directory.
	
	Absolute:
	/foo/bar
	A path that originates at pixanitys root directory.
*/
class Path
{
	public:
		Path();
		Path( const Path& path );
		Path( const std::string& path );
		Path( const char* path );
		~Path();
		
		Path& operator=( const Path& path );
		
		operator const std::string&() const;
		const char* c_str() const { return m_Path.c_str(); }
		
		bool operator==( const Path& path ) const;
		bool operator!=( const Path& path ) const { return !(*this == path); }
	
		/// Appends a new element (directory or file)
		Path& operator/=( const Path& sub );
		
		/// Concatenates this path with a new element and returns it.
		Path operator/( const Path& sub ) const;
		
		/// Appends directly to the string
		Path& operator+=( const std::string& sub );
		Path& operator+=( const char* sub );
		
		/// Concatenates this path with a string
		Path operator+( const std::string& sub ) const;
		Path operator+( const char* sub ) const;
		
		/// Check if this is an relative path.
		bool isRelativePath() const;
		
		/// Check if this is an absolute path i.e. it is independent from working directory.
		bool isAbsolutePath() const;
		
		enum
		{
			Relative = 0,
			Absolute
		};
		int pathType() const;
		
		
		/// Returns path to the containing directory
		Path dirName() const;
		
		/// Returns trailing name component of path
		/*!
			\param skipExtension Filter out the file extension
		*/
		std::string baseName( bool skipExtension = false ) const;
		
		/// Returns the file extension
		std::string extension() const;
		
		/// Returns a canonical version of this path
		/*!
			It removes unneccessary '.' and '..' elements
			and ensures that directory seperators fit to the system.
		*/
		Path canonical() const;
		
		/// Returns an absolute version of this path
		/*!
			It removes '.' and '..' elements
			and makes it independent from working directory.
		*/
		Path absolute() const;
		
	protected:
		void canonicalize( Path& out, int type ) const;
		
		std::string m_Path;
		
		friend Path CurDir();
		friend void SetDir( const Path& path );
};

// #include <iostream>
// std::ostream& operator<<( std::ostream& o, const Path& v );


Path CurDir();
void SetDir( const Path& path );

}

#endif
