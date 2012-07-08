//#############################################################################
//#	desc:	Dynamic and fixed size array templates
//#	file:	array.h
//#	author:	Pooya Shahinfar (Pswin)
//#############################################################################

#ifndef KGE_ARRAY_H
#define KGE_ARRAY_H

#include <cassert>
#include <iostream>
#include "kgedef.h"
#include "KgeMemory.h"

#if KGE_CPP11_COMPILER ==1
#include <initializer_list>
#endif // KGE_COMPILER_CPP11

namespace kge
{
	namespace core
	{
		//=====================================
		// Fixed size array
		//=====================================
		template <typename T>
		class FixedArray
		{
		public:
			// array of elements of type T
			T *elements;    

			// type definitions
			typedef T              value_type;
			typedef T*             iterator;
			typedef const T*       const_iterator;
			typedef T&             reference;
			typedef const T&       const_reference;

		public:

			//! default constructor
			explicit FixedArray(u32 _size = 10)
				: m_Size(_size)
			{
				elements = KGE_NEW_ARRAY(T,_size);
			};

			//! initializer constructor
			#if KGE_CPP11_COMPILER == 1
				FixedArray(std::initializer_list<T> seq)
				{
					elements = KGE_NEW_ARRAY(T,seq.size());
					m_Size = seq.size();
					int i = 0;
					for (auto it = seq.begin() ; it != seq.end() ; it++,i++)
					{
						elements[i] = (*it);
					}
				};
			#endif // KGE_COMPILER_CPP11

			//! copy constructor
			FixedArray(const FixedArray<T>& ar)
			{
				m_Size = ar.size()
				memcpy(elements,ar.elements,sizeof(T) * ar.size());
			};

			//! destructor
			~FixedArray()
			{
				KGE_DELETE_ARRAY(elements);
			};

			//! front() 
			reference front() 
			{ 
				return elements[0]; 
			};
        
			const_reference front() const 
			{
				return elements[0];
			};
        
			//! back
			reference back() 
			{ 
				return elements[(m_Size-1)]; 
			}
        
			const_reference back() const 
			{ 
				return elements[(m_Size-1)]; 
			}

			//! size of array
			u32 size() { return m_Size;};

			//! is array empty?
			bool empty() { return !(bool)m_Size;};

			//! swap (note: linear complexity)
			void swap (FixedArray<T>& y)
			{
				if ( m_Size != y.size())
					return;

				for (u32 i = 0; i < m_Size; ++i)
				{
					T temp = y.elements[i];
					y.elements[i] = elements[i];
					elements[i] = temp;
				}
			};

			// direct access to data (read-only)
			const T* data() const { return elements; }
			T* data() { return elements;};

			// use array as C array (direct read/write access to data)
			T* c_array() { return elements;};

	        //! iterator support for STL, boost:FOR_EACH and C++CIL "for each"
			iterator begin() { return elements; };
			const_iterator begin() const { return elements; };
			iterator end() { return  elements + m_Size ; };
			const_iterator end() const { return elements + m_Size; };

	        //! operator[] - reference
			inline reference operator[](u32 _index) 
			{ 
				assert(_index < m_Size);
				return elements[_index];
			}

			//! operator [] - const ref
			inline const_reference operator[](u32 _index) const 
			{     
				assert(_index <m_Size);
				return elements[_index]; 
			}

			// at() with range check
			reference at(u32 _index)
			{
				assert(_index <m_Size);
				return elements[_index]; 
			}
			const_reference at(u32  _index) const		
			{ 
				assert(_index <m_Size);
				return elements[_index];
			}
    
			//! assign one value to all elements
			void assign (const T& _value) { fill ( _value ); }    // A synonym for fill
      
			//! fill all elements with specified value
			void fill   (const T& _value)
			{
	            std::fill_n(begin(),size(),_value);
			}

		private:
			u32 m_Size; // size of array
		}; // Array

		//=====================================
		// Dynamic Array
		//=====================================
		template <typename T,u32 block_size = KGE_DEFAULT_ARRAY_BLOCK_SIZE>
		class DynamicArray
		{
		public:
			// array of elements of type T
			T *elements;    

			// type definitions
			typedef T              value_type;
			typedef T*             iterator;
			typedef const T*       const_iterator;
			typedef T&             reference;
			typedef const T&       const_reference;

		public:
			//! default constructor
			explicit DynamicArray()
				: m_Size(0),m_MaxSize(0),elements(NULL)
			{}

			//! copy constructor
			DynamicArray(const DynamicArray<T>& ar)
			{
				m_Size	 = ar.size();
				elements = KGE_NEW_ARRAY(T, m_Size);
				memcpy(elements, ar.elements, sizeof(T) * ar.size());
			}


			//! initializer constructor
			#if KGE_CPP11_COMPILER == 1
			DynamicArray(std::initializer_list<T> seq)
			{
				elements = KGE_NEW_ARRAY(T,seq.size());
				m_Size = seq.size();
				int i = 0;
				for (auto it = seq.begin() ; it != seq.end() ; it++,i++)
				{
					push_back((*it));
				}
			}
			#endif // KGE_COMPILER_CPP11

			//! destructor
			~DynamicArray()
			{
				KGE_DELETE_ARRAY(elements);
			}

			void clear()
			{
				m_Size = 0;
				KGE_DELETE_ARRAY(elements);
			}

			//! front() 
			reference front() 
			{ 
				return elements[0]; 
			}

			//! front - const_renrence
			const_reference front() const 
			{
				return elements[0];
			}

			//! back
			reference back() 
			{ 
				return elements[(m_Size-1)]; 
			}

			//! back - const_refrence
			const_reference back() const 
			{ 
				return elements[(m_Size-1)]; 
			}

			//! size of array
			inline u32 size(void) const { return m_Size; }

			//! return max size of array
			inline u32 max_size(void) const {return m_MaxSize;};

			//! is array empty?
			bool empty(void) const { return !(bool)m_Size; }

			//! swap (note: linear complexity)
			void swap (DynamicArray<T>& y)
			{
				///TODO:
			}

			// direct access to data (read-only)
			const T* data() const { return elements; }
			T* data() { return elements; }

			// use array as C array (direct read/write access to data)
			T* c_array() { return elements; }

			//! iterator support for STL, boost:FOR_EACH and C++ CIL "for each"
			iterator begin() { return elements; };
			const_iterator begin() const { return elements; };
			iterator end() { return  elements + m_Size ; };
			const_iterator end() const { return elements + m_Size; };

			//! operator[] - reference
			inline reference operator[](u32 index) 
			{ 
				assert(index < m_Size);
				return elements[index];
			}

			//! operator [] - const ref
			inline const_reference operator[](u32 index) const 
			{     
				assert(index <m_Size);
				return elements[index]; 
			}

			// at() with range check
			reference at(u32 _index)
			{
				assert(index <m_Size);
				return elements[_index]; 
			}
			const_reference at(u32  _index) const		
			{ 
				assert(index <m_Size);
				return elements[_index];
			}

			//! assign one value to all elements
			void assign (const T& _value) { fill ( _value ); }    // A synonym for fill

			//! fill all elements with specified value
			void fill(const T& _value)
			{
				std::fill_n(begin(),size(),_value);
			}

			//! add an element to array
			void push_back(const_reference _item)
			{
				if(m_Size == m_MaxSize) 
				{
					expand();
				}
				elements[m_Size] = _item;
				m_Size++;				

			}

			//! remove an item form array
			void erase(iterator _where)
			{
				memcpy(_where,(_where +1),(end() - (_where+1)) * sizeof(T) );
				m_Size--;
				if ((m_MaxSize - m_Size) > block_size)
					contract();
			}

			//! remove an item by index
			void erase(u32 _index)
			{
				if (_index > m_Size)
					return;
				memcpy(elements + _index,elements+_index+1,(m_Size - _index - 1) * sizeof(T));
				m_Size--;
				if ((m_MaxSize - m_Size) > block_size)
					contract();
			}
		private:
			u32 m_MaxSize;	//! maximum number that array can hold with out expanding
			u32 m_Size;		//! size of the array

			//! expand array
			inline void expand(void)
			{
				m_MaxSize += block_size;
				T* temp_buf = elements;
				elements = KGE_NEW_ARRAY(T,m_MaxSize);
				memcpy(elements,temp_buf,m_Size * sizeof(T));
				KGE_DELETE_ARRAY(temp_buf);
			}

			//! contract array
			inline void contract(void)
			{
				m_MaxSize -= block_size;
				T* temp_buf = elements;
				elements = KGE_NEW_ARRAY(T,m_MaxSize);
				memcpy(elements,temp_buf,m_Size * sizeof(T));
				KGE_DELETE_ARRAY(temp_buf);
			}
		}; // DynamicArray
	} // core
} // kge
#endif // KGE_ARRAY_H