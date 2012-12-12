#include <SparkPlug/Platform.h>
#include <SparkPlug/Directory.h>


namespace SparkPlug
{


#ifdef __WIN32__

	#include <io.h>

	struct DirData
	{
		intptr_t handle;
		_finddata64_t fileInfo;
	};

#else
	#include <cstddef>
// 	#include <stddef.h>
	#include <dirent.h>
	
	struct DirData
	{
		DIR* handle;
		union
		{
			struct dirent d;
			char b[offsetof(struct dirent, d_name) + NAME_MAX + 1]; // Dunno what this is but we seem to need it.
		} entry;
		struct dirent* result;
	};

#endif

Directory::Directory() :
	m_OmitSpecial(true),
	m_Path(),
	m_Data(0),
	m_End(true)
{
}

Directory::Directory( const Path& path ) :
	m_OmitSpecial(true),
	m_Path(path),
	m_Data(0),
	m_End(true)
{
	open(path);
}

Directory::~Directory()
{
	close();
}

void Directory::open( const Path& path )
{
	if(m_Data)
		close();
	
	m_Data = new DirData;
	DirData& data = *(DirData*)m_Data;
#ifdef __WIN32__
	data.handle = _findfirst64(path.absolute().c_str(), &data.fileInfo);
#else
	data.handle = opendir(path.absolute().c_str());
#endif
	m_End = false;
	m_Path = path;
}

void Directory::close()
{
	if(!m_Data)
		return;
	
#ifdef __WIN32__
	_findclose( ((DirData*)m_Data)->handle );
#else
	closedir( ((DirData*)m_Data)->handle );
#endif
	delete (DirData*)m_Data;
	
	m_Data = 0;
}

const Path& Directory::path() const
{
	return m_Path;
}

void Directory::omitSpecial( bool enable )
{
	m_OmitSpecial = enable;
}

bool Directory::next()
{
	if(m_End)
		return false;
	
	DirData& data = *(DirData*)m_Data;
#ifdef __WIN32__
	m_End = (_findnext64(data.handle, &data.fileInfo) != 0);
#else
	if(readdir_r(data.handle, &data.entry.d, &data.result))
	{
		m_End = true;
		return !m_End;
	}
	m_End = (data.result != &data.entry.d);
#endif
	
	if(m_OmitSpecial && ( (entryName() == ".") || (entryName() == "..") ))
		return next();
	return !m_End;
}

bool Directory::end() const
{
	return m_End;
}

std::string Directory::entryName() const
{
	if(end())
		return std::string();
	#ifdef __WIN32__
		return ((DirData*)m_Data)->fileInfo.name;
	#else
		return ((DirData*)m_Data)->entry.d.d_name;
	#endif
}

Path Directory::entryPath() const
{
	return m_Path/entryName();
}

}
