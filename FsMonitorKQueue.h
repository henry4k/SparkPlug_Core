/// --- BSD and MacOSX ---

// This should also work on BSD systems, since it relies on kqueue

#include <sys/event.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define MAX_CHANGE_EVENT_SIZE 2000
typedef struct kevent KEvent;

class EntryStruct
{
public:
	const char* filename;
	time_t modifiedTime;
	
	EntryStruct( const char* f, time_t t = 0 ) :
		filename(f), modifiedTime(t)
	{
	}
	
	~EntryStruct()
	{
		delete[] filename;
	}
};

int comparator( const void* ke1, const void* ke2 )
{
	return strcmp(((EntryStruct*)(((KEvent*)(ke1))->udata))->filename, ((EntryStruct*)(((KEvent*)(ke2))->udata))->filename);
}

class FsWatchImpl : public FsWatch
{
public:
	// index 0 is always the directory
	KEvent changeList[MAX_CHANGE_EVENT_SIZE];
	size_t changeListCount;
	FsMonitor* monitor;
	
	
	
	FsWatchImpl( int watchid, const std::string& p, FsMonitor* m )
	{
		id = watchid;
		path = p;
		monitor = m;

		changeListCount = 0;
		addAll();
	}
	
	void addFile( const std::string& name, bool imitEvents = true )
	{
		// create entry
		struct stat attrib;
		stat(name.c_str(), &attrib);
		
		int fd = open(name.c_str(), O_RDONLY);
		
		if(fd == -1)
			LogFatalError() << "Invalid directory " << name << endl;
		
		++changeListCount;
		
		char* namecopy = new char[name.length() + 1];
		strncpy(namecopy, name.c_str(), name.length());
		namecopy[name.length()] = 0;
		EntryStruct* entry = new EntryStruct(namecopy, attrib.st_mtime);
		
		// set the event data at the end of the list
		EV_SET(&changeList[changeListCount], fd, EVFILT_VNODE,
				EV_ADD | EV_ENABLE | EV_ONESHOT,
				NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_ATTRIB,
				0, (void*)entry);
		
		// qsort
		qsort(changeList + 1, changeListCount, sizeof(KEvent), comparator);
		
		// handle action
		if(imitEvents)
			handleEvent(name, FsEvent::Add);
	}
	
	void removeFile( const std::string& name, bool imitEvents = true )
	{
		// bsearch
		KEvent target;
		EntryStruct tempEntry(name.c_str(), 0);
		target.udata = &tempEntry;
		KEvent* ke = (KEvent*)bsearch(&target, &changeList, changeListCount + 1, sizeof(KEvent), comparator);
		if(!ke)
			LogFatalError() << "Invalid directory " << name << endl;
		
		tempEntry.filename = 0;
		
		// delete
		close(ke->ident);
		delete((EntryStruct*)ke->udata);
		memset(ke, 0, sizeof(KEvent));
		
		// move end to current
		memcpy(ke, &changeList[changeListCount], sizeof(KEvent));
		memset(&changeList[changeListCount], 0, sizeof(KEvent));
		--changeListCount;
		
		// qsort
		qsort(changeList + 1, changeListCount, sizeof(KEvent), comparator);
		
		// handle action
		if(imitEvents)
			handleEvent(name, FsEvent::Delete);
	}
	
	// called when the directory is actually changed
	// means a file has been added or removed
	// rescans the watched directory adding/removing files and sending notices
	void rescan()
	{
		// if new file, call addFile
		// if missing file, call removeFile
		// if timestamp modified, call handleEvent(filename, ACTION_MODIFIED);
		DIR* dir = opendir(path.c_str());
		if(!dir)
			return;
		
		struct dirent* dentry;
		KEvent* ke = &changeList[1];
		EntryStruct* entry = 0;
		struct stat attrib;			
		
		while((dentry = readdir(dir)) != NULL)
		{
			std::string fname = path + "/" + dentry->d_name;
			stat(fname.c_str(), &attrib);
			if(!S_ISREG(attrib.st_mode))
				continue;
			
			if(ke <= &changeList[changeListCount])
			{
				entry = (EntryStruct*)ke->udata;
				int result = strcmp(entry->filename, fname.c_str());
				if(result == 0)
				{
					stat(entry->filename, &attrib);
					time_t timestamp = attrib.st_mtime;
					
					if(entry->modifiedTime != timestamp)
					{
						entry->modifiedTime = timestamp;
						handleEvent(entry->filename, FsEvent::Modified);
					}
					ke++;
				}
				else if(result < 0)
				{
					// f1 was deleted
					removeFile(entry->filename);
					ke++;
				}
				else
				{
					// f2 was created
					addFile(fname);
					ke++;
				}
			}
			else
			{
				// just add
				addFile(fname);
				ke++;
			}
		}//end while
		
		closedir(dir);
	};
	
	void handleEvent( const std::string& filename, int action )
	{
		monitor->onFsEvent(this, Path(path)/filename, action);
	}
	
	void addAll()
	{
		// add base dir
		int fd = open(path.c_str(), O_RDONLY);
		EV_SET(&changeList[0], fd, EVFILT_VNODE,
				EV_ADD | EV_ENABLE | EV_ONESHOT,
				NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_ATTRIB,
				0, 0);
		
		// scan directory and call addFile(name, false) on each file
		DIR* dir = opendir(path.c_str());
		if(!dir)
			LogFatalError() << "Invalid directory " << path << endl;
		
		struct dirent* entry;
		struct stat attrib;
		while((entry = readdir(dir)) != NULL)
		{
			string fname = (path + "/" + string(entry->d_name));
			stat(fname.c_str(), &attrib);
			if(S_ISREG(attrib.st_mode))
				addFile(fname, false);
		}
		
		closedir(dir);
	}
	
	void removeAll()
	{
		KEvent* ke = NULL;
		
		// go through list removing each file and sending an event
		for(int i = 0; i < changeListCount; ++i)
		{
			ke = &changeList[i];
			EntryStruct* entry = (EntryStruct*)ke->udata;
			
			handleEvent(entry->filename, FsEvent::Delete);
			
			// delete
			close(ke->ident);
			delete((EntryStruct*)ke->udata);
		}
	}
};

class FsMonitorImpl
{
public:
	int m_Descriptor;
	struct timespec m_TimeOut;
	int m_LastWatchId;
	map<FsWatch, FsWatchImpl> m_Watches;
	
	FsMonitorImpl( FsMonitor* monitor )
	{
		m_Descriptor = kqueue();
		m_TimeOut.tv_sec = 0;
		m_TimeOut.tv_nsec = 0;
		m_Monitor = monitor;
	}
	
	~FsMonitorImpl()
	{
		m_Watches.clear();
		close(m_Descriptor);
	}
	
	FsWatch* add( const std::string& path )
	{
		FsWatchImpl watch(++mLastWatchId, path);
		m_Watches[mLastWatchId] = watch;
		return &m_Watches[mLastWatchId];
	}
	
	void remove( int watch )
	{
		map<int, FsWatchImpl>::iterator i = m_Watches.find( watch );
		if(i == m_Watches.end())
			return;
		delete i->second;
		m_Watches.erase(i);
	}
	
	void remove( const std::string& path )
	{
		string systemPath = path.toSystemPath();
		map<FsWatch, FsWatchImpl*>::iterator i = m_Watches.begin();
		for(; i != m_Watches.end(); i++)
		{
			if(path == i->second->path)
			{
				delete i->second;
				m_Watches.erase(i);
			}
		}
	}
	
	void update()
	{
		int nev = 0;
		struct kevent event;
		
		map<FsWatch, FsWatchImpl>::iterator i = m_Watches.begin();
		for(; i != m_Watches.end(); i++)
		{
			FsWatchImpl* watch = &i->second;
			
			while((nev = kevent(mDescriptor, (KEvent*)&(watch->changeList), watch->changeListCount + 1, &event, 1, &timeOut)) != 0)
			{
				if(nev == -1)
					LogFatalError() << "kevent" << endl;
				
				EntryStruct* entry = 0;
				if((entry = (EntryStruct*)event.udata) != 0)
				{
					if(event.fflags & NOTE_DELETE)
					{
						watch->removeFile(entry->filename);
					}
					if(event.fflags & NOTE_EXTEND || 
						event.fflags & NOTE_WRITE ||
						event.fflags & NOTE_ATTRIB)
					{
						struct stat attrib;
						stat(entry->filename, &attrib);
						entry->modifiedTime = attrib.st_mtime;
						watch->handleEvent(entry->mFilename, FsEvent::Modified);
					}
				}
				else
				{
					watch->rescan();
				}
			}
		}
	}
};

