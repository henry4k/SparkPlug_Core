/// ---- ReferenceT ----

template< typename T >
T* ReferenceT<T>::get() const
{
	if(m_RefCounted)
		return reinterpret_cast<T*>(m_RefCounted);
	else
		return NULL;
}

template< typename T >
const T* ReferenceT<T>::getConst() const
{
	if(m_RefCounted)
		return reinterpret_cast<T*>(m_RefCounted);
	else
		return NULL;
}

template< typename T >
T* ReferenceT<T>::operator->() const
{
    T* r = get();
    if(r == NULL)
		FatalError("Trying to access null reference!");
    return r;
}

template< typename T >
ReferenceT<T>::ReferenceT() :
	Reference()
{
}


/// ---- StrongRef ----

template< typename T >
StrongRef<T>::StrongRef() :
	ReferenceT<T>()
{
}

template< typename T >
StrongRef<T>::StrongRef( ReferenceCounted* ptr )
{
	this->Reference::m_RefCounted = ptr;
	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addStrongRef();
}

template< typename T>
StrongRef<T>::StrongRef( const StrongRef<T>& ref ) :
	ReferenceT<T>()
{
	this->Reference::m_RefCounted = ref.get();
	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addStrongRef();
}

template< typename T>
template< typename T1 >
StrongRef<T>::StrongRef( const ReferenceT<T1>& ref ) :
	ReferenceT<T>()
{
	this->Reference::m_RefCounted = ref.get();
	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addStrongRef();
}

template< typename T >
StrongRef<T>::~StrongRef()
{
	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->releaseStrongRef();
}

template< typename T >
StrongRef<T>& StrongRef<T>::operator=( const StrongRef<T>& ref )
{
	// Check for self assignment implicitly done here =)
	if(this->Reference::m_RefCounted == ref.m_RefCounted)
		return *this;

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->releaseStrongRef();

	this->Reference::m_RefCounted = ref.get();

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addStrongRef();

	return *this;
}

template< typename T >
template< typename T1 >
StrongRef<T>& StrongRef<T>::operator=( const ReferenceT<T1>& ref )
{
	// Check for self assignment implicitly done here =)
	if(this->Reference::m_RefCounted == ref.m_RefCounted)
		return *this;

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->releaseStrongRef();

	this->Reference::m_RefCounted = ref.get();

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addStrongRef();

	return *this;
}


/// ---- WeakRef ----

template< typename T >
WeakRef<T>::WeakRef() :
	ReferenceT<T>()
{
}

template< typename T >
WeakRef<T>::WeakRef( const WeakRef<T>& ref ) :
	ReferenceT<T>()
{
	this->Reference::m_RefCounted = ref.get();
	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addListener(this);
}

template< typename T >
template< typename T1 >
WeakRef<T>::WeakRef( const ReferenceT<T1>& ref ) :
	ReferenceT<T>()
{
	this->Reference::m_RefCounted = ref.get();
	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addListener(this);
}

template< typename T >
WeakRef<T>::~WeakRef()
{
	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->removeListener(this);
}

template< typename T >
WeakRef<T>& WeakRef<T>::operator=( const WeakRef<T>& ref )
{
	// Check for self assignment implicitly done here =)
	if(this->Reference::m_RefCounted == ref.m_RefCounted)
		return *this;

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->removeListener(this);

	this->Reference::m_RefCounted = ref.get();

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addListener(this);

	return *this;
}

template< typename T >
template< typename T1 >
WeakRef<T>& WeakRef<T>::operator=( const ReferenceT<T1>& ref )
{
	// Check for self assignment implicitly done here =)
	if(this->Reference::m_RefCounted == ref.m_RefCounted)
		return *this;

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->removeListener(this);

	this->Reference::m_RefCounted = ref.get();

	if(this->Reference::m_RefCounted)
		this->Reference::m_RefCounted->addListener(this);

	return *this;
}

template< typename T >
void WeakRef<T>::onZeroReferences( const ReferenceCounted* pointer )
{
	this->Reference::m_RefCounted = NULL;
}
