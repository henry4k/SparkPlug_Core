#ifndef __SPARKPLUG_REFERENCE__
#define __SPARKPLUG_REFERENCE__

#include <iostream>
#include <cstddef>
#include <set>
#include <SparkPlug/Common.h>

namespace SparkPlug
{

class IReferenceListener
{
	public:
		virtual ~IReferenceListener() {}

		virtual void onZeroReferences( void* pointer ) = 0;
};

class RefCounter
{
	public:
		/**
		 * Creates an instance.
		 * Does not add any references!
		 */
		template<typename T>
		static RefCounter* Create( T* pointer );
		
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
		void addListener( IReferenceListener* listener );
		
		/**
		 * ...
		 */
		void removeListener( IReferenceListener* listener );

		void* getPointer() const;

		const void* getConstPointer() const;
		
	private:
		typedef void (*DeleteFn)(void* p);

		template<typename T>
		static void DeleteFnT( void* p );

		// Refcounter may only be constructed on the heap (so that you can *delete* it)
		RefCounter( void* pointer, DeleteFn deleteFn );
		
		// See constructor info
		~RefCounter();

		void* m_Pointer;
		DeleteFn m_DeleteFn;
		int m_StrongReferences;
		std::set< IReferenceListener* > m_Listeners;
};


class Reference
{
	public:
		operator bool() const;
		bool operator!() const;
	
	//protected:
		RefCounter* m_RefCounter;
		std::size_t m_PointerOffset;
	
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

		void setByPointer( T* ptr );

		template< typename T1 >
		void setByReference( const Reference& ref );
};


template< typename T >
class StrongRef : public ReferenceT<T>
{
	public:
		StrongRef();
		
		explicit StrongRef( T* ptr );
		
		StrongRef( const ReferenceT<T>& ref );
		
		template< typename T1 >
		StrongRef( const ReferenceT<T1>& ref );
		
		~StrongRef();
		
		template< typename T1 >
		StrongRef<T>& operator=( const ReferenceT<T1>& ref );
};


template< typename T >
class WeakRef : public ReferenceT<T>, public IReferenceListener
{
	public:
		WeakRef();
		
		WeakRef( const ReferenceT<T>& ref );
		
		template< typename T1 >
		WeakRef( const ReferenceT<T1>& ref );

		~WeakRef();

		template< typename T1 >
		WeakRef<T>& operator=( const ReferenceT<T1>& ref );

	protected:
		void onZeroReferences( void* pointer );
};


#include <SparkPlug/Reference_Impl.h>

}
#endif
