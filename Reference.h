#ifndef __SPARKPLUG_REFERENCE__
#define __SPARKPLUG_REFERENCE__

#include <iostream>
#include <cstddef>
#include <set>
#include <SparkPlug/Common.h>

namespace SparkPlug
{

class ReferenceCounted;

class ReferenceListener
{
	public:
		virtual ~ReferenceListener() {}

		virtual void onZeroReferences( const ReferenceCounted* pointer ) = 0;
};

class ReferenceCounted
{
	public:
		/**
		 * Adds one reference.
		 */
		void addStrongRef();
		
		/**
		 * Releases one reference.
		 * When there are no reference left this instance and the pointer are deleted.
		 */
		void releaseStrongRef();
		
		/**
		 * ...
		 */
		void addListener( ReferenceListener* listener );
		
		/**
		 * ...
		 */
		void removeListener( ReferenceListener* listener );
	
	protected:
		ReferenceCounted();
		virtual ~ReferenceCounted();
		
		/**
		 * Prevent strong reference from deleting this object.
		 * Usefull inside constructor, when it calls functions that create references.
		 * Dont forget to unlock() the object afterwards.
		 */
		void lock();
		void unlock();

	private:
		int m_StrongReferences;
		std::set< ReferenceListener* > m_Listeners;
};


class Reference
{
	public:
		operator bool() const;
		bool operator!() const;

		bool operator==( const Reference& other ) const;
		bool operator!=( const Reference& other ) const;
		bool operator<( const Reference& other ) const;
	
	//protected:
		ReferenceCounted* m_RefCounted;
	
	protected:
		Reference();
};

std::ostream& operator<<( std::ostream& o, const Reference& v );


template< typename T >
class ReferenceT : public Reference
{
	public:
		T* get() const;
		const T* getConst() const;
		T* operator->() const;
		
	protected:
		ReferenceT();

		template< typename T1 >
		void setByReference( const Reference& ref );

		template< typename T1 >
		void setByReferenceCounted( ReferenceCounted* refCounter );
};


template< typename T >
class StrongRef : public ReferenceT<T>
{
	public:
		StrongRef();
		
		StrongRef( ReferenceCounted* ptr );

		StrongRef( const StrongRef<T>& ref );
		
		template< typename T1 >
		StrongRef( const ReferenceT<T1>& ref );
		
		~StrongRef();
		
		
		StrongRef<T>& operator=( const StrongRef<T>& ref );
		
		template< typename T1 >
		StrongRef<T>& operator=( const ReferenceT<T1>& ref );
};


template< typename T >
class WeakRef : public ReferenceT<T>, public ReferenceListener
{
	public:
		WeakRef();
		
		WeakRef( const WeakRef<T>& ref );
		
		template< typename T1 >
		WeakRef( const ReferenceT<T1>& ref );

		~WeakRef();
		
		WeakRef<T>& operator=( const WeakRef<T>& ref );

		template< typename T1 >
		WeakRef<T>& operator=( const ReferenceT<T1>& ref );

	protected:
		void onZeroReferences( const ReferenceCounted* pointer );
};


#include <SparkPlug/Reference_Impl.h>

}
#endif
