#include <string.h>
#include <SparkPlug/FileInfo.h>

namespace SparkPlug
{

FileInfo::FileInfo( const Path& path, bool followSymbolicLinks )
{
#if defined(__WIN32__)
	if( _stat64(path.absolute().c_str(), &m_Stat) != -1 )
		return;
#else
	if( followSymbolicLinks )
	{
		if( stat(path.absolute().c_str(), &m_Stat) != -1 )
			return;
	}
	else
	{
		if( lstat(path.absolute().c_str(), &m_Stat) != -1 )
			return;
	}
#endif
	
	memset(&m_Stat, 0, sizeof(m_Stat));
}

time_t FileInfo::accessTime() const { return m_Stat.st_atime; }
time_t FileInfo::creationTime() const { return m_Stat.st_ctime; }
time_t FileInfo::modificationTime() const { return m_Stat.st_mtime; }

int FileInfo::size() const { return m_Stat.st_size; }

#if defined(__WIN32__)
bool FileInfo::good() const { return m_Stat.st_mode; }
bool FileInfo::isRegularFile() const { return m_Stat.st_mode & _S_IFREG; }
bool FileInfo::isDirectory() const { return m_Stat.st_mode & _S_IFDIR; }
bool FileInfo::isBlockDevice() const { return false; }
bool FileInfo::isCharacterDevice() const { false; }
bool FileInfo::isFifo() const { return false; }
bool FileInfo::isLink() const { return false; }
bool FileInfo::isSocket() const { return false; }
#else
bool FileInfo::good() const { return m_Stat.st_mode; }
bool FileInfo::isRegularFile() const { return m_Stat.st_mode & S_IFREG; }
bool FileInfo::isDirectory() const { return m_Stat.st_mode & S_IFDIR; }
bool FileInfo::isBlockDevice() const { return m_Stat.st_mode & S_IFBLK; }
bool FileInfo::isCharacterDevice() const { return m_Stat.st_mode & S_IFCHR; }
bool FileInfo::isFifo() const { return m_Stat.st_mode & S_IFIFO; }
bool FileInfo::isLink() const { return m_Stat.st_mode & S_IFLNK; }
bool FileInfo::isSocket() const { return m_Stat.st_mode & S_IFSOCK; }

}

#endif
