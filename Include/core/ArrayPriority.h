// File name: ArrayPriority.h
// Des: This class is a dynamic array with priority for auto sorting.
// Date: 21/3/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef ARRAYPRIORITY_H
#define ARRAYPRIORITY_H

#include "../kgedef.h"
#include "../io/Logger.h"
#include "KgeMemory.h"

#ifdef KGE_USE_BOOST
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#endif // KGE_USE_BOOST

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
	namespace core
	{

		//! This class is a dynamic array.
		template <typename T, typename type=int>
		class ArrayPri
		{
		protected:

			struct ArraypItem
			{
				ArraypItem* prev;
				T		    Item;
				type	    Pri;		// Priority
				ArraypItem* next;

			}; /// ArrayItem

#ifdef KGE_USE_BOOST
			friend class boost::serialization::access;
			
			template<class Archive>
			void save(Archive & ar, const unsigned int version) const
			{
				ar & m_iCount;

				if (m_iCount <= 0)
					return;

				type ty;
				T item;

				for (int i = 0; i < m_iCount; i++)
				{
					item = *GetItem(i, ty); 
					ar & *item;
					ar & ty;
				}
			} // save

			template<class Archive>
			void load(Archive & ar, const unsigned int version)
			{
				ar & m_iCount;

				if (m_iCount <= 0)
					return;

				type ty;

				for (int i = 0; i < m_iCount; i++)
				{
// 				T item = new T();
// 					ar & *item;
// 					ar & ty;
// 					PushBack(T, ty);
				}

			} // load

			BOOST_SERIALIZATION_SPLIT_MEMBER()

#endif //  KGE_USE_BOOST

		public:

			//! Constructor.
			ArrayPri()
			{
				// Create a doming.
				ArraypItem* temp = KGE_NEW(ArraypItem);
				temp->Item	 	 = NULL;
				temp->next		 = NULL;
				temp->prev		 = NULL;
				temp->Pri		 = -100;
				start			 = temp;
				m_iCount		 = 0;

			} // Constructor.

			//! Destructor
			~ArrayPri()
			{
				Clear();
				KGE_DELETE(start, ArraypItem);
			}

			/*! Adds an item to the Array with priority sort.
			\param Item A pointer which will be added to the array.
			\param Priority 
			*/
			void PushBack(T Item, type Priority)
			{
				ArraypItem* temp	= start;
				ArraypItem* newitem = KGE_NEW(ArraypItem);
				newitem->Item		= Item;
				newitem->Pri		= Priority;

				while((temp->next != NULL) && (temp->Pri <= Priority))
					temp = temp->next;

				if (temp->Pri > Priority)
				{
					newitem->next		= temp;
					newitem->prev		= temp->prev;
					if (newitem->prev)
						newitem->prev->next = newitem;
					temp->prev			= newitem;
				}
				else
				{
					newitem->next		= NULL;
					newitem->prev		= temp;
					newitem->prev->next = newitem;
				}
				++m_iCount;

			} // PushBack.

			/*! Adds an item to the Array with priority sort but only one level of priority is avilable.<br>
			That means only the first item with the unique priority will be accepted.
			\param Item A pointer which will be added to the array.
			\param Priority 
			*/
			void PushBackOne(T Item, type Priority)
			{
				ArraypItem* temp	= start;
				ArraypItem* newitem = KGE_NEW(ArraypItem);
				newitem->Item		= Item;
				newitem->Pri		= Priority;

				while((temp->next != NULL) && (temp->Pri < Priority))
					temp = temp->next;

				if ( temp->Pri == Priority )
				{
					KGE_DELETE(newitem, ArraypItem);
					return;
				}
				else if (temp->Pri > Priority)
				{
					newitem->next		= temp;
					newitem->prev		= temp->prev;
					if (newitem->prev)
						newitem->prev->next = newitem;
					temp->prev			= newitem;
				}
				else
				{
					newitem->next		= NULL;
					newitem->prev		= temp;
					newitem->prev->next = newitem;
				}
				++m_iCount;

			} // PushBack.

			T& operator [](u32 Index)
			{
				u32		    i    = 0;
				ArraypItem* temp = start->next;
				while (i < Index)
				{
					temp = temp->next;
					++i;
				}
				return temp->Item;

			} // operator []

			const T& operator [](u32 Index) const
			{
				u32		    i    = 0;
				ArraypItem* temp = start->next;
				while (i < Index)
				{
					temp = temp->next;
					++i;
				}
				return temp->Item;

			} // operator []

			//! Directly access to an item in array.
			T& GetItem(u32 Index, type &Pri)
			{
				u32		    i    = 0;
				ArraypItem* temp = start->next;
				while (i < Index)
				{
					temp = temp->next;
					++i;
				}
				Pri = temp->Pri;
				return temp->Item;

			} // GetItem

			//! Directly access to an item in array.
			const T& GetItem(u32 Index, type &Pri) const
			{
				u32		    i    = 0;
				ArraypItem* temp = start->next;
				while (i < Index)
				{
					temp = temp->next;
					++i;
				}
				Pri = temp->Pri;
				return temp->Item;

			} // GetItem

			//! Removes an item from array.
			void remove(T Item)
			{
				ArraypItem* temp = start->next;
				while (temp)
				{
					if (temp->Item == Item)
					{
						temp->prev->next = temp->next;
						if (temp->next) temp->next->prev = temp->prev;
						delete temp;
						--m_iCount;
						return;
					}
					temp = temp->next;
				}

			} // Remove(Item)

			//! Return Items count in the array.
			u32 Count() const
			{ 
				return m_iCount; 

			} // Count

			//! Clear the array.
			void Clear()
			{
				ArraypItem* temp = start->next;
				if (!temp)
					return;
				while (temp->next)
				{
					temp = temp->next;
					KGE_DELETE(temp->prev, ArraypItem);
				}
				KGE_DELETE(temp, ArraypItem);
				m_iCount	= 0;
				start->next = NULL;

			} // Clear

		protected:

			//! The start link.
			ArraypItem *start;

			//! The count of the items.
			u32 m_iCount;

		}; // Array

	} // core

} // kge

#endif // ARRAYPRIORITY_H