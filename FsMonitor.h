#ifndef __SPARKPLUG_FSMONITOR__
#define __SPARKPLUG_FSMONITOR__

#include <vector>

namespace SparkPlug
{


namespace FsEvent
{
enum Enum
{
	Add,
	Delete,
	Modified
};
}

class FsEventHandler
{
	public:
		/**
		 * Handles the file action
		 * @param watchid The watch id for the directory
		 * @param path What has changed
		 * @param action Action that was performed
		 */
		virtual void onFsEvent( int id, const Path& path, int event ) = 0;
};

class FsWatch;
class FsMonitor;
class FsMonitorImpl;


class FsMonitor // Singleton?
{
	public:
		FsMonitor();
		~FsMonitor();
		
		/**
		 * Add a directory watch
		 */
		int add( const std::string& path, FsEventHandler* handler, bool recursive = false );
		
		/**
		 * Remove a directory watch
		 */
		void remove( int watch );
		
		/**
		 * Updates the watcher. Must be called often.
		 */
		void update();
	
	protected:
		FsMonitorImpl* m_Impl;
		friend class FsMonitorImpl;
		void onFsEvent( FsWatch* watch, const Path& path, int event );
};


}

#endif
