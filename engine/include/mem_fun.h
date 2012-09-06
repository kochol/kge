// File name: mem_fun.h
// Des: This class is for keeping pointer to member functions and creating them.
// Date: 10/5/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef MEM_FUN_H
#define MEM_FUN_H

#include "Functor.h"

//! \cond

namespace kge
{
namespace core
{
//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with no argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn>
class MFunctor0 : public Functor0<Treturn>
{
public:

	//! Constructor.
	MFunctor0(Tclass* obj, Treturn(Tclass::*_fun)()):
	  m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with no argument.
	Treturn Call()
	{
		return (*m_pObj.*m_pFun)();
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)();		/**< Pointer to member function */

}; // MFunctor0

//------------------------------------------
//! This functions craete a MFunctor0 class.
//------------------------------------------
template <class Tclass, class Treturn>
MFunctor0<Tclass, Treturn>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)() ) 
{
	MFunctor0<Tclass, Treturn>* zFunc = new MFunctor0<Tclass, Treturn>(obj,_fun);
	return zFunc;

} // MFunctor0 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with one argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1>
class ZFunctor1 : public Functor1<Treturn , Targ1>
{
public:

	//! Constructor.
	ZFunctor1(Tclass* obj, Treturn(Tclass::*_fun)(Targ1)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with one argument.
	Treturn Call(Targ1 arg1)
	{
		return (*m_pObj.*m_pFun)(arg1);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1);		/**< Pointer to member function */

}; // ZFunctor1

//------------------------------------------
//! This functions craete a ZFunctor1 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1>
ZFunctor1<Tclass, Treturn , Targ1>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1) ) 
{
	ZFunctor1<Tclass, Treturn , Targ1>* zFunc = new ZFunctor1<Tclass, Treturn , Targ1>(obj,_fun);
	return zFunc;

} // ZFunctor1 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with two argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2>
class ZFunctor2 : public Functor2<Treturn , Targ1 , Targ2>
{
public:

	//! Constructor.
	ZFunctor2(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with two argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2);		/**< Pointer to member function */

}; // ZFunctor2

//------------------------------------------
//! This functions craete a ZFunctor2 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2>
ZFunctor2<Tclass, Treturn , Targ1 , Targ2>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2) ) 
{
	ZFunctor2<Tclass, Treturn , Targ1 , Targ2>* zFunc = new ZFunctor2<Tclass, Treturn , Targ1 , Targ2>(obj,_fun);
	return zFunc;

} // ZFunctor2 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with three argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3>
class ZFunctor3 : public Functor3<Treturn , Targ1 , Targ2 , Targ3>
{
public:

	//! Constructor.
	ZFunctor3(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with three argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3);		/**< Pointer to member function */

}; // ZFunctor3

//------------------------------------------
//! This functions craete a ZFunctor3 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3>
ZFunctor3<Tclass, Treturn , Targ1 , Targ2 , Targ3>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3) ) 
{
	ZFunctor3<Tclass, Treturn , Targ1 , Targ2 , Targ3>* zFunc = new ZFunctor3<Tclass, Treturn , Targ1 , Targ2 , Targ3>(obj,_fun);
	return zFunc;

} // ZFunctor3 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with four argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4>
class ZFunctor4 : public Functor4<Treturn , Targ1 , Targ2 , Targ3 , Targ4>
{
public:

	//! Constructor.
	ZFunctor4(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3 , Targ4)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with four argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3 , arg4);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4);		/**< Pointer to member function */

}; // ZFunctor4

//------------------------------------------
//! This functions craete a ZFunctor4 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4>
ZFunctor4<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4) ) 
{
	ZFunctor4<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4>* zFunc = new ZFunctor4<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4>(obj,_fun);
	return zFunc;

} // ZFunctor4 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with five argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5>
class ZFunctor5 : public Functor5<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>
{
public:

	//! Constructor.
	ZFunctor5(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with five argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5);		/**< Pointer to member function */

}; // ZFunctor5

//------------------------------------------
//! This functions craete a ZFunctor5 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5>
ZFunctor5<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5) ) 
{
	ZFunctor5<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>* zFunc = new ZFunctor5<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>(obj,_fun);
	return zFunc;

} // ZFunctor5 mem_fun

//------------------------------------------
//! This functions craete a ZFunctor5 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5>
ZFunctor5<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>* mem_fun5 ( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5) ) 
{
    ZFunctor5<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>* zFunc = new ZFunctor5<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>(obj,_fun);
    return zFunc;

} // ZFunctor5 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with six argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6>
class ZFunctor6 : public Functor6<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>
{
public:

	//! Constructor.
	ZFunctor6(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with six argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6);		/**< Pointer to member function */

}; // ZFunctor6

//------------------------------------------
//! This functions craete a ZFunctor6 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6>
ZFunctor6<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6) ) 
{
	ZFunctor6<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>* zFunc = new ZFunctor6<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>(obj,_fun);
	return zFunc;

} // ZFunctor6 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with seven argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7>
class ZFunctor7 : public Functor7<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>
{
public:

	//! Constructor.
	ZFunctor7(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with seven argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7);		/**< Pointer to member function */

}; // ZFunctor7

//------------------------------------------
//! This functions craete a ZFunctor7 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7>
ZFunctor7<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7) ) 
{
	ZFunctor7<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>* zFunc = new ZFunctor7<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>(obj,_fun);
	return zFunc;

} // ZFunctor7 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with eight argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8>
class ZFunctor8 : public Functor8<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>
{
public:

	//! Constructor.
	ZFunctor8(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with eight argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8);		/**< Pointer to member function */

}; // ZFunctor8

//------------------------------------------
//! This functions craete a ZFunctor8 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8>
ZFunctor8<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8) ) 
{
	ZFunctor8<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>* zFunc = new ZFunctor8<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>(obj,_fun);
	return zFunc;

} // ZFunctor8 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with nine argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9>
class ZFunctor9 : public Functor9<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>
{
public:

	//! Constructor.
	ZFunctor9(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with nine argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8 , arg9);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9);		/**< Pointer to member function */

}; // ZFunctor9

//------------------------------------------
//! This functions craete a ZFunctor9 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9>
ZFunctor9<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9) ) 
{
	ZFunctor9<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>* zFunc = new ZFunctor9<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>(obj,_fun);
	return zFunc;

} // ZFunctor9 mem_fun

//----------------------------------------------------------------------------
//! This class is for storing pointer to member functions with ten argument(s).
//----------------------------------------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9 , class Targ10>
class ZFunctor10 : public Functor10<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>
{
public:

	//! Constructor.
	ZFunctor10(Tclass* obj, Treturn(Tclass::*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10)):
		m_pObj(obj), m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with ten argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9 , Targ10 arg10)
	{
		return (*m_pObj.*m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8 , arg9 , arg10);
	} // Call

protected:
	Tclass* m_pObj;					/**< Pointer to class object. */
	Treturn(Tclass::*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10);		/**< Pointer to member function */

}; // ZFunctor10

//------------------------------------------
//! This functions craete a ZFunctor10 class.
//------------------------------------------
template <class Tclass, class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9 , class Targ10>
ZFunctor10<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>* mem_fun	( Tclass *obj, Treturn(Tclass::*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9 , Targ10 arg10) ) 
{
	ZFunctor10<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>* zFunc = new ZFunctor10<Tclass, Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>(obj,_fun);
	return zFunc;

} // ZFunctor10 mem_fun


} // core

} // kge

//! \endcond

#endif // MEM_FUN_H