#include <cstdlib>
#include <cstring>
#include <vector>
#include <stack>
#include <SparkPlug/Common.h>
#include <SparkPlug/Platform.h>
#include <SparkPlug/Path.h>



namespace SparkPlug
{


#ifdef __WIN32__

#include <direct.h>

static const char DirSeperators[] = "\\/";
static const char DirSeperator = '\\';

Path CurDir()
{
	char* path;

	if((path = _getcwd(NULL, 0)) == NULL)
		return Path();

	Path r(&path[0]);
	free(path);
	if(r.m_Path[r.m_Path.length()-1] != '\\')
		r.m_Path += '\\';
	return r;
}

void SetDir( const Path& path )
{
	_chdir(path.absolute().c_str());
}


#else

#include <unistd.h>

static const char DirSeperators = '/';
static const char DirSeperator = '/';

Path CurDir()
{
	long size = pathconf(".", _PC_PATH_MAX);
	char* buf = new char[size];
	char* path;

	if((path = getcwd(buf, (size_t)size)) == NULL)
		return std::string();

	Path r(path);
	delete[] buf;
	
	if(r.m_Path[r.m_Path.length()-1] != '/')
		r.m_Path.push_back('/');
	return r;
}

void SetDir( const Path& path )
{
	chdir(path.absolute().c_str());
}

#endif

Path Path::dirName() const
{
	size_t pos = m_Path.find_last_of(DirSeperators);
	if(pos != std::string::npos)
		return m_Path.substr(0, pos);
	else
		return "";
}

std::string Path::baseName( bool skipExtension ) const
{
	size_t a = m_Path.find_last_of(DirSeperators);
	if(a == std::string::npos)
		a = 0;
	else
		++a;
	
	size_t b = ( skipExtension ) ? ( m_Path.find_last_of('.') ) : ( std::string::npos );
	if((b == std::string::npos) || (b < a))
		b = std::string::npos;
	else
		b -= a;
	
	return m_Path.substr(a, b);
}

std::string Path::extension() const
{
	std::string name = baseName();
	
	size_t a = name.find_first_of('.');
	if(a == std::string::npos)
		a = 0;
	else
		++a;
	
	return name.substr(a, std::string::npos);
}

bool NextPathElement( std::string& out, size_t& pos, size_t& length, const std::string& path )
{
	if(pos >= (path.length()-1))
		return false;
	
	size_t next = path.find_first_of(DirSeperators, pos);
	if(next == std::string::npos)
		next = path.length();
	
	length = next-pos;
	out = path.substr(pos, length);
	pos = next+1;
	
	if(!length)
		return NextPathElement(out, pos, length, path);
	return true;
}

bool NextPathElementReverse( std::string& out, size_t& pos, size_t& length, const std::string& path ) // pos should start with path.length()-1
{
	if(pos <= 0)
		return false;
	
	size_t next = path.find_last_of(DirSeperators, pos);
	if(next == std::string::npos)
		next = 0;
	
	length = pos-next;
	out = path.substr(pos, length);
	pos = next;
	
	if(!length)
		return NextPathElementReverse(out, pos, length, path);
	return true;
}

void Path::canonicalize( Path& out, int type ) const
{
	// "./Foo/./Bar/../Xyz"
	// "foo/bar/../.."
	// "/home/foo/bar"
	
	// On a relative path the working directory is appened at the front.
	// "." elements will be removed.
	// On ".." elements the previous element will be removed.
	// Path separators will be changed to fit the system.
	
	std::vector<std::string> entries;
	entries.reserve(10); // TODO: Maybe declare this as a constant. <.<

	if((type == Path::Absolute) && !isAbsolutePath())
		out = CurDir();
	
	out /= *this;

	size_t pos = 0;
	size_t length = 0;
	std::string entry;
	while(NextPathElement(entry, pos, length, out.m_Path))
	{
		if(entry == ".")
		{
			continue;
		}
		else if(entry == "..")
		{
			if(!entries.empty())
			{
				entries.pop_back(); // easy peasy ;)
				continue;
			}
			else if(type != Path::Relative)
			{
// 				Expect(false, "Can't process path entry '..'! (path: '%s')", out.m_Path.c_str());
			}
		}
		
		entries.push_back( entry );
	}
	
	switch(type)
	{
		case Path::Absolute:
			// ...
			break;
		case Path::Relative:
		{
			if(entries.empty())
				out.m_Path = ".";
			else
				out.m_Path = "";
		} break;
		
		default:
			FatalError("Invalid path type: %u", type);
	}
	
	for(int i = 0; i < entries.size(); i++)
	{
		out.m_Path += entries[i];
		if(i < entries.size()-1) // not last element?
			out.m_Path.push_back(DirSeperator); // Could fail if you process something like this: '/..', see security test below.
	}
	
// 	if(type != Path::Relative)
// 		Expect(out.isAbsolutePath(), "Processing path '%s' did not yield a valid result. (result: '%s')", m_Path.c_str(), out.m_Path.c_str());
}

Path Path::canonical() const
{
	Path r;
	canonicalize(r, false);
	return r;
}

Path Path::absolute() const
{
	Path r;
	canonicalize(r, true);
	return r;
}

// /foo/bar
bool Path::isAbsolutePath() const
{
#if defined(__WIN32__)
	return (m_Path.size() >= 3) && (m_Path[1] == ':') && (m_Path[2] == '\\');
#else
	return (m_Path.size() >= 1) && (m_Path[0] == '/');
#endif
}

bool Path::isRelativePath() const
{
	return !isAbsolutePath();
}

int Path::pathType() const
{
	if(isRelativePath())
		return Path::Relative;
	else
		return Path::Absolute;
}





Path::Path() :
	m_Path()
{
}

Path::Path( const Path& path ) :
	m_Path( path.m_Path )
{
}

Path::Path( const std::string& path ) :
	m_Path(path)
{
}

Path::Path( const char* path ) :
	m_Path(path)
{
}

Path::~Path()
{
}

Path& Path::operator=( const Path& path )
{
	m_Path = path.m_Path;
	return *this;
}

Path::operator const std::string&() const
{
	return m_Path;
}

/*
Path::operator const char*() const
{
	return m_Path.c_str();
}
*/

bool Path::operator==( const Path& path ) const
{
	if(path.m_Path == m_Path)
		return true;
	return path.absolute().m_Path == absolute().m_Path;
}

Path& Path::operator/=( const Path& sub )
{
	switch(sub.pathType())
	{
		case Path::Relative:
		{
			// If the previous path is not empty and has no separator at the end:
			if(!m_Path.empty()) //&& (m_Path[m_Path.length()-1] != DirSeperators))
				m_Path.push_back(DirSeperator);
			
			m_Path += sub;
		} break;
		
		case Path::Absolute:
		{
			m_Path = sub.m_Path;
		} break;
		
		default:
			FatalError("Invalid path type: %u", sub.pathType());
	}
	
	return *this;
}

Path Path::operator/( const Path& sub ) const
{
	Path r = *this;
	r /= sub;
	return r;
}

Path& Path::operator+=( const std::string& sub )
{
	m_Path += sub;
	return *this;
}

Path& Path::operator+=( const char* sub )
{
	m_Path += sub;
	return *this;
}

Path Path::operator+( const std::string& sub ) const
{
	return Path(m_Path+sub);
}

Path Path::operator+( const char* sub ) const
{
	return Path(m_Path+sub);
}

// ostream& operator<<( ostream& o, const Path& v ) 
// {
// 	o << '"' << v.c_str() << '"';
// 	return o;
// }

}
