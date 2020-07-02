#include <boost/python.hpp>
#include <boost/intrusive_ptr.hpp>

#include <iostream>
using namespace std;
using namespace boost::python;

class RefCounted
{
public:
	RefCounted() : m_numRefs(0) 
	{
		cout << "RefCounted::ctor()" << endl;
	}        
	void addRef() const 
	{ 
		m_numRefs++; 
		cout << "addRef " << m_numRefs << endl;
	}
	void removeRef() const 
	{
		m_numRefs--; 
		cout << "removeRef " << m_numRefs << endl;
		if (!m_numRefs) 
			delete this; 
	}

protected:
	virtual ~RefCounted() 
	{
		cout << "RefCounted::dtor()" << endl;
	}

	mutable long m_numRefs;
};

typedef boost::intrusive_ptr< RefCounted > RefCountedPtr;

inline void intrusive_ptr_add_ref( const RefCounted *r )
{
	r->addRef();
}

inline void intrusive_ptr_release( const RefCounted *r )
{
	r->removeRef();
}


class CppBase : public RefCounted
{
public:
	CppBase() 
	{
		cout << "CppBase::ctor()" << endl;
	}
	virtual ~CppBase() 
	{
		cout << "CppBase::dtor()" << endl;
	}

	virtual void doIt() = 0;
};

class CppDerived : public CppBase
{
public:
	CppDerived() 
	{
		cout << "CppDerived::ctor()" << endl;
	}
	virtual ~CppDerived() 
	{
		cout << "CppDerived::dtor()" << endl;
	}

	virtual void doIt()
	{
		cout << "hello from cpp" << endl;
	}
};

typedef boost::intrusive_ptr< CppDerived > CppDerivedPtr;

typedef boost::intrusive_ptr< CppBase > CppBasePtr;

class CppBaseWrap : public CppBase, public wrapper<CppBase>
{
public:

	virtual void doIt()
	{
		this->get_override("doIt")();
	}

};

typedef boost::intrusive_ptr< CppBaseWrap > CppBaseWrapPtr;

struct Holder
{
	RefCountedPtr m_heldItem;
	Holder() : m_heldItem(0) {}

	void put( RefCountedPtr toHold ) { m_heldItem = toHold; }
	RefCountedPtr get() { return m_heldItem; }
};

typedef boost::shared_ptr< Holder > HolderPtr;

void CppBase_exp()
{
	class_< RefCounted, RefCountedPtr, boost::noncopyable>("RefCounted");

	class_< Holder, HolderPtr, boost::noncopyable>("Holder")
		.def("get", &Holder::get)
		.def("put", &Holder::put)
		;

	class_< CppBaseWrap, CppBaseWrapPtr, boost::noncopyable, bases<RefCounted> >("CppBase")
		.def("doIt", pure_virtual( &CppBaseWrap::doIt) );

	implicitly_convertible<CppBasePtr, RefCountedPtr>();
	implicitly_convertible<CppBaseWrapPtr, CppBasePtr>();
	implicitly_convertible<CppDerivedPtr, CppBasePtr>();

	class_< CppDerived, CppDerivedPtr, bases<CppBase>, boost::noncopyable>("CppDerived")
		.def("doIt", &CppDerived::doIt );
	;

}
