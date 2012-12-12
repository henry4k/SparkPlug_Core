#ifndef __SPARKPLUG_SINGLETON__
#define __SPARKPLUG_SINGLETON__

#include <SparkPlug/Common.h>


namespace SparkPlug
{

/**
 * Helper function that stores a singleton.
 * Do not use directly!
 */
template<class T>
T*& Singleton_()
{
	static T* s = NULL;
	return s;
}

/**
 * Just derive your class from this one
 * and it will automatically create a singleton once its constructed.
 * Its assured that only one instance exists at a time.
 */
template<class T>
class SingletonClass
{
	protected:
		SingletonClass()
		{
			if( Singleton_<T>() != NULL ) // Only one instance at a time!
				FatalError();
			Singleton_<T>() = static_cast<T*>(this);
		}
		
		virtual ~SingletonClass()
		{
			Singleton_<T>() = NULL;
		}
};

/**
 * Lets you access a singleton.
 * @return Pointer to the singleton.
 */
template<class T>
T* Singleton()
{
	return Singleton_<T>();
}

}

#endif
