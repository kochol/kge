// File name: Functor.h
// Des: This class is an interface for pointer to functions and pointer to member functions.
// Date: 10/5/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef FUNCTOR_H
#define FUNCTOR_H

//! \cond

namespace kge
{
namespace core
{

//! This class is for calling a function with no argument.
template <class Treturn>
class Functor0 
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call() = 0;

}; // Functor0

//! This class is for calling a function with one argument.
template <class Treturn , class Targ1>
class Functor1
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1) = 0;

}; // Functor1

//! This class is for calling a function with two argument.
template <class Treturn , class Targ1 , class Targ2>
class Functor2
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2) = 0;

}; // Functor2

//! This class is for calling a function with three argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3>
class Functor3
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3) = 0;

}; // Functor3

//! This class is for calling a function with four argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4>
class Functor4
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3 , Targ4) = 0;

}; // Functor4

//! This class is for calling a function with five argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5>
class Functor5
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3 , Targ4 , Targ5) = 0;

}; // Functor5

//! This class is for calling a function with six argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6>
class Functor6
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6) = 0;

}; // Functor6

//! This class is for calling a function with seven argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7>
class Functor7
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7) = 0;

}; // Functor7

//! This class is for calling a function with eight argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8>
class Functor8
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8) = 0;

}; // Functor8

//! This class is for calling a function with nine argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9>
class Functor9
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9) = 0;

}; // Functor9

//! This class is for calling a function with ten argument.
template <class Treturn , class Targ1 , class Targ2 , class Targ3 , class Targ4 , class Targ5 , class Targ6 , class Targ7 , class Targ8 , class Targ9 , class Targ10>
class Functor10
{
public:

	//! Call the user function with no argument.
	virtual Treturn Call(Targ1 , Targ2 , Targ3 , Targ4 , Targ5 , Targ6 , Targ7 , Targ8 , Targ9 , Targ10) = 0;

}; // Functor10

} // core

} // kge

//! \endcond

#endif // FUNCTOR_H