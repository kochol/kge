// File name: ptr_fun.h
// Des: This class is for keeping pointer to functions and creating them.
// Date: 12/5/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef PTR_FUN_H
#define PTR_FUN_H

#include "Functor.h"

namespace kge
{
namespace core
{
//---------------------------------------------------------------------
//! This class is for storing pointer to functions with no argument(s).
//---------------------------------------------------------------------
template <class Treturn>
class PFunctor0 : public Functor0<Treturn>
{
public:

	//! Constructor.
	PFunctor0(Treturn(*_fun)()):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with no argument.
	Treturn Call()
	{
		return (m_pFun)();
	} // Call

protected:
	Treturn(*m_pFun)();		/**< Pointer to function */

}; // ZFunctor0

//------------------------------------------
//! This functions create a PFunctor0 class.
//------------------------------------------
template <class Treturn>
PFunctor0<Treturn>* ptr_fun	( Treturn(*_fun)() ) 
{
	PFunctor0<Treturn>* xFunc = new PFunctor0<Treturn>(_fun);

	return xFunc;

} // PFunctor0 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with one argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1>
class PFunctor1 : public Functor1<Treturn , Targ1>
{
public:

	//! Constructor.
	PFunctor1(Treturn(*_fun)(Targ1)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with one argument.
	Treturn Call(Targ1 arg1)
	{
		return (m_pFun)(arg1);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1);		/**< Pointer to member function */

}; // PFunctor1

//------------------------------------------
//! This functions create a PFunctor1 class.
//------------------------------------------
template <class Treturn , class Targ1>
PFunctor1<Treturn , Targ1>* ptr_fun	( Treturn(*_fun)(Targ1 arg1) ) 
{
	PFunctor1<Treturn , Targ1>* xFunc = new PFunctor1<Treturn , Targ1>(_fun);
	return xFunc;

} // PFuctor1 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with two argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2>
class PFunctor2 : public Functor2<Treturn , Targ1 , Targ2>
{
public:

	//! Constructor.
	PFunctor2(Treturn(*_fun)(Targ1 , Targ2)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with two argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2)
	{
		return (m_pFun)(arg1 , arg2);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2);		/**< Pointer to member function */

}; // PFunctor2

//------------------------------------------
//! This functions create a PFunctor2 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2>
PFunctor2<Treturn , Targ1 , Targ2>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2) ) 
{
	PFunctor2<Treturn , Targ1 , Targ2>* xFunc = new PFunctor2<Treturn , Targ1 , Targ2>(_fun);
	return xFunc;

} // PFuctor2 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with three argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3>
class PFunctor3 : public Functor3<Treturn , Targ1 , Targ2 , Targ3>
{
public:

	//! Constructor.
	PFunctor3(Treturn(*_fun)(Targ1 , Targ2 , Targ3)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with three argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3)
	{
		return (m_pFun)(arg1 , arg2 , arg3);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3);		/**< Pointer to member function */

}; // PFunctor3

//------------------------------------------
//! This functions create a PFunctor3 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3>
PFunctor3<Treturn , Targ1 , Targ2 , Targ3>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3) ) 
{
	PFunctor3<Treturn , Targ1 , Targ2 , Targ3>* xFunc = new PFunctor3<Treturn , Targ1 , Targ2 , Targ3>(_fun);
	return xFunc;

} // PFuctor3 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with four argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4>
class PFunctor4 : public Functor4<Treturn , Targ1 , Targ2 , Targ3 , Targ4>
{
public:

	//! Constructor.
	PFunctor4(Treturn(*_fun)(Targ1 , Targ2 , Targ3 , Targ4)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with four argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4)
	{
		return (m_pFun)(arg1 , arg2 , arg3 , arg4);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4);		/**< Pointer to member function */

}; // PFunctor4

//------------------------------------------
//! This functions create a PFunctor4 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4>
PFunctor4<Treturn , Targ1 , Targ2 , Targ3 , Targ4>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4) ) 
{
	PFunctor4<Treturn , Targ1 , Targ2 , Targ3 , Targ4>* xFunc = new PFunctor4<Treturn , Targ1 , Targ2 , Targ3 , Targ4>(_fun);
	return xFunc;

} // PFuctor4 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with five argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5>
class PFunctor5 : public Functor5<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>
{
public:

	//! Constructor.
	PFunctor5(Treturn(*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with five argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5)
	{
		return (m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5);		/**< Pointer to member function */

}; // PFunctor5

//------------------------------------------
//! This functions create a PFunctor5 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5>
PFunctor5<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5) ) 
{
	PFunctor5<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>* xFunc = new PFunctor5<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5>(_fun);
	return xFunc;

} // PFuctor5 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with six argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6>
class PFunctor6 : public Functor6<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>
{
public:

	//! Constructor.
	PFunctor6(Treturn(*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with six argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6)
	{
		return (m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6);		/**< Pointer to member function */

}; // PFunctor6

//------------------------------------------
//! This functions create a PFunctor6 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6>
PFunctor6<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6) ) 
{
	PFunctor6<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>* xFunc = new PFunctor6<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6>(_fun);
	return xFunc;

} // PFuctor6 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with seven argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7>
class PFunctor7 : public Functor7<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>
{
public:

	//! Constructor.
	PFunctor7(Treturn(*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with seven argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7)
	{
		return (m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7);		/**< Pointer to member function */

}; // PFunctor7

//------------------------------------------
//! This functions create a PFunctor7 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7>
PFunctor7<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7) ) 
{
	PFunctor7<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>* xFunc = new PFunctor7<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7>(_fun);
	return xFunc;

} // PFuctor7 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with eight argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8>
class PFunctor8 : public Functor8<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>
{
public:

	//! Constructor.
	PFunctor8(Treturn(*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with eight argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8)
	{
		return (m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8);		/**< Pointer to member function */

}; // PFunctor8

//------------------------------------------
//! This functions create a PFunctor8 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8>
PFunctor8<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8) ) 
{
	PFunctor8<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>* xFunc = new PFunctor8<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8>(_fun);
	return xFunc;

} // PFuctor8 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with nine argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9>
class PFunctor9 : public Functor9<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>
{
public:

	//! Constructor.
	PFunctor9(Treturn(*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with nine argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9)
	{
		return (m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8 , arg9);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9);		/**< Pointer to member function */

}; // PFunctor9

//------------------------------------------
//! This functions create a PFunctor9 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9>
PFunctor9<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9) ) 
{
	PFunctor9<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>* xFunc = new PFunctor9<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9>(_fun);
	return xFunc;

} // PFuctor9 ptr_fun

//---------------------------------------------------------------------
//! This class is for storing pointer to functions with ten argument(s).
//---------------------------------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9 , class Targ10>
class PFunctor10 : public Functor10<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>
{
public:

	//! Constructor.
	PFunctor10(Treturn(*_fun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10)):
		m_pFun(_fun)
	{
	} // Constructor.

	//! Call the user function with ten argument.
	Treturn Call(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9 , Targ10 arg10)
	{
		return (m_pFun)(arg1 , arg2 , arg3 , arg4 , arg5 , arg6 , arg7 , arg8 , arg9 , arg10);
	} // Call

protected:
	Treturn(*m_pFun)(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10);		/**< Pointer to member function */

}; // PFunctor10

//------------------------------------------
//! This functions create a PFunctor10 class.
//------------------------------------------
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9 , class Targ10>
PFunctor10<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>* ptr_fun	( Treturn(*_fun)(Targ1 arg1 , Targ2 arg2 , Targ3 arg3 , Targ4 arg4 , Targ5 arg5 , Targ6 arg6 , Targ7 arg7 , Targ8 arg8 , Targ9 arg9 , Targ10 arg10) ) 
{
	PFunctor10<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>* xFunc = new PFunctor10<Treturn , Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10>(_fun);
	return xFunc;

} // PFuctor10 ptr_fun

} // core

} // kge

#endif // PTR_FUN_H