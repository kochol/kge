// File name: Array2D.h
// Des: This class is for working with 2D arrays easier
// Date: 24/8/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_ARRAY2D_H
#define KGE_ARRAY2D_H

#include <algorithm>
#include "KgeMemory.h"

namespace kge
{
namespace core
{
template <class T>
class Array2D
{
public:

	//! Constructor
	Array2D(int width, int height)
	{
		m_iWidth  = width;
		m_iHeight = height;
		m_pArray  = KGE_NEW_ARRAY(T, width * height);
	}

    //! Copy constructor
    Array2D(const Array2D& other)
        : m_pArray(KGE_NEW_ARRAY(T, other.m_iWidth * other.m_iHeight)),
        m_iWidth(other.m_iWidth), m_iHeight(other.m_iHeight)
    {
        std::copy(other.m_pArray, other.m_pArray + m_iWidth * m_iHeight, m_pArray);
    }

	//! Destructor
	~Array2D()
	{
		KGE_DELETE_ARRAY(m_pArray);

	} // Destructor

    // Assignment operator
    Array2D& operator =(const Array2D& other)
    {
        Array2D temp(other);
        Swap(temp);
        return *this;
    }

	T* Get(int x, int y)
	{
		return &m_pArray[y * m_iWidth + x];
	}

	T* GetArray()
	{
		return m_pArray;
	}

	void SetArray(T* pData, int width, int height)
	{
		KGE_DELETE_ARRAY(m_pArray);
		m_iWidth  = width;
		m_iHeight = height;
		m_pArray  = pData;
	}

	int GetArraySize()
	{
		return m_iWidth * m_iHeight;
	}

	void Set(T data, int x, int y)
	{
		if (x < 0 || x >= m_iWidth) return;
		if (y < 0 || y >= m_iHeight) return;
		m_pArray[y * m_iWidth + x] = data;
	}

	void Recreate(int width, int height)
	{
		KGE_DELETE_ARRAY(m_pArray);
		m_iWidth  = width;
		m_iHeight = height;
		m_pArray  = KGE_NEW_ARRAY(T, width * height);
	}

	void SetAll(T data)
	{
		for (int j = 0; j < m_iHeight; j++)
			for (int i = 0; i < m_iWidth; i++)
				m_pArray[j * m_iWidth + i] = data;
	}

    void Swap(Array2D& other)
    {
        T* p = this->m_pArray;
        this->m_pArray = other.m_pArray;
        other.m_pArray = p;

        int temp = this->m_iWidth;
        this->m_iWidth = other.m_iWidth;
        other.m_iWidth = temp;

        temp = this->m_iHeight;
        this->m_iHeight = other.m_iHeight;
        other.m_iHeight = temp;
    }

protected:

	T				*	m_pArray;
	int					m_iWidth,
						m_iHeight;

}; // Array2D

} // core

} // kge

#endif // KGE_ARRAY2D_H
