#include <SparkPlug/Reference.h>

namespace SparkPlug
{

/// ---- RefCounter ----

void RefCounter::addStrongRef()
{
	++m_StrongReferences;
}

void RefCounter::releaseStrongRef()
{
	--m_StrongReferences;
	if(m_StrongReferences <= 0)
	{
		delete this;
	}
}
		
void RefCounter::addListener( IReferenceListener* listener )
{
	m_Listeners.insert(listener);
}

void RefCounter::removeListener( IReferenceListener* listener )
{
	m_Listeners.erase(listener);
}

void* RefCounter::getPointer() const
{
	return m_Pointer;
}

const void* RefCounter::getConstPointer() const
{
	return m_Pointer;
}

RefCounter::RefCounter( void* pointer, DeleteFn deleteFn ) :
	m_Pointer(pointer),
	m_DeleteFn(deleteFn),
	m_StrongReferences(0)
{
}

RefCounter::~RefCounter()
{
	// Inform all listeners that there are no references left
	std::set<IReferenceListener*>::const_iterator i = m_Listeners.begin();
	for(; i != m_Listeners.end(); ++i)
	{
		(*i)->onZeroReferences(m_Pointer);
	}

	// Dun dun
	DeleteFn(m_Pointer);
}




/// ---- Reference ----

Reference::Reference() :
	m_RefCounter(NULL),
	m_PointerOffset(0)
{
}

Reference::operator bool() const
{
	return m_RefCounter && m_RefCounter->getConstPointer();
}

bool Reference::operator!() const
{
	return !m_RefCounter || !m_RefCounter->getConstPointer();
}

}
