#pragma once

#include "../kgedef.h"

namespace kge
{
namespace core
{
class KGE_API KgeArray
{
public:

	//! Constructor
	KgeArray(uint SizeofItem, uint InitItemCount, uint ExpandCount);

	//! Destructor
	~KgeArray();

	//! Adds an element
	void Add(void* data, u32 count = 1);

	//! Gets an item by its index
	void* Get(uint index);

	//! Returns the array size
	uint Size() {return m_iItemCount;}

	//! Clear the array
	void Clear() {m_iItemCount = 0;}

	//! Get Array Data
	u8* GetData() {return m_pData;}

protected:

	u8				*	m_pData;
	uint				m_iSizeofItem,
						m_iArraySize,
						m_iItemCount,
						m_iExpandCount;

	//! Expand the array size
	void Expand();
	
}; // KgeArray

} // core

} // kge
