// File name: Array.h
// Des: This class is a dynamic array.
// Date: 19/3/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef ARRAY_H
#define ARRAY_H

#include "../kgedef.h"
#include "KgeMemory.h"

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
namespace core
{


//! This class is a dynamic array.
template <class T>
class Array
{
private:

	struct ArrayItem
	{
		ArrayItem* prev;
		T		   Item;
		ArrayItem* next;

	}; /// ArrayItem

public:

	//! Constructor.
	Array()
	{
		// Create a domming.
		ArrayItem* temp = KGE_NEW(ArrayItem);
		temp->Item		= NULL;
		temp->next		= NULL;
		temp->prev		= NULL;
		start			= temp;
		end				= temp;
		m_iCount		= 0;

	} // Constructor.

	// Destructor
	~Array()
	{
		Clear();
		KGE_DELETE(start, ArrayItem);

	} // Destructor

	/*! Adds an item to the end of the Array.
	\param Item A pointer which will be add to the end of the array.
	*/
	void PushBack(T Item)
	{
		ArrayItem* temp = KGE_NEW(ArrayItem);
		temp->prev		= end;
		temp->Item		= Item;
		temp->next		= NULL;
		end->next		= temp;
		end				= temp;
		++m_iCount;

	} // PushBack.

	//! Directly access to an item in array.
	T operator [](u32 Index)
	{
		u32		   i    = 0;
		ArrayItem* temp = start->next;
		while (i < Index)
		{
			temp = temp->next;
			++i;
		}
		return temp->Item;

	} // operator []

	//! Removes an item from array.
	void remove(T Item)
	{
		ArrayItem* temp = start->next;
		while (temp->next)
		{
			if (temp->Item == Item)
			{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				KGE_DELETE(temp, ArrayItem);
				return;
			}
			temp = temp->next;
		}

	} // Remove(Item)

	//! Removes all array data
	//! \param DeleteData If you pass true the array data will be deleted with KGE_DELETE macro
	void Clear(bool DeleteData = false)
	{
		ArrayItem* temp = start->next;
		ArrayItem* temp2;
		while (temp)
		{
			temp2 = temp;
			temp = temp->next;
			if (DeleteData)
				KGE_DELETE(temp2->Item, T);
			KGE_DELETE(temp2, ArrayItem);
		}
		start->next = NULL;
		end = start;

	}

	//! Return Items count in the array.
	u32 Count() 
	{ 
		return m_iCount; 
	}

protected:
	
	//! The start link.
	ArrayItem *start;
	//! The end link.
	ArrayItem *end;

	//! The count of the items.
	u32 m_iCount;

}; // Array

} // core

} // kge

#endif // ARRAY_H