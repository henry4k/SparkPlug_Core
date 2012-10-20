/// ---- RefCounter ----

template<typename T>
RefCounter* RefCounter::Create( T* pointer )
{
	return new RefCounter(pointer, DeleteFnT<T>);
}

template<typename T>
void RefCounter::DeleteFnT( void* p )
{
	delete reinterpret_cast<T*>(p);
}


/// ---- ReferenceT ----

template< typename T >
T* ReferenceT<T>::get() const
{
	if(m_RefCounter)
		return reinterpret_cast<T*>( reinterpret_cast<char*>(m_RefCounter->getPointer())+m_PointerOffset);
	else
		return NULL;
}

template< typename T >
const T* ReferenceT<T>::getConst() const
{
	if(m_RefCounter)
		return reinterpret_cast<T*>( reinterpret_cast<const char*>(m_RefCounter->getConstPointer())+m_PointerOffset);
	else
		return NULL;
}

template< typename T >
T* ReferenceT<T>::operator->() const
{
	return get();
}

template< typename T >
ReferenceT<T>::ReferenceT() :
	Reference()
{
}

template< typename T >
void ReferenceT<T>::setByPointer( T* ptr )
{
	m_RefCounter = RefCounter::Create<T>(ptr);
	m_PointerOffset = 0;
}

template< typename T >
template< typename T1 >
void ReferenceT<T>::setByReference( const Reference& ref )
{
	m_RefCounter = ref.m_RefCounter;
	m_PointerOffset = ref.m_PointerOffset+( sizeof(T)-sizeof(T1) );
}


/// ---- StrongRef ----

template< typename T >
StrongRef<T>::StrongRef()
{
}

template< typename T >
StrongRef<T>::StrongRef( T* ptr )
{
	ReferenceT<T>::setByPointer(ptr);
	this->Reference::m_RefCounter->addStrongRef();
}

template< typename T>
template< typename T1 >
StrongRef<T>::StrongRef( const ReferenceT<T1>& ref )
{
	ReferenceT<T>::template setByReference<T1>(ref);
	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->addStrongRef();
}

template< typename T >
StrongRef<T>::~StrongRef()
{
	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->releaseStrongRef();
}

template< typename T >
template< typename T1 >
StrongRef<T>& StrongRef<T>::operator=( const ReferenceT<T1>& ref )
{
	if(this->Reference::m_RefCounter == ref.m_RefCounter)
		return *this;

	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->releaseStrongRef();

	ReferenceT<T>::template setByReference<T1>(ref);

	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->addStrongRef();

	return *this;
}


/// ---- WeakRef ----

template< typename T >
WeakRef<T>::WeakRef()
{
}

template< typename T >
template< typename T1 >
WeakRef<T>::WeakRef( const ReferenceT<T1>& ref )
{
	ReferenceT<T>::template setByReference<T1>(ref);
	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->addListener(this);
}

template< typename T >
WeakRef<T>::~WeakRef()
{
	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->removeListener(this);
}

template< typename T >
template< typename T1 >
WeakRef<T>& WeakRef<T>::operator=( const ReferenceT<T1>& ref )
{
	if(this->Reference::m_RefCounter == ref.m_RefCounter)
		return *this;

	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->removeListener(this);

	ReferenceT<T>::template setByReference<T1>(ref);

	if(this->Reference::m_RefCounter)
		this->Reference::m_RefCounter->addListener(this);

	return *this;
}

template< typename T >
void WeakRef<T>::onZeroReferences( void* pointer )
{
	this->Reference::m_RefCounter = NULL;
	this->Reference::m_PointerOffset = 0;
}
