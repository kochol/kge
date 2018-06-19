#include "../../Include/core/KgeArray.h"
#include "../../Include/core/KgeMemory.h"
#include <string.h>

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
namespace core
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	KgeArray::KgeArray( uint SizeofItem, uint InitItemCount, uint ExpandCount ):
		m_iSizeofItem(SizeofItem), m_iItemCount(0), m_iArraySize(InitItemCount),
			m_iExpandCount(ExpandCount)
	{
		m_pData = (u8*)KgeMemory::AllocateBytes(SizeofItem * InitItemCount, __FILE__, __LINE__, __FUNCTION__);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	KgeArray::~KgeArray()
	{
		KGE_DELETE_ARRAY(m_pData);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Adds an element
	//------------------------------------------------------------------------------------
	void KgeArray::Add( void* data, u32 count )
	{
		while (m_iItemCount + count >= m_iArraySize)
			Expand();

		memcpy(m_pData + (m_iItemCount * m_iSizeofItem), data, m_iSizeofItem * count);
		m_iItemCount += count;

	} // Add

	//------------------------------------------------------------------------------------
	// Gets an item by its index
	//------------------------------------------------------------------------------------
	void* KgeArray::Get( uint index )
	{
		return m_pData + (index * m_iSizeofItem);

	} // Get

	//------------------------------------------------------------------------------------
	// Expand the array size
	//------------------------------------------------------------------------------------
	void KgeArray::Expand()
	{
		m_iArraySize += m_iExpandCount;
		m_pData = (u8*)KgeMemory::ReallocateBytes(m_pData, m_iSizeofItem * m_iArraySize,
			__FILE__, __LINE__, __FUNCTION__);

	} // Expand

} // core

} // kge
