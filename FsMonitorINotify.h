/// --- Linux ---

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/inotify.h>
#define BUFF_SIZE ((sizeof(struct inotify_event)+FILENAME_MAX)*1024)


namespace SparkPlug
{

class FsWatchImpl : public FsWatch
{
public:
	// ...
};

class FsMonitorImpl
{
public:
	int m_FD;
	struct timeval m_TimeOut;
	fd_set m_DescriptorSet;
	std::map<int, FsWatchImpl*> m_Watches;
	FsMonitor* m_Monitor;
	
	FsMonitorImpl( FsMonitor* monitor )
	{
		m_FD = inotify_init();
		assert(m_FD >= 0);
		
		m_TimeOut.tv_sec = 0;
		m_TimeOut.tv_usec = 0;
		
		FD_ZERO(&m_DescriptorSet);
		
		m_Monitor = monitor;
	}
	
	~FsMonitorImpl()
	{
		std::map<int, FsWatchImpl*>::const_iterator i = m_Watches.begin();
		for(; i != m_Watches.end(); i++)
			delete i->second;
		m_Watches.clear();
	}
	
	FsWatch* add( const std::string& path )
	{
		int wd = inotify_add_watch(m_FD, path.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO | IN_CREATE | IN_MOVED_FROM | IN_DELETE);
		if(wd < 0)
		{
			if(errno == ENOENT)
				LogError("Invalid path %s", path.c_str());
			assert(false);
		}
		
		FsWatchImpl* w = new FsWatchImpl;
		w->id = wd;
		w->path = path;
		m_Watches[wd] = w;
		return w;
	}
	
	void remove( int watch )
	{
		std::map<int, FsWatchImpl*>::iterator i = m_Watches.find( watch );
		if(i == m_Watches.end())
			return;
		
		inotify_rm_watch(m_FD, watch);
		delete i->second;
		m_Watches.erase(i);
	}
	
	void remove( const std::string& path )
	{
		std::map<int, FsWatchImpl*>::iterator i = m_Watches.begin();
		for(; i != m_Watches.end(); i++)
		{
			if(path == i->second->path)
			{
				inotify_rm_watch(m_FD, i->second->id);
				delete i->second;
				m_Watches.erase(i);
			}
		}
	}
	
	void update()
	{
		FD_SET(m_FD, &m_DescriptorSet);
		
		int ret = select(m_FD + 1, &m_DescriptorSet, NULL, NULL, &m_TimeOut);
		assert(ret >= 0);

		if(FD_ISSET(m_FD, &m_DescriptorSet))
		{
			ssize_t len, i = 0;
			char action[81+FILENAME_MAX];
			char buff[BUFF_SIZE];
			std::memset(action, 0, sizeof(action));
			std::memset(buff, 0, sizeof(buff));
			
			len = read(m_FD, buff, BUFF_SIZE);
			
			while(i < len)
			{
				struct inotify_event* pevent = (struct inotify_event*)&buff[i];
				
				FsWatchImpl* watch = m_Watches[pevent->wd];
				handleEvent(watch, pevent->name, pevent->mask);
				i += sizeof(struct inotify_event) + pevent->len;
			}
		}
	}
	
	void handleEvent( FsWatchImpl* watch, const std::string& path, unsigned long action )
	{
		
		std::string p = Path(watch->path)/path;
		
		if(IN_CLOSE_WRITE & action)
			m_Monitor->onFsEvent(watch, p, FsEvent::Modified);
		
		if(IN_MOVED_TO & action || IN_CREATE & action)
			m_Monitor->onFsEvent(watch, p, FsEvent::Add);
		
		if(IN_MOVED_FROM & action || IN_DELETE & action)
			m_Monitor->onFsEvent(watch, p, FsEvent::Delete);
	}
};

}
