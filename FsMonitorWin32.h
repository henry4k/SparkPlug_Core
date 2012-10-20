/// --- Windows ---

#define _WIN32_WINNT 0x0550
#include <windows.h>

#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif

class FsMonitorImpl;

class FsWatchImpl : public FsWatch
{
public:
	OVERLAPPED overlapped;
	HANDLE dirHandle;
	BYTE buffer[32 * 1024];
	LPARAM lParam;
	DWORD notifyFilter;
	bool stopNow;
	FsMonitorImpl* fileWatcher;
};

class FsMonitorImpl
{
public:
	FsWatch m_LastWatchId;
	map<FsWatch, FsWatchImpl*> m_Watches;
	FsMonitor* m_Monitor;
	
/// Internal begin
	
	bool RefreshWatch( FsWatchImpl* watch, bool clear = false );
	
	/// Unpacks events and passes them to a user defined callback.
	void CALLBACK WatchCallback( DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped )
	{
		TCHAR szFile[MAX_PATH];
		PFILE_NOTIFY_INFORMATION pNotify;
		FsWatchImpl* watch = (FsWatchImpl*) lpOverlapped;
		size_t offset = 0;
		
		if(dwNumberOfBytesTransfered == 0)
			return;
		
		if (dwErrorCode == ERROR_SUCCESS)
		{
			do
			{
				pNotify = (PFILE_NOTIFY_INFORMATION) &watch->buffer[offset];
				offset += pNotify->NextEntryOffset;
				
#			if defined(UNICODE)
				{
					lstrcpynW(szFile, pNotify->FileName,
						min(MAX_PATH, pNotify->FileNameLength / sizeof(WCHAR) + 1));
				}
#			else
				{
					int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName,
						pNotify->FileNameLength / sizeof(WCHAR),
						szFile, MAX_PATH - 1, NULL, NULL);
					szFile[count] = TEXT('\0');
				}
#			endif
				
				Path p = Path::BySystemPath(watch->path)/szFile;
				m_Monitor->onFsEvent(watch, p, pNotify->Action);
				
			} while (pNotify->NextEntryOffset != 0);
		}
		
		if (!watch->stopNow)
		{
			RefreshWatch(watch);
		}
	}
	
	/// Refreshes the directory monitoring.
	bool RefreshWatch( FsWatchImpl* watch, bool clear )
	{
		return ReadDirectoryChangesW(
			watch->dirHandle, watch->buffer, sizeof(watch->buffer), FALSE,
			watch->notifyFilter, NULL, &watch->overlapped, clear ? 0 : WatchCallback) != 0;
	}
	
	/// Stops monitoring a directory.
	void DestroyWatch( FsWatchImpl* watch )
	{
		if (watch)
		{
			watch->stopNow = TRUE;
			
			CancelIo(watch->dirHandle);
			
			RefreshWatch(watch, true);
			
			if(!HasOverlappedIoCompleted(&watch->overlapped))
			{
				SleepEx(5, TRUE);
			}
			
			CloseHandle(watch->overlapped.hEvent);
			CloseHandle(watch->dirHandle);
			
			delete watch;
		}
	}
	
	/// Starts monitoring a directory.
	FsWatchImpl* CreateWatch( LPCTSTR szDirectory, DWORD notifyFilter )
	{
		FsWatchImpl* watch = new FsWatchImpl;
		
		watch->dirHandle = CreateFile(szDirectory, FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, 
			OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
		
		if(watch->dirHandle == INVALID_HANDLE_VALUE)
			LogFatalError() << "Invalid directory " << szDirectory << endl;

		watch->overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		watch->notifyFilter = notifyFilter;
		
		if(!RefreshWatch(watch))
			FatalError();
		return watch;
	}
	
/// Internal end
	
	
	FsMonitorImpl( FsMonitor* monitor ) :
		m_LastWatchId(0),
		m_Monitor(monitor)
	{
	}
	
	~FsMonitorImpl()
	{
		map<FsWatch, FsWatchImpl>::iterator i = m_Watches.begin();
		for(; i != m_Watches.end(); i++)
			DestroyWatch(&i->second);
		m_Watches.clear();
	}
	
	FsWatch* add( const Path& path )
	{
		string systemPath = path.toSystemPath();
		FsWatch watchid = ++m_LastWatchId;
		
		FsWatchImpl* watch = CreateWatch(systemPath.c_str(), FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME);
		
		watch.id = watchid;
		watch.fileWatcher = this;
		watch.path = systemPath;
		
		m_Watches[watchid] = watch;
		
		return watch;
	}
	
	void remove( FsWatch watch )
	{
		map<FsWatch, FsWatchImpl*>::iterator i = m_Watches.find( watch );
		if(i == m_Watches.end())
			return;
		
		DestroyWatch(&i->second);
		m_Watches.erase(i);
	}
	
	void remove( const Path& path )
	{
		string systemPath = path.toSystemPath();
		map<FsWatch, FsWatchImpl*>::iterator i = m_Watches.begin();
		for(; i != m_Watches.end(); i++)
		{
			if(path == i->second->path)
			{
				DestroyWatch(&i->second);
				m_Watches.erase(i);
			}
		}
	}
	
	void update()
	{
		MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
	}
	
	void handleEvent( FsWatchImpl* watch, const string& path, unsigned long action )
	{
		int event;
		switch(action)
		{
			case FILE_ACTION_RENAMED_NEW_NAME:
			case FILE_ACTION_ADDED:
				event = FsEvent::Add;
				break;
			case FILE_ACTION_RENAMED_OLD_NAME:
			case FILE_ACTION_REMOVED:
				event = FsEvent::Delete;
				break;
			case FILE_ACTION_MODIFIED:
				event = FsEvent::Modified;
				break;
		};

		watch->mFileWatchListener->onFsEvent(watch->id, watch->path, path, event);
	}
};
