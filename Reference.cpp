#include <SparkPlug/Common.h>
#include <SparkPlug/Reference.h>

namespace SparkPlug
{

/// ---- ReferenceCounted ----
ReferenceCounted::ReferenceCounted() :
	m_StrongReferences(0)
{
}

ReferenceCounted::~ReferenceCounted()
{
	// Inform all listeners that there are no references left
	std::set<ReferenceListener*>::const_iterator i = m_Listeners.begin();
	for(; i != m_Listeners.end(); ++i)
	{
		(*i)->onZeroReferences(this);
	}
}

void ReferenceCounted::addStrongRef()
{
	++m_StrongReferences;
}

void ReferenceCounted::releaseStrongRef()
{
	--m_StrongReferences;
	if(m_StrongReferences <= 0)
	{
		delete this;
	}
}
		
void ReferenceCounted::addListener( ReferenceListener* listener )
{
	m_Listeners.insert(listener);
}

void ReferenceCounted::removeListener( ReferenceListener* listener )
{
	m_Listeners.erase(listener);
}

void ReferenceCounted::enterImmortalSection()
{
	if(m_StrongReferences > 0)
		FatalError("Locking reference %u in used state.", this);
	
	++m_StrongReferences;
}

void ReferenceCounted::leaveImmortalSection()
{
	--m_StrongReferences;
	
	if(m_StrongReferences < 0)
		FatalError("Unlocking reference %u in deleted state.", this);
}


/// ---- Reference ----

Reference::Reference() :
	m_RefCounted(NULL)
{
}

Reference::operator bool() const
{
	return m_RefCounted;
}

bool Reference::operator!() const
{
	return !m_RefCounted;
}

bool Reference::operator==( const Reference& other ) const
{
	return m_RefCounted == other.m_RefCounted;
}

bool Reference::operator!=( const Reference& other ) const
{
	return m_RefCounted != other.m_RefCounted;
}

bool Reference::operator<( const Reference& other ) const
{
	return m_RefCounted < other.m_RefCounted;
}

std::ostream& operator<<( std::ostream& o, const Reference& v )
{
	o << v.m_RefCounted << std::endl;
 	return o;
}

}
