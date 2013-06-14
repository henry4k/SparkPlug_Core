#include <map>

#include <SparkPlug/Common.h>
#include <SparkPlug/Platform.h>
#include <SparkPlug/FileInfo.h>
#include <SparkPlug/Directory.h>
#include <SparkPlug/FsMonitor.h>


namespace SparkPlug
{



/// --- FsWatch ---

class FsWatch
{
public:
	FsWatch()
	{
	}

	virtual ~FsWatch()
	{
		std::vector<int>::const_iterator i = childs.begin();
		for(; i != childs.end(); i++)
			monitor->remove(*i);
	}
	
	int id;
	std::string path;
	FsEventHandler* handler;
	FsMonitor* monitor;
	bool recursive;
	std::vector<int> childs;
};

}


#if defined(__WIN32__)
#	include <SparkPlug/FsMonitorWin32.h>
#elif defined(__LINUX__)
# 	include <SparkPlug/FsMonitorINotify.h>
#else
# 	include <SparkPlug/FsMonitorKQueue.h>
#endif


namespace SparkPlug
{

/// --- FsMonitor ---

FsMonitor::FsMonitor() :
	m_Impl( new FsMonitorImpl(this) )
{
}

FsMonitor::~FsMonitor()
{
	delete m_Impl;
}

int FsMonitor::add( const std::string& path, FsEventHandler* handler, bool recursive )
{
	FsWatch* p = m_Impl->add(path);
	p->monitor = this;
	p->handler = handler;
	p->recursive = recursive;
	
	// Add recursivley all subdirectories
	if(recursive)
	{
		Directory dir(path);
		while(dir.next())
		{
			if( FileInfo( dir.entryPath() ).isDirectory() )
			{
				p->childs.push_back( add(dir.entryPath(), handler, recursive) );
			}
		}
	}
	
	return p->id;
}

void FsMonitor::remove( int watch )
{
	m_Impl->remove(watch);
}

void FsMonitor::update()
{
	m_Impl->update();
}

void FsMonitor::onFsEvent( FsWatch* watch, const Path& path, int event )
{
	if(event == FsEvent::Delete)
		m_Impl->remove(path);
	watch->handler->onFsEvent(watch->id, path, event);
}


}
